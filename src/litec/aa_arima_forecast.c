#ifndef AA_ARIMA_FORECAST_C
#define AA_ARIMA_FORECAST_C

static var aa_error_sum(vars actualSeries,vars forecastSeries,int sampleCount,int errorMode)
{
  int sampleIndex;
  int validCount;
  var errorSum;
  var forecastError;

  if(!actualSeries || !forecastSeries || sampleCount <= 0)
    return 0.;

  validCount = 0;
  errorSum = 0.;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++) {
    forecastError = actualSeries[sampleIndex]-forecastSeries[sampleIndex];
    if(errorMode == 1)
      errorSum += forecastError*forecastError;
    else if(errorMode == 2) {
      if(abs(actualSeries[sampleIndex]) > AA_EPS) {
        errorSum += abs(forecastError/actualSeries[sampleIndex]);
        validCount++;
      }
    } else {
      errorSum += abs(forecastError);
    }
  }

  if(errorMode == 2 && validCount > 0)
    return errorSum/(var)validCount;

  return errorSum;
}

static int aa_sign_match(var actualChange,var forecastChange)
{
  if((actualChange >= 0. && forecastChange >= 0.) || (actualChange < 0. && forecastChange < 0.))
    return 1;
  return 0;
}

var aa_forecast_one_step(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  int lagIndex;
  int transformedLength;
  var predictedValue;

  if(!model || !inputSeries || sampleCount <= 0 || !work)
    return 0.;

  transformedLength = aa_difference_series(model->d,inputSeries,sampleCount,work->tmp1);
  if(model->D > 0)
    transformedLength = aa_seasonal_difference_series(work->tmp1,transformedLength,model->D,max(model->m,1),work->tmp2);
  else
    aa_copy_vars(work->tmp2,work->tmp1,transformedLength);

  predictedValue = model->constant;
  for(lagIndex=1;lagIndex<=model->p;lagIndex++)
    if(transformedLength-lagIndex>=0)
      predictedValue += model->ar[lagIndex-1]*work->tmp2[transformedLength-lagIndex];
  for(lagIndex=1;lagIndex<=model->q;lagIndex++)
    if(model->residualCount-lagIndex>=0)
      predictedValue += model->ma[lagIndex-1]*model->residuals[model->residualCount-lagIndex];

  work->tmp3[0] = predictedValue;
  if(model->D > 0)
    aa_inverse_seasonal_difference(work->tmp1,aa_difference_series(model->d,inputSeries,sampleCount,work->tmp1),work->tmp3,1,model->D,max(model->m,1),work->tmp4);
  else
    work->tmp4[0] = predictedValue;

  if(model->d > 0)
    aa_inverse_difference(inputSeries[0],work->tmp4,1,model->d,work->tmp5);
  else
    work->tmp5[0] = work->tmp4[0];

  return work->tmp5[0];
}

int aa_forecast_multi_step(ARIMA_MODEL* model,vars inputSeries,int sampleCount,int forecastHorizon,ARIMA_WORK* work,vars forecastPath)
{
  int horizonIndex;

  if(!model || !inputSeries || !work || !forecastPath || forecastHorizon <= 0)
    return 0;

  for(horizonIndex=0;horizonIndex<forecastHorizon;horizonIndex++)
    forecastPath[horizonIndex] = aa_forecast_one_step(model,inputSeries,sampleCount,work);

  model->forecast = forecastPath[0];
  if(model->forecastPath && model->forecastCap >= forecastHorizon) {
    aa_copy_vars(model->forecastPath,forecastPath,forecastHorizon);
    model->forecastCount = forecastHorizon;
  }

  return 1;
}

var aa_forecast_variance(ARIMA_MODEL* fittedModel,int forecastStep)
{
  if(!fittedModel)
    return 0.;

  return fittedModel->sigma2*max((var)forecastStep,1.);
}

var aa_forecast_standard_error(ARIMA_MODEL* fittedModel,int forecastStep)
{
  return sqrt(aa_forecast_variance(fittedModel,forecastStep));
}

void aa_forecast_confidence_interval(var pointForecast,var forecastStandardError,var confidenceAlpha,var* lowerConfidenceBound,var* upperConfidenceBound)
{
  var zValue;

  zValue = 1.96;
  if(confidenceAlpha >= 0.10)
    zValue = 1.645;
  if(lowerConfidenceBound)
    *lowerConfidenceBound = pointForecast - zValue*forecastStandardError;
  if(upperConfidenceBound)
    *upperConfidenceBound = pointForecast + zValue*forecastStandardError;
}

