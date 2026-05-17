#ifndef AA_ARIMA_EXOGENOUS_C
#define AA_ARIMA_EXOGENOUS_C

static void aa_apply_beta_adjustment(ARIMA_MODEL* model,vars designMatrix,int numRows,int numCols,vars inputSeries,vars adjustedSeries)
{
  int rowIndex;
  int colIndex;

  if(!model || !designMatrix || !inputSeries || !adjustedSeries)
    return;

  for(rowIndex=0;rowIndex<numRows;rowIndex++) {
    adjustedSeries[rowIndex] = inputSeries[rowIndex];
    for(colIndex=0;colIndex<numCols;colIndex++)
      adjustedSeries[rowIndex] -= designMatrix[rowIndex*numCols+colIndex]*model->beta[colIndex];
  }
}

static void aa_add_future_beta(ARIMA_MODEL* model,vars* futureExogenous,int forecastHorizon,vars forecastPath)
{
  int horizonIndex;
  int colIndex;
  var adjustedForecast;

  if(!model || !futureExogenous || !forecastPath)
    return;

  for(horizonIndex=0;horizonIndex<forecastHorizon;horizonIndex++) {
    adjustedForecast = forecastPath[horizonIndex];
    for(colIndex=0;colIndex<model->betaCap;colIndex++)
      adjustedForecast += futureExogenous[colIndex][horizonIndex]*model->beta[colIndex];
    forecastPath[horizonIndex] = adjustedForecast;
  }
}

int aa_prepare_exogenous_matrix(vars* exogenousColumns,int numRows,int numCols,ARIMA_WORK* work)
{
  int rowIndex;
  int colIndex;

  if(!exogenousColumns || !work || numRows <= 0 || numCols <= 0)
    return 0;

  if(!aa_prepare_arima_work(work,numRows,max(numCols,4),max(numCols,4),0,0,4))
    return 0;

  for(colIndex=0;colIndex<numCols;colIndex++)
    for(rowIndex=0;rowIndex<numRows;rowIndex++)
      work->matrix[rowIndex*numCols+colIndex] = exogenousColumns[colIndex][rowIndex];

  return 1;
}

int aa_validate_exogenous_data(vars* exogenousColumns,int numRows,int numCols)
{
  int rowIndex;
  int colIndex;

  if(!exogenousColumns || numRows <= 0 || numCols <= 0)
    return 0;

  for(colIndex=0;colIndex<numCols;colIndex++)
    for(rowIndex=0;rowIndex<numRows;rowIndex++)
      if(invalid(exogenousColumns[colIndex][rowIndex]))
        return 0;

  return 1;
}

int aa_regression_fit(vars responseSeries,vars* exogenousColumns,int numRows,int numCols,vars betaOutput)
{
  int rowIndex;
  int colIndex;
  int fitSucceeded;
  var* designMatrix = aa_alloc_vars(numRows*numCols);
  var* regressionWork = aa_alloc_vars(numCols*numCols*3 + numCols*4 + 16);

  if(!responseSeries || !exogenousColumns || !betaOutput || numRows <= 0 || numCols <= 0 || !designMatrix || !regressionWork) {
    aa_free_vars(designMatrix);
    aa_free_vars(regressionWork);
    return 0;
  }

  for(rowIndex=0;rowIndex<numRows;rowIndex++)
    for(colIndex=0;colIndex<numCols;colIndex++)
      designMatrix[rowIndex*numCols+colIndex] = exogenousColumns[colIndex][rowIndex];

  fitSucceeded = aa_ols_fit(responseSeries,designMatrix,numRows,numCols,betaOutput,regressionWork);
  aa_free_vars(designMatrix);
  aa_free_vars(regressionWork);
  return fitSucceeded;
}

var aa_regression_predict(vars exogenousRow,vars betaCoefficients,int numCols)
{
  int colIndex;
  var prediction;

  if(!exogenousRow || !betaCoefficients || numCols <= 0)
    return 0.;

  prediction = 0.;
  for(colIndex=0;colIndex<numCols;colIndex++)
    prediction += exogenousRow[colIndex]*betaCoefficients[colIndex];

  return prediction;
}

int aa_arimax_fit(int arOrder,int diffOrder,int maOrder,vars closeSeries,vars* exogenousColumns,int sampleCount,int exogenousCount,ARIMA_WORK* work,ARIMA_MODEL* model)
{
  int rowIndex;
  int colIndex;
  int differencedLength;
  int regressionRows;

  if(!closeSeries || !exogenousColumns || !work || !model)
    return 0;

  if(!aa_prepare_arima_work(work,sampleCount,max(arOrder,exogenousCount+2),max(maOrder,exogenousCount+2),0,0,8))
    return 0;

  differencedLength = aa_difference_series(diffOrder,closeSeries,sampleCount,work->tmp1);
  regressionRows = differencedLength;
  for(rowIndex=0;rowIndex<regressionRows;rowIndex++)
    for(colIndex=0;colIndex<exogenousCount;colIndex++)
      work->matrix[rowIndex*exogenousCount+colIndex] = exogenousColumns[colIndex][sampleCount-differencedLength+rowIndex];

  if(!aa_prepare_all_model_arrays(model,arOrder,maOrder,0,0,differencedLength,work->capacityH,exogenousCount))
    return 0;

  if(!aa_ols_fit(work->tmp1,work->matrix,regressionRows,exogenousCount,model->beta,work->workspace))
    return 0;

  aa_apply_beta_adjustment(model,work->matrix,regressionRows,exogenousCount,work->tmp1,work->tmp2);
  model->betaCap = exogenousCount;
  model->p = arOrder;
  model->d = diffOrder;
  model->q = maOrder;

  if(!aa_fit_nonseasonal_into_model(arOrder,maOrder,work->tmp2,differencedLength,work,model))
    return 0;

  aa_fill_score_fields(model,differencedLength,aa_candidate_param_count(arOrder,diffOrder,maOrder,0,0,0,1,exogenousCount));
  return 1;
}

int aa_arimax_forecast(ARIMA_MODEL* model,vars closeSeries,vars* futureExogenous,int sampleCount,int forecastHorizon,ARIMA_WORK* work,vars forecastPath)
{
  if(!model || !futureExogenous || !forecastPath || !work)
    return 0;

  if(!aa_forecast_multi_step(model,closeSeries,sampleCount,forecastHorizon,work,forecastPath))
    return 0;

  aa_add_future_beta(model,futureExogenous,forecastHorizon,forecastPath);
  return 1;
}

int aa_sarimax_fit(int arOrder,int diffOrder,int maOrder,int seasonalArOrder,int seasonalDiffOrder,int seasonalMaOrder,int seasonalPeriod,vars closeSeries,vars* exogenousColumns,int sampleCount,int exogenousCount,ARIMA_WORK* work,ARIMA_MODEL* model)
{
  if(!aa_arimax_fit(arOrder,diffOrder,maOrder,closeSeries,exogenousColumns,sampleCount,exogenousCount,work,model))
    return 0;

  model->P = seasonalArOrder;
  model->D = seasonalDiffOrder;
  model->Q = seasonalMaOrder;
  model->m = seasonalPeriod;
  return 1;
}

int aa_sarimax_forecast(ARIMA_MODEL* model,vars closeSeries,vars* futureExogenous,int sampleCount,int forecastHorizon,ARIMA_WORK* work,vars forecastPath)
{
  return aa_arimax_forecast(model,closeSeries,futureExogenous,sampleCount,forecastHorizon,work,forecastPath);
}

#endif
