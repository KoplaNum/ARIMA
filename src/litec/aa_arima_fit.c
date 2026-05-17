#ifndef AA_ARIMA_FIT_C
#define AA_ARIMA_FIT_C

static int aa_compute_arma_residuals_internal(vars inputSeries,int sampleCount,int arOrder,int maOrder,vars arCoefficients,vars maCoefficients,var constantTerm,vars residualSeries)
{
  int timeIndex;
  int lagIndex;
  var predictedValue;

  if(!inputSeries || !residualSeries || sampleCount <= 0)
    return 0;

  for(timeIndex=0;timeIndex<sampleCount;timeIndex++) {
    predictedValue = constantTerm;
    for(lagIndex=1;lagIndex<=arOrder;lagIndex++)
      if(timeIndex-lagIndex>=0)
        predictedValue += arCoefficients[lagIndex-1]*inputSeries[timeIndex-lagIndex];
    for(lagIndex=1;lagIndex<=maOrder;lagIndex++)
      if(timeIndex-lagIndex>=0)
        predictedValue += maCoefficients[lagIndex-1]*residualSeries[timeIndex-lagIndex];
    residualSeries[timeIndex] = inputSeries[timeIndex]-predictedValue;
  }

  return 1;
}

static void aa_fill_score_fields(ARIMA_MODEL* model,int sampleCount,int parameterCount)
{
  if(!model)
    return;

  model->logLik = aa_gaussian_loglik_from_sse(model->sse,sampleCount);
  model->aic = aa_aic_score(sampleCount,model->logLik,parameterCount);
  model->aicc = aa_aicc_score_general(sampleCount,model->logLik,parameterCount);
  model->bic = aa_bic_score(sampleCount,model->logLik,parameterCount);
  model->hqic = aa_hqic_score(sampleCount,model->logLik,parameterCount);
  model->score = model->aicc;
}

int aa_arma_fit(int arOrder,int maOrder,vars inputSeries,int sampleCount,var* outSse,var* outConstant,vars arCoefficients,vars maCoefficients)
{
  int sampleIndex;
  var seriesMean;
  var residualSumSquares;
  var* residualBuffer;

  residualBuffer = aa_alloc_vars(sampleCount);
  if(!inputSeries || sampleCount <= 2 || !residualBuffer)
    return 0;

  if(arOrder > 0 && arCoefficients) {
    aa_yule_walker(inputSeries,sampleCount,arOrder,arCoefficients);
    aa_enforce_stationarity(arCoefficients,arOrder);
  }

  if(maOrder > 0 && maCoefficients) {
    aa_acf(inputSeries,sampleCount,maOrder,residualBuffer);
    aa_initial_ma_from_acf(residualBuffer,maOrder,maCoefficients);
    aa_enforce_invertibility(maCoefficients,maOrder);
  }

  seriesMean = aa_mean(inputSeries,sampleCount);
  if(outConstant)
    *outConstant = seriesMean;

  aa_compute_arma_residuals_internal(inputSeries,sampleCount,arOrder,maOrder,arCoefficients,maCoefficients,seriesMean,residualBuffer);
  residualSumSquares = 0.;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    residualSumSquares += residualBuffer[sampleIndex]*residualBuffer[sampleIndex];

  if(outSse)
    *outSse = residualSumSquares;

  aa_free_vars(residualBuffer);
  return 1;
}

static int aa_fit_nonseasonal_into_model(int arOrder,int maOrder,vars stationarySeries,int sampleCount,ARIMA_WORK* work,ARIMA_MODEL* model)
{
  int seasonalIndex;
  var residualSumSquares;
  var constantTerm;

  if(!stationarySeries || !work || !model || sampleCount <= 2)
    return 0;

  if(!aa_prepare_all_model_arrays(model,arOrder,maOrder,model->P,model->Q,sampleCount,work->capacityH,model->betaCap))
    return 0;

  if(!aa_arma_fit(arOrder,maOrder,stationarySeries,sampleCount,&residualSumSquares,&constantTerm,model->ar,model->ma))
    return 0;

  model->constant = constantTerm;
  model->sse = residualSumSquares;
  model->sigma2 = residualSumSquares/max((var)sampleCount,1.);
  model->residualCount = sampleCount;
  aa_compute_arma_residuals_internal(stationarySeries,sampleCount,arOrder,maOrder,model->ar,model->ma,model->constant,model->residuals);
  for(seasonalIndex=0;seasonalIndex<model->P;seasonalIndex++) model->sar[seasonalIndex] = 0.;
  for(seasonalIndex=0;seasonalIndex<model->Q;seasonalIndex++) model->sma[seasonalIndex] = 0.;
  model->converged = 1;
  return 1;
}

