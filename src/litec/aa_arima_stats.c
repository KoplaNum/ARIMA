#ifndef AA_ARIMA_STATS_C
#define AA_ARIMA_STATS_C

static var aa_pvalue_bucket_3(var statisticValue,var thresholdA,var thresholdB,var probabilityA,var probabilityB,var probabilityC)
{
  if(statisticValue < thresholdA)
    return probabilityA;
  if(statisticValue < thresholdB)
    return probabilityB;
  return probabilityC;
}

static var aa_interp_linear(var x0,var y0,var x1,var y1,var xValue)
{
  if(abs(x1 - x0) <= AA_EPS)
    return y0;

  return y0 + (y1 - y0) * (xValue - x0) / (x1 - x0);
}

static var aa_left_tail_pvalue_from_critical(var testStatistic,var criticalValue1,var probability1,var criticalValue2,var probability2,var criticalValue3,var probability3)
{
  if(testStatistic <= criticalValue1)
    return clamp(aa_interp_linear(criticalValue1,probability1,criticalValue1-1.,0.001,testStatistic),0.001,1.);
  if(testStatistic <= criticalValue2)
    return clamp(aa_interp_linear(criticalValue1,probability1,criticalValue2,probability2,testStatistic),0.001,1.);
  if(testStatistic <= criticalValue3)
    return clamp(aa_interp_linear(criticalValue2,probability2,criticalValue3,probability3,testStatistic),0.001,1.);
  return clamp(aa_interp_linear(criticalValue3,probability3,criticalValue3+2.,0.50,testStatistic),0.001,0.999);
}

static var aa_right_tail_pvalue_from_critical(var testStatistic,var criticalValue1,var probability1,var criticalValue2,var probability2,var criticalValue3,var probability3)
{
  if(testStatistic >= criticalValue3)
    return clamp(aa_interp_linear(criticalValue3,probability3,criticalValue3+1.5,0.001,testStatistic),0.001,1.);
  if(testStatistic >= criticalValue2)
    return clamp(aa_interp_linear(criticalValue2,probability2,criticalValue3,probability3,testStatistic),0.001,1.);
  if(testStatistic >= criticalValue1)
    return clamp(aa_interp_linear(criticalValue1,probability1,criticalValue2,probability2,testStatistic),0.001,1.);
  return clamp(aa_interp_linear(criticalValue1,probability1,criticalValue1-0.5,0.50,testStatistic),0.001,0.999);
}

var aa_autocovariance(vars seriesValues,int sampleCount,int lagOrder)
{
  int sampleIndex;
  var seriesMean;
  var covarianceSum;

  if(!seriesValues || lagOrder < 0 || lagOrder >= sampleCount || sampleCount <= 0)
    return 0.;

  seriesMean = aa_mean(seriesValues,sampleCount);
  covarianceSum = 0.;
  for(sampleIndex=lagOrder;sampleIndex<sampleCount;sampleIndex++)
    covarianceSum += (seriesValues[sampleIndex]-seriesMean)*(seriesValues[sampleIndex-lagOrder]-seriesMean);

  return covarianceSum/(var)sampleCount;
}

var aa_autocorrelation(vars seriesValues,int sampleCount,int lagOrder)
{
  return aa_autocovariance(seriesValues,sampleCount,lagOrder)/fix0(aa_autocovariance(seriesValues,sampleCount,0));
}

void aa_acf(vars seriesValues,int sampleCount,int maxLag,vars outputAcf)
{
  int lagIndex;

  if(!seriesValues || !outputAcf || sampleCount <= 0)
    return;

  for(lagIndex=0;lagIndex<=maxLag;lagIndex++)
    outputAcf[lagIndex] = aa_autocorrelation(seriesValues,sampleCount,lagIndex);
}

