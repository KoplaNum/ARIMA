#ifndef AA_ARIMA_DATA_C
#define AA_ARIMA_DATA_C

static void aa_copy_chrono(vars inputSeries,int sampleCount,vars outputSeries)
{
  if(inputSeries && outputSeries && sampleCount > 0)
    memcpy(outputSeries,inputSeries,sampleCount*sizeof(var));
}

static void aa_reverse_newest_to_chrono(vars inputSeries,int sampleCount,vars outputSeries)
{
  int sampleIndex;

  if(!inputSeries || !outputSeries || sampleCount <= 0)
    return;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    outputSeries[sampleIndex] = inputSeries[sampleCount-1-sampleIndex];
}

int aa_validate_series(vars seriesValues,int sampleCount)
{
  int sampleIndex;

  if(!seriesValues || sampleCount <= 1)
    return 0;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    if(invalid(seriesValues[sampleIndex]))
      return 0;

  return 1;
}

int aa_validate_price_series(vars closeSeries,int sampleCount)
{
  int sampleIndex;

  if(!closeSeries || sampleCount <= 1)
    return 0;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    if(invalid(closeSeries[sampleIndex]) || closeSeries[sampleIndex] <= 0.)
      return 0;

  return 1;
}

int aa_has_invalid_values(vars seriesValues,int sampleCount)
{
  int sampleIndex;

  if(!seriesValues || sampleCount <= 0)
    return 1;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    if(invalid(seriesValues[sampleIndex]))
      return 1;

  return 0;
}

int aa_count_invalid_values(vars seriesValues,int sampleCount)
{
  int sampleIndex;
  int invalidCount;

  if(!seriesValues || sampleCount <= 0)
    return 0;

  invalidCount = 0;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    if(invalid(seriesValues[sampleIndex]))
      invalidCount++;

  return invalidCount;
}

int aa_remove_invalid_values(vars inputSeries,int sampleCount,vars outputSeries)
{
  int sampleIndex;
  int outputCount;

  if(!inputSeries || !outputSeries || sampleCount <= 0)
    return 0;

  outputCount = 0;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++) {
    if(!invalid(inputSeries[sampleIndex]))
      outputSeries[outputCount++] = inputSeries[sampleIndex];
  }

  return outputCount;
}

void aa_fill_missing_forward(vars seriesValues,int sampleCount)
{
  int sampleIndex;
  var lastValidValue;

  if(!seriesValues || sampleCount <= 0)
    return;

  lastValidValue = 0.;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++) {
    if(invalid(seriesValues[sampleIndex]))
      seriesValues[sampleIndex] = lastValidValue;
    else
      lastValidValue = seriesValues[sampleIndex];
  }
}

void aa_fill_missing_mean(vars seriesValues,int sampleCount)
{
  int sampleIndex;
  int validCount;
  var validSum;
  var replacementMean;

  if(!seriesValues || sampleCount <= 0)
    return;

  validCount = 0;
  validSum = 0.;
  replacementMean = 0.;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++) {
    if(!invalid(seriesValues[sampleIndex])) {
      validSum += seriesValues[sampleIndex];
      validCount++;
    }
  }

  if(validCount > 0)
    replacementMean = validSum/(var)validCount;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    if(invalid(seriesValues[sampleIndex]))
      seriesValues[sampleIndex] = replacementMean;
}

void aa_replace_zero_prices(vars seriesValues,int sampleCount,var replacementValue)
{
  int sampleIndex;

  if(!seriesValues || sampleCount <= 0)
    return;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    if(seriesValues[sampleIndex] <= 0.)
      seriesValues[sampleIndex] = replacementValue;
}

void aa_clip_outliers(vars seriesValues,int sampleCount,var lowerBound,var upperBound)
{
  int sampleIndex;

  if(!seriesValues || sampleCount <= 0)
    return;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    seriesValues[sampleIndex] = clamp(seriesValues[sampleIndex],lowerBound,upperBound);
}

void aa_winsorize_series(vars seriesValues,int sampleCount,var percentileFraction)
{
  var lowerValue;
  var upperValue;
  var valueRange;
  var clampedPercentile;

  if(!seriesValues || sampleCount <= 0)
    return;

  clampedPercentile = clamp(percentileFraction,0.,0.49);
  lowerValue = aa_min(seriesValues,sampleCount);
  upperValue = aa_max(seriesValues,sampleCount);
  valueRange = upperValue-lowerValue;
  aa_clip_outliers(seriesValues,sampleCount,lowerValue+valueRange*clampedPercentile,upperValue-valueRange*clampedPercentile);
}

void aa_limit_returns(vars seriesValues,int sampleCount,var maxAbsReturn)
{
  int sampleIndex;
  var periodReturn;

  if(!seriesValues || sampleCount <= 1)
    return;

  for(sampleIndex=1;sampleIndex<sampleCount;sampleIndex++) {
    if(abs(seriesValues[sampleIndex-1]) <= AA_EPS)
      continue;

    periodReturn = seriesValues[sampleIndex]/seriesValues[sampleIndex-1]-1.;
    periodReturn = clamp(periodReturn,-maxAbsReturn,maxAbsReturn);
    seriesValues[sampleIndex] = seriesValues[sampleIndex-1]*(1.+periodReturn);
  }
}

#endif
