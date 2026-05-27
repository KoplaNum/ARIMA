#ifndef AA_ARIMA_MATH_C
#define AA_ARIMA_MATH_C

static int aa_candidate_param_count(int arOrder,int diffOrder,int maOrder,int seasonalArOrder,int seasonalDiffOrder,int seasonalMaOrder,int hasConstantTerm,int exogenousCount)
{
  int parameterCount;

  parameterCount = arOrder + maOrder + seasonalArOrder + seasonalMaOrder + diffOrder + seasonalDiffOrder + exogenousCount;
  if(hasConstantTerm)
    parameterCount++;
  if(parameterCount < 1)
    parameterCount = 1;

  return parameterCount;
}

static var aa_gaussian_loglik_from_sse(var residualSumSquares,int sampleCount)
{
  var residualVariance;

  if(sampleCount <= 0)
    return -AA_BIG;

  if(residualSumSquares <= AA_EPS)
    residualSumSquares = AA_EPS;

  residualVariance = residualSumSquares/(var)sampleCount;
  return -(var)sampleCount*0.5*(log(2.*3.141592653589793)+log(residualVariance)+1.);
}

static int aa_solve_linear_system(vars coefficientMatrix,vars rightHandSide,int systemSize,vars solutionVector,vars solveWorkspace)
{
  int rowIndex;
  int colIndex;
  int pivotIndex;
  int bestPivotRow;
  var pivotValue;
  var temporaryValue;
  var eliminationFactor;
  var* workingMatrix;
  var* workingVector;

  if(!coefficientMatrix || !rightHandSide || !solutionVector || !solveWorkspace || systemSize <= 0)
    return 0;

  workingMatrix = solveWorkspace;
  workingVector = solveWorkspace + systemSize*systemSize;
  for(rowIndex=0;rowIndex<systemSize*systemSize;rowIndex++)
    workingMatrix[rowIndex] = coefficientMatrix[rowIndex];
  for(rowIndex=0;rowIndex<systemSize;rowIndex++)
    workingVector[rowIndex] = rightHandSide[rowIndex];

  for(rowIndex=0;rowIndex<systemSize;rowIndex++) {
    bestPivotRow = rowIndex;
    pivotValue = abs(workingMatrix[rowIndex*systemSize+rowIndex]);
    for(colIndex=rowIndex+1;colIndex<systemSize;colIndex++) {
      if(abs(workingMatrix[colIndex*systemSize+rowIndex]) > pivotValue) {
        pivotValue = abs(workingMatrix[colIndex*systemSize+rowIndex]);
        bestPivotRow = colIndex;
      }
    }
    if(pivotValue <= AA_EPS)
      return 0;

    if(bestPivotRow != rowIndex) {
      for(pivotIndex=rowIndex;pivotIndex<systemSize;pivotIndex++) {
        temporaryValue = workingMatrix[rowIndex*systemSize+pivotIndex];
        workingMatrix[rowIndex*systemSize+pivotIndex] = workingMatrix[bestPivotRow*systemSize+pivotIndex];
        workingMatrix[bestPivotRow*systemSize+pivotIndex] = temporaryValue;
      }
      temporaryValue = workingVector[rowIndex];
      workingVector[rowIndex] = workingVector[bestPivotRow];
      workingVector[bestPivotRow] = temporaryValue;
    }

    pivotValue = workingMatrix[rowIndex*systemSize+rowIndex];
    for(pivotIndex=rowIndex;pivotIndex<systemSize;pivotIndex++)
      workingMatrix[rowIndex*systemSize+pivotIndex] /= pivotValue;
    workingVector[rowIndex] /= pivotValue;

    for(colIndex=0;colIndex<systemSize;colIndex++) {
      if(colIndex == rowIndex)
        continue;
      eliminationFactor = workingMatrix[colIndex*systemSize+rowIndex];
      if(abs(eliminationFactor) <= AA_EPS)
        continue;
      for(pivotIndex=rowIndex;pivotIndex<systemSize;pivotIndex++)
        workingMatrix[colIndex*systemSize+pivotIndex] -= eliminationFactor*workingMatrix[rowIndex*systemSize+pivotIndex];
      workingVector[colIndex] -= eliminationFactor*workingVector[rowIndex];
    }
  }

  for(rowIndex=0;rowIndex<systemSize;rowIndex++)
    solutionVector[rowIndex] = workingVector[rowIndex];

  return 1;
}

static int aa_ols_fit(vars responseSeries,vars designMatrix,int rowCount,int columnCount,vars coefficientVector,vars regressionWorkspace)
{
  int rowIndex;
  int columnIndex;
  int crossIndex;
  var* normalMatrix;
  var* normalVector;

  if(!responseSeries || !designMatrix || !coefficientVector || !regressionWorkspace || rowCount <= 0 || columnCount <= 0)
    return 0;

  normalMatrix = regressionWorkspace;
  normalVector = regressionWorkspace + columnCount*columnCount;
  memset(normalMatrix,0,columnCount*columnCount*sizeof(var));
  memset(normalVector,0,columnCount*sizeof(var));
  for(rowIndex=0;rowIndex<rowCount;rowIndex++) {
    for(columnIndex=0;columnIndex<columnCount;columnIndex++) {
      normalVector[columnIndex] += designMatrix[rowIndex*columnCount+columnIndex]*responseSeries[rowIndex];
      for(crossIndex=0;crossIndex<columnCount;crossIndex++)
        normalMatrix[columnIndex*columnCount+crossIndex] += designMatrix[rowIndex*columnCount+columnIndex]*designMatrix[rowIndex*columnCount+crossIndex];
    }
  }
  for(columnIndex=0;columnIndex<columnCount;columnIndex++)
    normalMatrix[columnIndex*columnCount+columnIndex] += 0.000001;

  return aa_solve_linear_system(normalMatrix,normalVector,columnCount,coefficientVector,regressionWorkspace + columnCount*columnCount + columnCount);
}