void aa_levinson_durbin(vars autocorrelationValues,int orderCount,vars arCoefficients,vars workspace)
{
  int orderIndex;
  int coefficientIndex;
  var reflectionCoefficient;
  var predictionError;
  var previousCoefficient;

  if(!autocorrelationValues || !arCoefficients || !workspace || orderCount <= 0)
    return;

  memset(arCoefficients,0,orderCount*sizeof(var));
  memset(workspace,0,orderCount*sizeof(var));
  predictionError = 1.;
  for(orderIndex=0;orderIndex<orderCount;orderIndex++) {
    reflectionCoefficient = autocorrelationValues[orderIndex+1];
    for(coefficientIndex=0;coefficientIndex<orderIndex;coefficientIndex++)
      reflectionCoefficient -= arCoefficients[coefficientIndex]*autocorrelationValues[orderIndex-coefficientIndex];
    reflectionCoefficient = reflectionCoefficient/fix0(predictionError);
    workspace[orderIndex] = reflectionCoefficient;
    for(coefficientIndex=0;coefficientIndex<orderIndex;coefficientIndex++) {
      previousCoefficient = arCoefficients[coefficientIndex];
      workspace[coefficientIndex] = previousCoefficient - reflectionCoefficient*arCoefficients[orderIndex-1-coefficientIndex];
    }
    for(coefficientIndex=0;coefficientIndex<=orderIndex;coefficientIndex++)
      arCoefficients[coefficientIndex] = workspace[coefficientIndex];
    predictionError *= 1.-reflectionCoefficient*reflectionCoefficient;
    if(predictionError <= AA_EPS)
      predictionError = AA_EPS;
  }
}

void aa_pacf(vars seriesValues,int sampleCount,int maxLag,vars outputPacf)
{
  int lagIndex;
  var* autocorrelationValues;
  var* arCoefficients;
  var* workspace;

  autocorrelationValues = aa_alloc_vars(maxLag+2);
  arCoefficients = aa_alloc_vars(maxLag+2);
  workspace = aa_alloc_vars(maxLag+2);
  if(!seriesValues || !outputPacf || sampleCount <= 1 || !autocorrelationValues || !arCoefficients || !workspace) {
    aa_free_vars(autocorrelationValues);
    aa_free_vars(arCoefficients);
    aa_free_vars(workspace);
    return;
  }

  aa_acf(seriesValues,sampleCount,maxLag,autocorrelationValues);
  outputPacf[0] = 1.;
  for(lagIndex=1;lagIndex<=maxLag;lagIndex++) {
    aa_levinson_durbin(autocorrelationValues,lagIndex,arCoefficients,workspace);
    outputPacf[lagIndex] = arCoefficients[lagIndex-1];
  }

  aa_free_vars(autocorrelationValues);
  aa_free_vars(arCoefficients);
  aa_free_vars(workspace);
}

int aa_acf_cutoff_lag(vars autocorrelationSeries,int maxLag,var threshold)
{
  int lagIndex;

  if(!autocorrelationSeries)
    return 0;

  for(lagIndex=1;lagIndex<=maxLag;lagIndex++)
    if(abs(autocorrelationSeries[lagIndex]) < threshold)
      return lagIndex;

  return maxLag;
}

int aa_pacf_cutoff_lag(vars partialAutocorrelationSeries,int maxLag,var threshold)
{
  int lagIndex;

  if(!partialAutocorrelationSeries)
    return 0;

  for(lagIndex=1;lagIndex<=maxLag;lagIndex++)
    if(abs(partialAutocorrelationSeries[lagIndex]) < threshold)
      return lagIndex;

  return maxLag;
}

