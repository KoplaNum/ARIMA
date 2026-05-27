#ifndef AA_ARIMA_TRANSFORM_C
#define AA_ARIMA_TRANSFORM_C

void aa_log_transform(vars inputSeries,int sampleCount,vars outputSeries)
{
  int sampleIndex;

  if(!inputSeries || !outputSeries || sampleCount <= 0)
    return;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    outputSeries[sampleIndex] = log(max(inputSeries[sampleIndex],AA_EPS));
}

void aa_inverse_log_transform(vars inputSeries,int sampleCount,vars outputSeries)
{
  int sampleIndex;

  if(!inputSeries || !outputSeries || sampleCount <= 0)
    return;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    outputSeries[sampleIndex] = exp(inputSeries[sampleIndex]);
}

var aa_boxcox_lambda(vars inputSeries,int sampleCount)
{
  return 0.;
}

void aa_boxcox_transform(vars inputSeries,int sampleCount,var lambdaValue,vars outputSeries)
{
  int sampleIndex;

  if(!inputSeries || !outputSeries || sampleCount <= 0)
    return;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++) {
    if(abs(lambdaValue) <= 0.000001)
      outputSeries[sampleIndex] = log(max(inputSeries[sampleIndex],AA_EPS));
    else
      outputSeries[sampleIndex] = (pow(max(inputSeries[sampleIndex],AA_EPS),lambdaValue)-1.)/lambdaValue;
  }
}

void aa_inverse_boxcox_transform(vars inputSeries,int sampleCount,var lambdaValue,vars outputSeries)
{
  int sampleIndex;
  var transformedBase;

  if(!inputSeries || !outputSeries || sampleCount <= 0)
    return;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++) {
    if(abs(lambdaValue) <= 0.000001) {
      outputSeries[sampleIndex] = exp(inputSeries[sampleIndex]);
    } else {
      transformedBase = lambdaValue*inputSeries[sampleIndex]+1.;
      if(transformedBase < AA_EPS)
        transformedBase = AA_EPS;
      outputSeries[sampleIndex] = pow(transformedBase,1./lambdaValue);
    }
  }
}

void aa_return_transform(vars closeSeries,int sampleCount,vars returnSeries)
{
  int sampleIndex;

  if(!closeSeries || !returnSeries || sampleCount <= 1)
    return;

  for(sampleIndex=1;sampleIndex<sampleCount;sampleIndex++)
    returnSeries[sampleIndex-1] = closeSeries[sampleIndex]/fix0(closeSeries[sampleIndex-1])-1.;
}

void aa_inverse_return_forecast(var lastPrice,vars returnForecast,int forecastHorizon,vars priceForecast)
{
  int horizonIndex;
  var runningPrice;

  if(!returnForecast || !priceForecast || forecastHorizon <= 0)
    return;

  runningPrice = lastPrice;
  for(horizonIndex=0;horizonIndex<forecastHorizon;horizonIndex++) {
    runningPrice *= 1.+returnForecast[horizonIndex];
    priceForecast[horizonIndex] = runningPrice;
  }
}

void aa_difference_once(vars inputSeries,int sampleCount,vars outputSeries)
{
  int sampleIndex;

  if(!inputSeries || !outputSeries || sampleCount <= 1)
    return;

  for(sampleIndex=1;sampleIndex<sampleCount;sampleIndex++)
    outputSeries[sampleIndex-1] = inputSeries[sampleIndex]-inputSeries[sampleIndex-1];
}

void aa_difference_twice(vars inputSeries,int sampleCount,vars outputSeries)
{
  if(!inputSeries || !outputSeries || sampleCount <= 2)
    return;

  aa_difference_once(inputSeries,sampleCount,outputSeries);
  aa_difference_once(outputSeries,sampleCount-1,outputSeries);
}

int aa_difference_series(int diffOrder,vars closeSeries,int sampleCount,vars differencedSeries)
{
  int diffIndex;
  int workingLength;

  if(!closeSeries || !differencedSeries || sampleCount <= 0)
    return 0;

  aa_reverse_newest_to_chrono(closeSeries,sampleCount,differencedSeries);
  workingLength = sampleCount;
  for(diffIndex=0;diffIndex<diffOrder;diffIndex++) {
    aa_difference_once(differencedSeries,workingLength,differencedSeries);
    workingLength--;
    if(workingLength <= 0)
      break;
  }

  return workingLength;
}

void aa_inverse_difference(var lastValue,vars differencedForecast,int forecastHorizon,int diffOrder,vars outputSeries)
{
  int horizonIndex;
  var runningValue;

  if(!differencedForecast || !outputSeries || forecastHorizon <= 0)
    return;

  runningValue = lastValue;
  for(horizonIndex=0;horizonIndex<forecastHorizon;horizonIndex++) {
    runningValue += differencedForecast[horizonIndex];
    outputSeries[horizonIndex] = runningValue;
  }
}

void aa_inverse_difference_path(vars originalSeries,int sampleCount,vars differencedForecast,int forecastHorizon,int diffOrder,vars outputSeries)
{
  if(!originalSeries || sampleCount <= 0 || !differencedForecast || !outputSeries)
    return;

  aa_inverse_difference(originalSeries[0],differencedForecast,forecastHorizon,diffOrder,outputSeries);
}

void aa_seasonal_difference_once(vars inputSeries,int sampleCount,int seasonalPeriod,vars outputSeries)
{
  int sampleIndex;

  if(!inputSeries || !outputSeries || sampleCount <= seasonalPeriod || seasonalPeriod <= 0)
    return;

  for(sampleIndex=seasonalPeriod;sampleIndex<sampleCount;sampleIndex++)
    outputSeries[sampleIndex-seasonalPeriod] = inputSeries[sampleIndex]-inputSeries[sampleIndex-seasonalPeriod];
}

int aa_seasonal_difference_series(vars inputSeries,int sampleCount,int seasonalDiffOrder,int seasonalPeriod,vars outputSeries)
{
  int diffIndex;
  int workingLength;

  if(!inputSeries || !outputSeries || sampleCount <= 0)
    return 0;

  aa_copy_chrono(inputSeries,sampleCount,outputSeries);
  workingLength = sampleCount;
  for(diffIndex=0;diffIndex<seasonalDiffOrder;diffIndex++) {
    aa_seasonal_difference_once(outputSeries,workingLength,seasonalPeriod,outputSeries);
    workingLength -= seasonalPeriod;
    if(workingLength <= 0)
      return 0;
  }

  return workingLength;
}

void aa_inverse_seasonal_difference(vars originalSeries,int sampleCount,vars seasonalForecast,int forecastHorizon,int seasonalDiffOrder,int seasonalPeriod,vars outputSeries)
{
  int horizonIndex;
  int sourceIndex;

  if(!originalSeries || !seasonalForecast || !outputSeries || sampleCount <= 0 || forecastHorizon <= 0 || seasonalPeriod <= 0)
    return;

  for(horizonIndex=0;horizonIndex<forecastHorizon;horizonIndex++) {
    sourceIndex = sampleCount-seasonalPeriod+horizonIndex;
    if(sourceIndex < sampleCount)
      outputSeries[horizonIndex] = seasonalForecast[horizonIndex] + originalSeries[sourceIndex];
    else
      outputSeries[horizonIndex] = seasonalForecast[horizonIndex] + outputSeries[horizonIndex-seasonalPeriod];
  }
}

#endif
