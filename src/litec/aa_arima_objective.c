#ifndef AA_ARIMA_OBJECTIVE_C
#define AA_ARIMA_OBJECTIVE_C

static int aa_model_param_count(ARIMA_MODEL* model)
{
  int parameterCount;

  if(!model)
    return 0;

  parameterCount = model->p + model->q + model->P + model->Q + model->betaCap;
  if(model->hasConstant)
    parameterCount++;
  if(parameterCount < 1)
    parameterCount = 1;

  return parameterCount;
}

static int aa_pack_model_params(ARIMA_MODEL* model,vars packedParams)
{
  int coefficientIndex;
  int packedCount = 0;

  if(!model || !packedParams)
    return 0;

  if(model->hasConstant)
    packedParams[packedCount++] = model->constant;
  for(coefficientIndex=0;coefficientIndex<model->p;coefficientIndex++) packedParams[packedCount++] = model->ar[coefficientIndex];
  for(coefficientIndex=0;coefficientIndex<model->q;coefficientIndex++) packedParams[packedCount++] = model->ma[coefficientIndex];
  for(coefficientIndex=0;coefficientIndex<model->P;coefficientIndex++) packedParams[packedCount++] = model->sar[coefficientIndex];
  for(coefficientIndex=0;coefficientIndex<model->Q;coefficientIndex++) packedParams[packedCount++] = model->sma[coefficientIndex];
  for(coefficientIndex=0;coefficientIndex<model->betaCap;coefficientIndex++) packedParams[packedCount++] = model->beta[coefficientIndex];

  return packedCount;
}

static void aa_unpack_model_params(ARIMA_MODEL* model,vars packedParams)
{
  int coefficientIndex;
  int packedCount = 0;

  if(!model || !packedParams)
    return;

  if(model->hasConstant)
    model->constant = packedParams[packedCount++];
  for(coefficientIndex=0;coefficientIndex<model->p;coefficientIndex++) model->ar[coefficientIndex] = packedParams[packedCount++];
  for(coefficientIndex=0;coefficientIndex<model->q;coefficientIndex++) model->ma[coefficientIndex] = packedParams[packedCount++];
  for(coefficientIndex=0;coefficientIndex<model->P;coefficientIndex++) model->sar[coefficientIndex] = packedParams[packedCount++];
  for(coefficientIndex=0;coefficientIndex<model->Q;coefficientIndex++) model->sma[coefficientIndex] = packedParams[packedCount++];
  for(coefficientIndex=0;coefficientIndex<model->betaCap;coefficientIndex++) model->beta[coefficientIndex] = packedParams[packedCount++];

  aa_clamp_coefficients(model->ar,model->p,-0.99,0.99);
  aa_clamp_coefficients(model->ma,model->q,-0.99,0.99);
  aa_clamp_coefficients(model->sar,model->P,-0.99,0.99);
  aa_clamp_coefficients(model->sma,model->Q,-0.99,0.99);
  aa_enforce_stationarity(model->ar,model->p);
  aa_enforce_invertibility(model->ma,model->q);
}

static var aa_prediction_variance(ARIMA_MODEL* model,int timeIndex)
{
  int lagIndex;
  var predictionVariance;

  if(!model)
    return 1.;

  predictionVariance = max(model->sigma2,AA_EPS);
  for(lagIndex=0;lagIndex<model->q;lagIndex++)
    if(timeIndex-lagIndex-1 >= 0)
      predictionVariance += model->sigma2*model->ma[lagIndex]*model->ma[lagIndex];
  for(lagIndex=0;lagIndex<model->Q;lagIndex++)
    if(timeIndex-(lagIndex+1)*max(model->m,1) >= 0)
      predictionVariance += model->sigma2*model->sma[lagIndex]*model->sma[lagIndex];
  if(predictionVariance <= AA_EPS)
    predictionVariance = AA_EPS;

  return predictionVariance;
}

static var aa_model_objective(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work,int useExactMode)
{
  int timeIndex;
  int lagIndex;
  int seasonalLag;
  var predictedValue;
  var residualValue;
  var residualSumSquares;
  var logLikelihood;
  var innovationVariance;

  if(!model || !inputSeries || !work || sampleCount <= 0)
    return -AA_BIG;

  residualSumSquares = 0.;
  logLikelihood = 0.;
  for(timeIndex=0;timeIndex<sampleCount;timeIndex++) {
    predictedValue = model->constant;
    for(lagIndex=1;lagIndex<=model->p;lagIndex++)
      if(timeIndex-lagIndex >= 0)
        predictedValue += model->ar[lagIndex-1]*inputSeries[timeIndex-lagIndex];
    for(lagIndex=1;lagIndex<=model->P;lagIndex++) {
      seasonalLag = lagIndex*max(model->m,1);
      if(timeIndex-seasonalLag >= 0)
        predictedValue += model->sar[lagIndex-1]*inputSeries[timeIndex-seasonalLag];
    }
    for(lagIndex=1;lagIndex<=model->q;lagIndex++)
      if(timeIndex-lagIndex >= 0)
        predictedValue += model->ma[lagIndex-1]*work->residuals[timeIndex-lagIndex];
    for(lagIndex=1;lagIndex<=model->Q;lagIndex++) {
      seasonalLag = lagIndex*max(model->m,1);
      if(timeIndex-seasonalLag >= 0)
        predictedValue += model->sma[lagIndex-1]*work->residuals[timeIndex-seasonalLag];
    }

    residualValue = inputSeries[timeIndex] - predictedValue;
    work->residuals[timeIndex] = residualValue;
    residualSumSquares += residualValue*residualValue;
    if(useExactMode) {
      innovationVariance = aa_prediction_variance(model,timeIndex);
      logLikelihood += -0.5*(log(2.*3.141592653589793*innovationVariance) + residualValue*residualValue/innovationVariance);
    }
  }

  model->sse = residualSumSquares;
  model->sigma2 = max(residualSumSquares/max((var)sampleCount,1.),AA_EPS);
  model->residualCount = sampleCount;
  if(model->residuals && model->residualCap >= sampleCount)
    aa_copy_vars(model->residuals,work->residuals,sampleCount);
  if(useExactMode)
    return logLikelihood;

  return aa_loglikelihood(work->residuals,sampleCount,model->sigma2);
}

var aa_kalman_loglikelihood(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  return aa_model_objective(model,inputSeries,sampleCount,work,1);
}

var aa_conditional_loglikelihood(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  return aa_model_objective(model,inputSeries,sampleCount,work,0);
}

var aa_exact_loglikelihood(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  return aa_model_objective(model,inputSeries,sampleCount,work,1);
}

#endif