int aa_forecast_bands(ARIMA_MODEL* fittedModel,vars forecastPath,int forecastHorizon,var confidenceAlpha,ARIMA_WORK* work,vars lowerForecastBand,vars upperForecastBand)
{
  int horizonIndex;

  if(!fittedModel || !forecastPath || !lowerForecastBand || !upperForecastBand)
    return 0;

  for(horizonIndex=0;horizonIndex<forecastHorizon;horizonIndex++)
    aa_forecast_confidence_interval(
      forecastPath[horizonIndex],
      aa_forecast_standard_error(fittedModel,horizonIndex+1),
      confidenceAlpha,
      &lowerForecastBand[horizonIndex],
      &upperForecastBand[horizonIndex]);

  return 1;
}

void aa_integrate_forecast(vars originalSeries,int sampleCount,vars differencedForecast,int forecastHorizon,int diffOrder,vars outputSeries)
{
  aa_inverse_difference_path(originalSeries,sampleCount,differencedForecast,forecastHorizon,diffOrder,outputSeries);
}

void aa_backtransform_forecast(vars transformedForecastPath,int forecastHorizon,int transformMode,var boxCoxLambda,vars outputForecastPath)
{
  if(transformMode == AA_TRANSFORM_LOG)
    aa_inverse_log_transform(transformedForecastPath,forecastHorizon,outputForecastPath);
  else if(transformMode == AA_TRANSFORM_BOXCOX)
    aa_inverse_boxcox_transform(transformedForecastPath,forecastHorizon,boxCoxLambda,outputForecastPath);
  else
    aa_copy_vars(outputForecastPath,transformedForecastPath,forecastHorizon);
}

void aa_bias_adjusted_backtransform(vars transformedForecastPath,vars forecastStandardErrors,int forecastHorizon,int transformMode,var boxCoxLambda,vars biasAdjustedForecastPath)
{
  int horizonIndex;

  if(!transformedForecastPath || !biasAdjustedForecastPath)
    return;

  if(transformMode == AA_TRANSFORM_LOG) {
    for(horizonIndex=0;horizonIndex<forecastHorizon;horizonIndex++)
      biasAdjustedForecastPath[horizonIndex] = exp(transformedForecastPath[horizonIndex] + 0.5*forecastStandardErrors[horizonIndex]*forecastStandardErrors[horizonIndex]);
  } else {
    aa_backtransform_forecast(transformedForecastPath,forecastHorizon,transformMode,boxCoxLambda,biasAdjustedForecastPath);
  }
}

int aa_sarima_forecast_one_step(ARIMA_MODEL* fittedModel,vars closeSeries,int historyLength,ARIMA_WORK* workBuffer,var* nextForecast)
{
  if(!fittedModel || !closeSeries || !workBuffer || !nextForecast)
    return 0;

  *nextForecast = aa_forecast_one_step(fittedModel,closeSeries,historyLength,workBuffer);
  return 1;
}

int aa_sarima_forecast_multi_step(ARIMA_MODEL* fittedModel,vars closeSeries,int historyLength,int forecastHorizon,ARIMA_WORK* workBuffer,vars multiStepForecastPath)
{
  return aa_forecast_multi_step(fittedModel,closeSeries,historyLength,forecastHorizon,workBuffer,multiStepForecastPath);
}

void aa_train_test_split(vars sourceSeries,int totalSampleCount,var trainFraction,vars trainingSeries,vars testingSeries,int* trainingCount,int* testingCount)
{
  int sampleIndex;
  int trainingLength;

  if(!sourceSeries || !trainingSeries || !testingSeries || !trainingCount || !testingCount || totalSampleCount <= 0)
    return;

  trainingLength = clamp((int)floor(trainFraction*(var)totalSampleCount),1,totalSampleCount-1);
  *trainingCount = trainingLength;
  *testingCount = totalSampleCount-trainingLength;
  for(sampleIndex=0;sampleIndex<trainingLength;sampleIndex++)
    trainingSeries[sampleIndex] = sourceSeries[sampleIndex];
  for(sampleIndex=0;sampleIndex<*testingCount;sampleIndex++)
    testingSeries[sampleIndex] = sourceSeries[trainingLength+sampleIndex];
}

int aa_rolling_forecast_test(vars inputSeries,int sampleCount,int lookbackWindow,int forecastHorizon,ARIMA_WORK* work,vars forecastSeries,vars actualSeries)
{
  int windowEndIndex;
  int resultCount;
  ARIMA_MODEL rollingModel;

  if(!inputSeries || !work || !forecastSeries || !actualSeries)
    return 0;

  init_arima_model(&rollingModel);
  resultCount = 0;
  for(windowEndIndex=lookbackWindow;windowEndIndex+forecastHorizon<=sampleCount;windowEndIndex++) {
    if(aa_arima_fit(1,0,0,inputSeries+(sampleCount-windowEndIndex),lookbackWindow,work,&rollingModel)) {
      forecastSeries[resultCount] = aa_forecast_one_step(&rollingModel,inputSeries+(sampleCount-windowEndIndex),lookbackWindow,work);
      actualSeries[resultCount] = inputSeries[sampleCount-windowEndIndex-1];
      resultCount++;
    }
  }

  free_arima_model(&rollingModel);
  return resultCount;
}