int aa_arima_fit(int arOrder,int diffOrder,int maOrder,vars closeSeries,int sampleCount,ARIMA_WORK* work,ARIMA_MODEL* model)
{
  int differencedLength;
  int parameterCount;

  if(!closeSeries || !work || !model)
    return 0;

  if(!aa_prepare_arima_work(work,sampleCount,arOrder,maOrder,0,0,8))
    return 0;

  reset_arima_model(model);
  model->p = arOrder;
  model->d = diffOrder;
  model->q = maOrder;
  model->hasConstant = 1;
  model->lastPrice = closeSeries[0];

  differencedLength = aa_difference_series(diffOrder,closeSeries,sampleCount,work->tmp1);
  if(differencedLength <= max(arOrder,maOrder)+1)
    return 0;

  if(!aa_prepare_all_model_arrays(model,arOrder,maOrder,0,0,differencedLength,work->capacityH,0))
    return 0;

  if(!aa_fit_nonseasonal_into_model(arOrder,maOrder,work->tmp1,differencedLength,work,model))
    return 0;

  parameterCount = aa_candidate_param_count(arOrder,diffOrder,maOrder,0,0,0,1,0);
  model->paramCount = parameterCount;
  aa_fill_score_fields(model,differencedLength,parameterCount);
  return 1;
}

int aa_sarima_fit(int arOrder,int diffOrder,int maOrder,int seasonalArOrder,int seasonalDiffOrder,int seasonalMaOrder,int seasonalPeriod,vars closeSeries,int sampleCount,ARIMA_WORK* work,ARIMA_MODEL* model)
{
  int transformedLength;
  int parameterCount;
  int seasonalIndex;

  if(!closeSeries || !work || !model)
    return 0;

  if(!aa_prepare_arima_work(work,sampleCount,arOrder,maOrder,seasonalArOrder,seasonalMaOrder,8))
    return 0;

  reset_arima_model(model);
  model->p = arOrder;
  model->d = diffOrder;
  model->q = maOrder;
  model->P = seasonalArOrder;
  model->D = seasonalDiffOrder;
  model->Q = seasonalMaOrder;
  model->m = seasonalPeriod;
  model->hasConstant = 1;
  model->lastPrice = closeSeries[0];

  transformedLength = aa_difference_series(diffOrder,closeSeries,sampleCount,work->tmp1);
  if(transformedLength <= 0)
    return 0;

  if(seasonalDiffOrder > 0)
    transformedLength = aa_seasonal_difference_series(work->tmp1,transformedLength,seasonalDiffOrder,seasonalPeriod,work->tmp2);
  else
    aa_copy_vars(work->tmp2,work->tmp1,transformedLength);

  if(transformedLength <= arOrder+maOrder+seasonalArOrder+seasonalMaOrder+1)
    return 0;

  if(!aa_prepare_all_model_arrays(model,arOrder,maOrder,seasonalArOrder,seasonalMaOrder,transformedLength,work->capacityH,0))
    return 0;

  if(!aa_fit_nonseasonal_into_model(arOrder,maOrder,work->tmp2,transformedLength,work,model))
    return 0;

  for(seasonalIndex=0;seasonalIndex<seasonalArOrder;seasonalIndex++)
    model->sar[seasonalIndex] = aa_autocorrelation(work->tmp2,transformedLength,(seasonalIndex+1)*seasonalPeriod)*0.5;
  for(seasonalIndex=0;seasonalIndex<seasonalMaOrder;seasonalIndex++)
    model->sma[seasonalIndex] = -aa_autocorrelation(work->tmp2,transformedLength,(seasonalIndex+1)*seasonalPeriod)*0.25;

  parameterCount = aa_candidate_param_count(arOrder,diffOrder,maOrder,seasonalArOrder,seasonalDiffOrder,seasonalMaOrder,1,0);
  model->paramCount = parameterCount;
  aa_fill_score_fields(model,transformedLength,parameterCount);
  return 1;
}

int aa_css_fit(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  int sampleIndex;

  if(!model || !inputSeries || !work)
    return 0;

  aa_compute_arma_residuals_internal(inputSeries,sampleCount,model->p,model->q,model->ar,model->ma,model->constant,work->residuals);
  model->sse = 0.;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    model->sse += work->residuals[sampleIndex]*work->residuals[sampleIndex];
  model->sigma2 = max(model->sse/max((var)sampleCount,1.),AA_EPS);
  return 1;
}

#endif