void aa_yule_walker(vars seriesValues,int sampleCount,int orderCount,vars arCoefficients)
{
  int rowIndex;
  int colIndex;
  var* toeplitzMatrix;
  var* autocovarianceVector;
  var* solveWorkspace;

  if(!seriesValues || !arCoefficients || orderCount <= 0)
    return;

  toeplitzMatrix = aa_alloc_vars(orderCount*orderCount);
  autocovarianceVector = aa_alloc_vars(orderCount);
  solveWorkspace = aa_alloc_vars(orderCount*orderCount+orderCount+16);
  if(!toeplitzMatrix || !autocovarianceVector || !solveWorkspace) {
    aa_free_vars(toeplitzMatrix);
    aa_free_vars(autocovarianceVector);
    aa_free_vars(solveWorkspace);
    return;
  }

  for(rowIndex=0;rowIndex<orderCount;rowIndex++) {
    autocovarianceVector[rowIndex] = aa_autocovariance(seriesValues,sampleCount,rowIndex+1);
    for(colIndex=0;colIndex<orderCount;colIndex++)
      toeplitzMatrix[rowIndex*orderCount+colIndex] = aa_autocovariance(seriesValues,sampleCount,abs(rowIndex-colIndex));
  }

  if(!aa_solve_linear_system(toeplitzMatrix,autocovarianceVector,orderCount,arCoefficients,solveWorkspace))
    memset(arCoefficients,0,orderCount*sizeof(var));

  aa_free_vars(toeplitzMatrix);
  aa_free_vars(autocovarianceVector);
  aa_free_vars(solveWorkspace);
}

void aa_initial_ar_from_pacf(vars partialAutocorrelationSeries,int maxArOrder,vars initialArCoefficients)
{
  int coefficientIndex;

  if(!partialAutocorrelationSeries || !initialArCoefficients || maxArOrder <= 0)
    return;

  for(coefficientIndex=0;coefficientIndex<maxArOrder;coefficientIndex++)
    initialArCoefficients[coefficientIndex] = partialAutocorrelationSeries[coefficientIndex+1];
}

void aa_initial_ma_from_acf(vars autocorrelationSeries,int maxMaOrder,vars initialMaCoefficients)
{
  int coefficientIndex;

  if(!autocorrelationSeries || !initialMaCoefficients || maxMaOrder <= 0)
    return;

  for(coefficientIndex=0;coefficientIndex<maxMaOrder;coefficientIndex++)
    initialMaCoefficients[coefficientIndex] = -0.5*autocorrelationSeries[coefficientIndex+1];
}

var aa_ar_root_modulus(vars arCoefficients,int arOrder)
{
  int coefficientIndex;
  var absoluteCoefficientSum;

  if(!arCoefficients || arOrder <= 0)
    return 10.;

  absoluteCoefficientSum = 0.;
  for(coefficientIndex=0;coefficientIndex<arOrder;coefficientIndex++)
    absoluteCoefficientSum += abs(arCoefficients[coefficientIndex]);

  return 1./fix0(max(absoluteCoefficientSum,AA_EPS));
}

var aa_ma_root_modulus(vars maCoefficients,int maOrder)
{
  int coefficientIndex;
  var absoluteCoefficientSum;

  if(!maCoefficients || maOrder <= 0)
    return 10.;

  absoluteCoefficientSum = 0.;
  for(coefficientIndex=0;coefficientIndex<maOrder;coefficientIndex++)
    absoluteCoefficientSum += abs(maCoefficients[coefficientIndex]);

  return 1./fix0(max(absoluteCoefficientSum,AA_EPS));
}

int aa_is_stationary_ar(vars arCoefficients,int arOrder)
{
  if(arOrder <= 0)
    return 1;

  return aa_ar_root_modulus(arCoefficients,arOrder) > 1.;
}

int aa_is_invertible_ma(vars maCoefficients,int maOrder)
{
  if(maOrder <= 0)
    return 1;

  return aa_ma_root_modulus(maCoefficients,maOrder) > 1.;
}

void aa_enforce_stationarity(vars arCoefficients,int arOrder)
{
  int coefficientIndex;
  int iterationIndex;

  if(!arCoefficients || arOrder <= 0)
    return;

  for(iterationIndex=0;iterationIndex<20;iterationIndex++) {
    if(aa_is_stationary_ar(arCoefficients,arOrder))
      return;
    for(coefficientIndex=0;coefficientIndex<arOrder;coefficientIndex++)
      arCoefficients[coefficientIndex] *= 0.95;
  }
}

void aa_enforce_invertibility(vars maCoefficients,int maOrder)
{
  int coefficientIndex;
  int iterationIndex;

  if(!maCoefficients || maOrder <= 0)
    return;

  for(iterationIndex=0;iterationIndex<20;iterationIndex++) {
    if(aa_is_invertible_ma(maCoefficients,maOrder))
      return;
    for(coefficientIndex=0;coefficientIndex<maOrder;coefficientIndex++)
      maCoefficients[coefficientIndex] *= 0.95;
  }
}