var aa_sum(vars dataSeries,int sampleCount)
{
  int sampleIndex;
  var sumValue;

  if(!dataSeries || sampleCount <= 0)
    return 0.;

  sumValue = 0.;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    sumValue += dataSeries[sampleIndex];

  return sumValue;
}

var aa_mean(vars dataSeries,int sampleCount)
{
  if(!dataSeries || sampleCount <= 0)
    return 0.;

  return aa_sum(dataSeries,sampleCount)/(var)sampleCount;
}

var aa_variance(vars dataSeries,int sampleCount)
{
  int sampleIndex;
  var meanValue;
  var varianceSum;

  if(!dataSeries || sampleCount <= 0)
    return 0.;

  meanValue = aa_mean(dataSeries,sampleCount);
  varianceSum = 0.;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    varianceSum += (dataSeries[sampleIndex]-meanValue)*(dataSeries[sampleIndex]-meanValue);

  return varianceSum/(var)sampleCount;
}

var aa_stddev(vars dataSeries,int sampleCount)
{
  return sqrt(max(aa_variance(dataSeries,sampleCount),0.));
}

var aa_covariance(vars leftSeries,vars rightSeries,int sampleCount)
{
  int sampleIndex;
  var leftMean;
  var rightMean;
  var covarianceSum;

  if(!leftSeries || !rightSeries || sampleCount <= 0)
    return 0.;

  leftMean = aa_mean(leftSeries,sampleCount);
  rightMean = aa_mean(rightSeries,sampleCount);
  covarianceSum = 0.;
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    covarianceSum += (leftSeries[sampleIndex]-leftMean)*(rightSeries[sampleIndex]-rightMean);

  return covarianceSum/(var)sampleCount;
}

var aa_correlation(vars leftSeries,vars rightSeries,int sampleCount)
{
  return aa_covariance(leftSeries,rightSeries,sampleCount)
    /fix0(aa_stddev(leftSeries,sampleCount)*aa_stddev(rightSeries,sampleCount));
}

var aa_min(vars dataSeries,int sampleCount)
{
  int sampleIndex;
  var minValue;

  if(!dataSeries || sampleCount <= 0)
    return 0.;

  minValue = dataSeries[0];
  for(sampleIndex=1;sampleIndex<sampleCount;sampleIndex++)
    if(dataSeries[sampleIndex] < minValue)
      minValue = dataSeries[sampleIndex];

  return minValue;
}

var aa_max(vars dataSeries,int sampleCount)
{
  int sampleIndex;
  var maxValue;

  if(!dataSeries || sampleCount <= 0)
    return 0.;

  maxValue = dataSeries[0];
  for(sampleIndex=1;sampleIndex<sampleCount;sampleIndex++)
    if(dataSeries[sampleIndex] > maxValue)
      maxValue = dataSeries[sampleIndex];

  return maxValue;
}

int aa_argmin(vars dataSeries,int sampleCount)
{
  int sampleIndex;
  int minIndex;

  if(!dataSeries || sampleCount <= 0)
    return -1;

  minIndex = 0;
  for(sampleIndex=1;sampleIndex<sampleCount;sampleIndex++)
    if(dataSeries[sampleIndex] < dataSeries[minIndex])
      minIndex = sampleIndex;

  return minIndex;
}

int aa_argmax(vars dataSeries,int sampleCount)
{
  int sampleIndex;
  int maxIndex;

  if(!dataSeries || sampleCount <= 0)
    return -1;

  maxIndex = 0;
  for(sampleIndex=1;sampleIndex<sampleCount;sampleIndex++)
    if(dataSeries[sampleIndex] > dataSeries[maxIndex])
      maxIndex = sampleIndex;

  return maxIndex;
}

void aa_demean(vars dataSeries,int sampleCount,vars outputSeries)
{
  int sampleIndex;
  var meanValue;

  if(!dataSeries || !outputSeries || sampleCount <= 0)
    return;

  meanValue = aa_mean(dataSeries,sampleCount);
  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    outputSeries[sampleIndex] = dataSeries[sampleIndex]-meanValue;
}

void aa_standardize(vars dataSeries,int sampleCount,vars outputSeries)
{
  int sampleIndex;
  var meanValue;
  var standardDeviation;

  if(!dataSeries || !outputSeries || sampleCount <= 0)
    return;

  meanValue = aa_mean(dataSeries,sampleCount);
  standardDeviation = aa_stddev(dataSeries,sampleCount);
  if(standardDeviation <= AA_EPS)
    standardDeviation = 1.;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    outputSeries[sampleIndex] = (dataSeries[sampleIndex]-meanValue)/standardDeviation;
}

void aa_normalize_minmax(vars dataSeries,int sampleCount,vars outputSeries)
{
  int sampleIndex;
  var minValue;
  var maxValue;
  var valueRange;

  if(!dataSeries || !outputSeries || sampleCount <= 0)
    return;

  minValue = aa_min(dataSeries,sampleCount);
  maxValue = aa_max(dataSeries,sampleCount);
  valueRange = maxValue-minValue;
  if(abs(valueRange) <= AA_EPS)
    valueRange = 1.;

  for(sampleIndex=0;sampleIndex<sampleCount;sampleIndex++)
    outputSeries[sampleIndex] = (dataSeries[sampleIndex]-minValue)/valueRange;
}

#endif
