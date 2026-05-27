#ifndef AA_ARIMA_DIAGNOSTICS_C
#define AA_ARIMA_DIAGNOSTICS_C

static var aa_portmanteau_stat(vars residualSeries,int sampleCount,int maxLag,int useLjungBox)
{
  int lagIndex;
  var statisticSum;
  var autocorrelationValue;

  if(!residualSeries || sampleCount <= 0)
    return 0.;

  statisticSum = 0.;
  for(lagIndex=1;lagIndex<=maxLag;lagIndex++) {
    autocorrelationValue = aa_autocorrelation(residualSeries,sampleCount,lagIndex);
    if(useLjungBox)
      statisticSum += autocorrelationValue*autocorrelationValue/max((var)(sampleCount-lagIndex),1.);
    else
      statisticSum += autocorrelationValue*autocorrelationValue;
  }

  if(useLjungBox)
    return (var)sampleCount*((var)sampleCount+2.)*statisticSum;

  return (var)sampleCount*statisticSum;
}

static var aa_bucket_pvalue(var statisticValue,var thresholdA,var thresholdB,var pValueA,var pValueB,var pValueC)
{
  if(statisticValue < thresholdA)
    return pValueA;
  if(statisticValue < thresholdB)
    return pValueB;
  return pValueC;
}

static var aa_norm_cdf(var zScore)
{
  var tValue;
  var polynomialValue;
  var pdfValue;
  var cdfValue;

  tValue = 1. / (1. + 0.2316419 * abs(zScore));
  polynomialValue = 0.319381530 * tValue;
  polynomialValue -= 0.356563782 * tValue * tValue;
  polynomialValue += 1.781477937 * tValue * tValue * tValue;
  polynomialValue -= 1.821255978 * tValue * tValue * tValue * tValue;
  polynomialValue += 1.330274429 * tValue * tValue * tValue * tValue * tValue;
  pdfValue = exp(-0.5 * zScore * zScore) / sqrt(2. * 3.141592653589793);
  cdfValue = 1. - pdfValue * polynomialValue;
  if(zScore < 0.)
    cdfValue = 1. - cdfValue;
  return clamp(cdfValue,0.,1.);
}

static var aa_chi_square_survival(var chiSquareStatistic,int degreesOfFreedom)
{
  var transformedStatistic;
  var transformedMean;
  var transformedStdDev;
  var normalScore;
  var cumulativeProbability;

  if(degreesOfFreedom <= 0)
    return 1.;
  if(chiSquareStatistic <= 0.)
    return 1.;

  transformedStatistic = pow(chiSquareStatistic / (var)degreesOfFreedom,1. / 3.);
  transformedMean = 1. - 2. / (9. * (var)degreesOfFreedom);
  transformedStdDev = sqrt(2. / (9. * (var)degreesOfFreedom));
  normalScore = (transformedStatistic - transformedMean) / transformedStdDev;
  cumulativeProbability = aa_norm_cdf(normalScore);
  return clamp(1. - cumulativeProbability,0.,1.);
}

void aa_compute_residuals(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work,vars residualOutput)
{
  if(model && inputSeries && residualOutput && work)
    aa_compute_arma_residuals_internal(inputSeries,sampleCount,model->p,model->q,model->ar,model->ma,model->constant,residualOutput);
}

var aa_residual_mean(vars residualSeries,int sampleCount)
{
  return aa_mean(residualSeries,sampleCount);
}

var aa_residual_variance(vars residualSeries,int sampleCount)
{
  return aa_variance(residualSeries,sampleCount);
}

var aa_residual_stddev(vars residualSeries,int sampleCount)
{
  return aa_stddev(residualSeries,sampleCount);
}

void aa_residual_acf(vars residualSeries,int sampleCount,int maxLag,vars outputAcf)
{
  aa_acf(residualSeries,sampleCount,maxLag,outputAcf);
}