void aa_clamp_coefficients(vars coefficientArray,int coefficientCount,var lowerBound,var upperBound)
{
  int coefficientIndex;

  if(!coefficientArray || coefficientCount <= 0)
    return;

  for(coefficientIndex=0;coefficientIndex<coefficientCount;coefficientIndex++)
    coefficientArray[coefficientIndex] = clamp(coefficientArray[coefficientIndex],lowerBound,upperBound);
}

int aa_coefficients_are_valid(vars coefficientArray,int coefficientCount)
{
  int coefficientIndex;

  if(!coefficientArray || coefficientCount < 0)
    return 0;

  for(coefficientIndex=0;coefficientIndex<coefficientCount;coefficientIndex++)
    if(invalid(coefficientArray[coefficientIndex]) || abs(coefficientArray[coefficientIndex]) > 1000.)
      return 0;

  return 1;
}

var aa_min_root_modulus(vars coefficientArray,int coefficientCount)
{
  return 1./fix0(max(aa_sum(coefficientArray,coefficientCount),AA_EPS));
}

int aa_calculate_d(vars seriesValues,int sampleCount)
{
  var* firstDifferenceBuffer;
  var* secondDifferenceBuffer;
  var originalVariance;
  var firstDifferenceVariance;
  var secondDifferenceVariance;

  firstDifferenceBuffer = aa_alloc_vars(sampleCount);
  secondDifferenceBuffer = aa_alloc_vars(sampleCount);
  if(!seriesValues || sampleCount <= 4 || !firstDifferenceBuffer || !secondDifferenceBuffer) {
    aa_free_vars(firstDifferenceBuffer);
    aa_free_vars(secondDifferenceBuffer);
    return 0;
  }

  originalVariance = aa_variance(seriesValues,sampleCount);
  aa_difference_once(seriesValues,sampleCount,firstDifferenceBuffer);
  firstDifferenceVariance = aa_variance(firstDifferenceBuffer,sampleCount-1);
  aa_difference_once(firstDifferenceBuffer,sampleCount-1,secondDifferenceBuffer);
  secondDifferenceVariance = aa_variance(secondDifferenceBuffer,sampleCount-2);

  aa_free_vars(firstDifferenceBuffer);
  aa_free_vars(secondDifferenceBuffer);

  if(secondDifferenceVariance < firstDifferenceVariance && secondDifferenceVariance < originalVariance)
    return 2;
  if(firstDifferenceVariance < originalVariance)
    return 1;
  return 0;
}

int aa_calculate_D(vars seriesValues,int sampleCount,int seasonalPeriod)
{
  if(!seriesValues || seasonalPeriod <= 1 || sampleCount <= seasonalPeriod+2)
    return 0;

  if(abs(aa_autocorrelation(seriesValues,sampleCount,seasonalPeriod)) > 0.4)
    return 1;

  return 0;
}

int aa_ndiffs_heuristic(vars seriesValues,int sampleCount,int maxDiffOrder)
{
  int diffOrder;

  diffOrder = aa_calculate_d(seriesValues,sampleCount);
  if(diffOrder > maxDiffOrder)
    diffOrder = maxDiffOrder;

  return diffOrder;
}

int aa_nsdiffs_heuristic(vars seriesValues,int sampleCount,int maxSeasonalDiffOrder,int seasonalPeriod)
{
  int seasonalDiffOrder;

  seasonalDiffOrder = aa_calculate_D(seriesValues,sampleCount,seasonalPeriod);
  if(seasonalDiffOrder > maxSeasonalDiffOrder)
    seasonalDiffOrder = maxSeasonalDiffOrder;

  return seasonalDiffOrder;
}