int aa_walk_forward_arima(vars inputSeries,int sampleCount,int lookbackWindow,int forecastHorizon,int refitInterval,ARIMA_WORK* work,vars forecastSeries)
{
  int windowEndIndex;
  int resultCount;
  int barsSinceFit;
  ARIMA_MODEL rollingModel;

  if(!inputSeries || !forecastSeries || !work)
    return 0;

  init_arima_model(&rollingModel);
  resultCount = 0;
  barsSinceFit = refitInterval;
  for(windowEndIndex=lookbackWindow;windowEndIndex+forecastHorizon<=sampleCount;windowEndIndex++) {
    if(barsSinceFit >= refitInterval || !rollingModel.converged) {
      aa_arima_fit(1,0,0,inputSeries+(sampleCount-windowEndIndex),lookbackWindow,work,&rollingModel);
      barsSinceFit = 0;
    }

    forecastSeries[resultCount++] = aa_forecast_one_step(&rollingModel,inputSeries+(sampleCount-windowEndIndex),lookbackWindow,work);
    barsSinceFit++;
  }

  free_arima_model(&rollingModel);
  return resultCount;
}

var aa_forecast_error_mae(vars actualSeries,vars forecastSeries,int sampleCount)
{
  return aa_error_sum(actualSeries,forecastSeries,sampleCount,0)/(var)max(sampleCount,1);
}

var aa_forecast_error_mse(vars actualSeries,vars forecastSeries,int sampleCount)
{
  return aa_error_sum(actualSeries,forecastSeries,sampleCount,1)/(var)max(sampleCount,1);
}

var aa_forecast_error_rmse(vars actualSeries,vars forecastSeries,int sampleCount)
{
  return sqrt(aa_forecast_error_mse(actualSeries,forecastSeries,sampleCount));
}

var aa_forecast_error_mape(vars actualSeries,vars forecastSeries,int sampleCount)
{
  return 100.*aa_error_sum(actualSeries,forecastSeries,sampleCount,2);
}

var aa_directional_accuracy(vars actualSeries,vars forecastSeries,int sampleCount)
{
  int sampleIndex;
  int hitCount;
  int comparisonCount;
  var actualChange;
  var forecastChange;

  if(!actualSeries || !forecastSeries || sampleCount <= 1)
    return 0.;

  hitCount = 0;
  comparisonCount = 0;
  for(sampleIndex=1;sampleIndex<sampleCount;sampleIndex++) {
    actualChange = actualSeries[sampleIndex]-actualSeries[sampleIndex-1];
    forecastChange = forecastSeries[sampleIndex]-actualSeries[sampleIndex-1];
    hitCount += aa_sign_match(actualChange,forecastChange);
    comparisonCount++;
  }

  return (var)hitCount/fix0((var)comparisonCount);
}

var aa_forecast_return(var lastObservedValue,var pointForecast)
{
  return (pointForecast-lastObservedValue)/fix0(lastObservedValue);
}

var aa_forecast_edge(var lastObservedValue,var pointForecast,var transactionCost)
{
  return aa_forecast_return(lastObservedValue,pointForecast)-transactionCost;
}

var aa_forecast_zscore(var pointForecast,var forecastStandardError,var lastObservedValue)
{
  return (pointForecast-lastObservedValue)/fix0(forecastStandardError);
}

int aa_signal_from_forecast(var lastObservedValue,var pointForecast,var signalThreshold)
{
  var forecastReturn;

  forecastReturn = aa_forecast_return(lastObservedValue,pointForecast);
  if(forecastReturn > signalThreshold)
    return 1;
  if(forecastReturn < -signalThreshold)
    return -1;

  return 0;
}

int aa_signal_from_confidence_band(var lastObservedValue,var lowerConfidenceBound,var upperConfidenceBound)
{
  if(lowerConfidenceBound > lastObservedValue)
    return 1;
  if(upperConfidenceBound < lastObservedValue)
    return -1;

  return 0;
}

int aa_signal_from_directional_accuracy(var directionalAccuracy,var minimumAccuracy)
{
  return directionalAccuracy >= minimumAccuracy;
}

var aa_position_size_from_forecast(var forecastEdge,var riskBudget,var stopDistance)
{
  return riskBudget*abs(forecastEdge)/fix0(stopDistance);
}

var aa_position_size_from_confidence(var forecastConfidenceZScore,var basePositionSize)
{
  return basePositionSize*min(abs(forecastConfidenceZScore),3.);
}

int aa_backtest_forecast_signal(vars closeSeries,vars forecastSeries,int sampleCount,var signalThreshold,vars signalSeries)
{
  int sampleIndex;

  if(!closeSeries || !forecastSeries || !signalSeries || sampleCount <= 0)
    return 0;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    signalSeries[sampleIndex] = aa_signal_from_forecast(closeSeries[sampleIndex],forecastSeries[sampleIndex],signalThreshold);

  return 1;
}

#endif