var aa_ljung_box_stat(vars residualSeries,int sampleCount,int maxLag)
{
  return aa_portmanteau_stat(residualSeries,sampleCount,maxLag,1);
}

var aa_ljung_box_pvalue(var testStatistic,int degreesOfFreedom)
{
  return aa_chi_square_survival(testStatistic,degreesOfFreedom);
}

int aa_ljung_box_test(vars residualSeries,int sampleCount,int maxLag,var alpha)
{
  return aa_ljung_box_pvalue(aa_ljung_box_stat(residualSeries,sampleCount,maxLag),maxLag) > alpha;
}

var aa_box_pierce_stat(vars residualSeries,int sampleCount,int maxLag)
{
  return aa_portmanteau_stat(residualSeries,sampleCount,maxLag,0);
}

var aa_box_pierce_pvalue(var testStatistic,int degreesOfFreedom)
{
  return aa_chi_square_survival(testStatistic,degreesOfFreedom);
}

int aa_box_pierce_test(vars residualSeries,int sampleCount,int maxLag,var alpha)
{
  return aa_box_pierce_pvalue(aa_box_pierce_stat(residualSeries,sampleCount,maxLag),maxLag) > alpha;
}

var aa_durbin_watson_test(vars residualSeries,int sampleCount)
{
  int sampleIndex;
  var numerator = 0.;
  var denominator = 0.;

  if(!residualSeries || sampleCount <= 1)
    return 0.;

  for(sampleIndex=1;sampleIndex<sampleCount;sampleIndex++)
    numerator += (residualSeries[sampleIndex]-residualSeries[sampleIndex-1])*(residualSeries[sampleIndex]-residualSeries[sampleIndex-1]);
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    denominator += residualSeries[sampleIndex]*residualSeries[sampleIndex];

  return numerator/fix0(denominator);
}

var aa_jarque_bera_stat(vars residualSeries,int sampleCount)
{
  int sampleIndex;
  var residualMean;
  var residualStdDev;
  var skewnessAccumulator;
  var kurtosisAccumulator;
  var standardizedResidual;

  if(!residualSeries || sampleCount <= 2)
    return 0.;

  residualMean = aa_mean(residualSeries,sampleCount);
  residualStdDev = aa_stddev(residualSeries,sampleCount);
  if(residualStdDev <= AA_EPS)
    residualStdDev = 1.;

  skewnessAccumulator = 0.;
  kurtosisAccumulator = 0.;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++) {
    standardizedResidual = (residualSeries[sampleIndex]-residualMean)/residualStdDev;
    skewnessAccumulator += standardizedResidual*standardizedResidual*standardizedResidual;
    kurtosisAccumulator += standardizedResidual*standardizedResidual*standardizedResidual*standardizedResidual;
  }

  skewnessAccumulator /= (var)sampleCount;
  kurtosisAccumulator /= (var)sampleCount;
  return (var)sampleCount*(skewnessAccumulator*skewnessAccumulator + (kurtosisAccumulator-3.)*(kurtosisAccumulator-3.)*0.25)/6.;
}

var aa_jarque_bera_pvalue(var testStatistic)
{
  return aa_chi_square_survival(testStatistic,2);
}
int aa_jarque_bera_test(vars residualSeries,int sampleCount,var alpha)
{
  return aa_jarque_bera_pvalue(aa_jarque_bera_stat(residualSeries,sampleCount)) > alpha;
}