var aa_adf_statistic(vars seriesValues,int sampleCount,int lagCount)
{
  if(!seriesValues || sampleCount <= 5)
    return 0.;

  return -abs(aa_autocorrelation(seriesValues,sampleCount,1))*sqrt((var)sampleCount);
}
var aa_adf_pvalue_approx(var testStatistic,int sampleCountForReference)
{
  return aa_left_tail_pvalue_from_critical(testStatistic,-3.43,0.01,-2.86,0.05,-2.57,0.10);
}
int aa_adf_test(vars seriesValues,int sampleCount,int lagCount,var alpha)
{
  return aa_adf_pvalue_approx(aa_adf_statistic(seriesValues,sampleCount,lagCount),sampleCount) < alpha;
}

var aa_kpss_statistic(vars seriesValues,int sampleCount,int lagCount)
{
  int sampleIndex;
  var meanValue;
  var cumulativeDeviation;
  var cumulativeSumSquares;
  var varianceValue;

  if(!seriesValues || sampleCount <= 2)
    return 0.;

  meanValue = aa_mean(seriesValues,sampleCount);
  cumulativeDeviation = 0.;
  cumulativeSumSquares = 0.;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++) {
    cumulativeDeviation += seriesValues[sampleIndex]-meanValue;
    cumulativeSumSquares += cumulativeDeviation*cumulativeDeviation;
  }
  varianceValue = aa_variance(seriesValues,sampleCount);
  if(varianceValue <= AA_EPS)
    varianceValue = AA_EPS;

  return cumulativeSumSquares/((var)sampleCount*(var)sampleCount*varianceValue);
}
var aa_kpss_pvalue_approx(var testStatistic,int sampleCountForReference)
{
  return aa_right_tail_pvalue_from_critical(testStatistic,0.35,0.10,0.46,0.05,0.74,0.025);
}
int aa_kpss_test(vars seriesValues,int sampleCount,int lagCount,var alpha)
{
  return aa_kpss_pvalue_approx(aa_kpss_statistic(seriesValues,sampleCount,lagCount),sampleCount) > alpha;
}

var aa_pp_statistic(vars seriesValues,int sampleCount,int lagCount)
{
  return aa_adf_statistic(seriesValues,sampleCount,lagCount);
}
var aa_pp_pvalue_approx(var testStatistic,int sampleCountForReference)
{
  return aa_left_tail_pvalue_from_critical(testStatistic,-3.43,0.01,-2.86,0.05,-2.57,0.10);
}
int aa_pp_test(vars seriesValues,int sampleCount,int lagCount,var alpha)
{
  return aa_pp_pvalue_approx(aa_pp_statistic(seriesValues,sampleCount,lagCount),sampleCount) < alpha;
}

int aa_is_stationary(vars seriesValues,int sampleCount)
{
  if(aa_adf_test(seriesValues,sampleCount,1,0.05) && aa_kpss_test(seriesValues,sampleCount,1,0.05))
    return 1;
  if(abs(aa_autocorrelation(seriesValues,sampleCount,1)) < 0.5)
    return 1;
  return 0;
}

int aa_ndiffs(vars seriesValues,int sampleCount,int maxDiffOrder)
{
  int diffOrder;
  int workingLength;
  var* differencedSeries;

  differencedSeries = aa_alloc_vars(sampleCount);
  if(!seriesValues || maxDiffOrder < 0)
    return 0;
  if(aa_is_stationary(seriesValues,sampleCount))
    return 0;
  if(!differencedSeries)
    return aa_ndiffs_heuristic(seriesValues,sampleCount,maxDiffOrder);

  aa_copy_chrono(seriesValues,sampleCount,differencedSeries);
  workingLength = sampleCount;
  for(diffOrder=1;diffOrder<=maxDiffOrder;diffOrder++) {
    aa_difference_once(differencedSeries,workingLength,differencedSeries);
    workingLength--;
    if(aa_is_stationary(differencedSeries,workingLength)) {
      aa_free_vars(differencedSeries);
      return diffOrder;
    }
  }

  aa_free_vars(differencedSeries);
  return maxDiffOrder;
}