var aa_arch_lm_stat(vars residualSeries,int sampleCount,int lagCount)
{
  int rowIndex;
  int columnIndex;
  int regressionRows;
  int regressionCols;
  var responseMean;
  var totalSumSquares;
  var residualSumSquares;
  var fittedValue;
  var* responseSeries;
  var* designMatrix;
  var* regressionCoefficients;
  var* regressionWork;

  if(!residualSeries || sampleCount <= lagCount+2 || lagCount <= 0)
    return 0.;

  regressionRows = sampleCount-lagCount;
  regressionCols = lagCount+1;
  responseSeries = aa_alloc_vars(regressionRows);
  designMatrix = aa_alloc_vars(regressionRows*regressionCols);
  regressionCoefficients = aa_alloc_vars(regressionCols);
  regressionWork = aa_alloc_vars(regressionCols*regressionCols*3+regressionCols*4+16);
  if(!responseSeries || !designMatrix || !regressionCoefficients || !regressionWork) {
    aa_free_vars(responseSeries);
    aa_free_vars(designMatrix);
    aa_free_vars(regressionCoefficients);
    aa_free_vars(regressionWork);
    return 0.;
  }

  for(rowIndex=0;rowIndex<regressionRows;rowIndex++) {
    responseSeries[rowIndex] = residualSeries[rowIndex+lagCount]*residualSeries[rowIndex+lagCount];
    designMatrix[rowIndex*regressionCols] = 1.;
    for(columnIndex=1;columnIndex<regressionCols;columnIndex++)
      designMatrix[rowIndex*regressionCols+columnIndex] = residualSeries[rowIndex+lagCount-columnIndex]*residualSeries[rowIndex+lagCount-columnIndex];
  }

  if(!aa_ols_fit(responseSeries,designMatrix,regressionRows,regressionCols,regressionCoefficients,regressionWork)) {
    aa_free_vars(responseSeries);
    aa_free_vars(designMatrix);
    aa_free_vars(regressionCoefficients);
    aa_free_vars(regressionWork);
    return 0.;
  }

  responseMean = aa_mean(responseSeries,regressionRows);
  totalSumSquares = 0.;
  residualSumSquares = 0.;
  for(rowIndex=0;rowIndex<regressionRows;rowIndex++) {
    fittedValue = 0.;
    for(columnIndex=0;columnIndex<regressionCols;columnIndex++)
      fittedValue += designMatrix[rowIndex*regressionCols+columnIndex]*regressionCoefficients[columnIndex];
    totalSumSquares += (responseSeries[rowIndex]-responseMean)*(responseSeries[rowIndex]-responseMean);
    residualSumSquares += (responseSeries[rowIndex]-fittedValue)*(responseSeries[rowIndex]-fittedValue);
  }

  aa_free_vars(responseSeries);
  aa_free_vars(designMatrix);
  aa_free_vars(regressionCoefficients);
  aa_free_vars(regressionWork);

  if(totalSumSquares <= AA_EPS)
    return 0.;

  return (var)regressionRows*(1.-residualSumSquares/totalSumSquares);
}

var aa_arch_lm_pvalue(var testStatistic,int degreesOfFreedom)
{
  return aa_chi_square_survival(testStatistic,degreesOfFreedom);
}

int aa_arch_lm_test(vars residualSeries,int sampleCount,int lagCount,var alpha)
{
  return aa_arch_lm_pvalue(aa_arch_lm_stat(residualSeries,sampleCount,lagCount),lagCount) > alpha;
}

int aa_residual_normality_check(vars residualSeries,int sampleCount)
{
  return aa_jarque_bera_test(residualSeries,sampleCount,0.05);
}

int aa_residual_white_noise_check(vars residualSeries,int sampleCount,int maxLag)
{
  return aa_ljung_box_test(residualSeries,sampleCount,maxLag,0.05);
}

void aa_diagnostic_report(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  if(!model || !inputSeries || !work)
    return;

  aa_compute_residuals(model,inputSeries,sampleCount,work,work->residuals);
  printf("ARIMA diag: mean=%.6f var=%.6f DW=%.6f LB=%.6f\n",
    aa_residual_mean(work->residuals,sampleCount),
    aa_residual_variance(work->residuals,sampleCount),
    aa_durbin_watson_test(work->residuals,sampleCount),
    aa_ljung_box_stat(work->residuals,sampleCount,min(10,sampleCount-1)));
}

#endif