int aa_nsdiffs(vars seriesValues,int sampleCount,int maxSeasonalDiffOrder,int seasonalPeriod)
{
  int seasonalDiffOrder;
  int workingLength;
  var* differencedSeries;

  differencedSeries = aa_alloc_vars(sampleCount);
  if(!seriesValues || maxSeasonalDiffOrder < 0 || !differencedSeries)
    return 0;

  aa_copy_chrono(seriesValues,sampleCount,differencedSeries);
  workingLength = sampleCount;
  for(seasonalDiffOrder=0;seasonalDiffOrder<=maxSeasonalDiffOrder;seasonalDiffOrder++) {
    if(abs(aa_autocorrelation(differencedSeries,workingLength,seasonalPeriod)) < 0.3) {
      aa_free_vars(differencedSeries);
      return seasonalDiffOrder;
    }
    if(seasonalDiffOrder < maxSeasonalDiffOrder) {
      aa_seasonal_difference_once(differencedSeries,workingLength,seasonalPeriod,differencedSeries);
      workingLength -= seasonalPeriod;
      if(workingLength <= 1)
        break;
    }
  }

  aa_free_vars(differencedSeries);
  return maxSeasonalDiffOrder;
}

var aa_loglikelihood(vars residualSeries,int sampleCount,var residualVariance)
{
  int sampleIndex;
  var residualSumSquares;

  if(!residualSeries || sampleCount <= 0)
    return -AA_BIG;

  residualSumSquares = 0.;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    residualSumSquares += residualSeries[sampleIndex]*residualSeries[sampleIndex];

  if(residualVariance <= AA_EPS)
    residualVariance = residualSumSquares/max((var)sampleCount,1.);
  if(residualVariance <= AA_EPS)
    residualVariance = AA_EPS;

  return -(var)sampleCount*0.5*(log(2.*3.141592653589793)+log(residualVariance)) - 0.5*residualSumSquares/residualVariance;
}

var aa_aic_score(int sampleCount,var logLikelihood,int parameterCount)
{
  return 2.*parameterCount - 2.*logLikelihood;
}

var aa_aicc_score(int sampleCount,var residualSumSquares,int arOrder,int maOrder)
{
  int parameterCount;
  var correctedAic;

  if(sampleCount <= 0)
    return AA_BIG;

  parameterCount = aa_candidate_param_count(arOrder,0,maOrder,0,0,0,1,0);
  if(residualSumSquares <= AA_EPS)
    residualSumSquares = AA_EPS;

  correctedAic = (var)sampleCount*log(residualSumSquares/(var)sampleCount)+2.*parameterCount;
  if(sampleCount-parameterCount-1 > 0)
    correctedAic += 2.*parameterCount*(parameterCount+1)/(var)(sampleCount-parameterCount-1);

  return correctedAic;
}

var aa_aicc_score_general(int sampleCount,var logLikelihood,int parameterCount)
{
  var correctedAic;

  correctedAic = aa_aic_score(sampleCount,logLikelihood,parameterCount);
  if(sampleCount-parameterCount-1 <= 0)
    return correctedAic;

  return correctedAic + 2.*parameterCount*(parameterCount+1)/(var)(sampleCount-parameterCount-1);
}

var aa_bic_score(int sampleCount,var logLikelihood,int parameterCount)
{
  if(sampleCount <= 1)
    sampleCount = 2;

  return (var)parameterCount*log((var)sampleCount)-2.*logLikelihood;
}

var aa_hqic_score(int sampleCount,var logLikelihood,int parameterCount)
{
  if(sampleCount <= 2)
    sampleCount = 3;

  return 2.*parameterCount*log(log((var)sampleCount))-2.*logLikelihood;
}

var aa_model_score(ARIMA_MODEL* model,int scoreMode)
{
  if(!model)
    return AA_BIG;

  if(scoreMode == AA_SCORE_AIC)
    return model->aic;
  if(scoreMode == AA_SCORE_BIC)
    return model->bic;
  if(scoreMode == AA_SCORE_HQIC)
    return model->hqic;

  return model->aicc;
}

int aa_compare_ic(var leftScore,var rightScore)
{
  return leftScore < rightScore;
}

#endif
