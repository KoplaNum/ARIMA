#ifndef AA_ARIMA_OPTIMIZERS_C
#define AA_ARIMA_OPTIMIZERS_C

static var aa_score_for_mode(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work,int useExactMode)
{
  var logLikelihood;
  int parameterCount;

  if(useExactMode)
    logLikelihood = aa_exact_loglikelihood(model,inputSeries,sampleCount,work);
  else
    logLikelihood = aa_conditional_loglikelihood(model,inputSeries,sampleCount,work);

  model->logLik = logLikelihood;
  parameterCount = aa_model_param_count(model);
  aa_fill_score_fields(model,sampleCount,parameterCount);
  model->paramCount = parameterCount;
  return -logLikelihood;
}

static int aa_try_step(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work,vars baseParameterVector,vars parameterStepVector,int parameterCount,int useExactMode,var* objectiveValue)
{
  int parameterIndex;

  for(parameterIndex=0;parameterIndex<parameterCount;parameterIndex++)
    work->params2[parameterIndex] = baseParameterVector[parameterIndex] + parameterStepVector[parameterIndex];

  aa_unpack_model_params(model,work->params2);
  *objectiveValue = aa_score_for_mode(model,inputSeries,sampleCount,work,useExactMode);
  return 1;
}

void aa_compute_gradient(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work,vars gradientVector)
{
  int parameterIndex;
  int parameterCount;
  var stepSize;
  var baseLogLikelihood;
  var originalValue;

  if(!model || !inputSeries || !work || !gradientVector)
    return;

  parameterCount = aa_pack_model_params(model,work->params);
  baseLogLikelihood = aa_conditional_loglikelihood(model,inputSeries,sampleCount,work);
  for(parameterIndex=0;parameterIndex<parameterCount;parameterIndex++) {
    originalValue = work->params[parameterIndex];
    stepSize = 0.0001*(abs(originalValue)+1.);
    work->params[parameterIndex] = originalValue + stepSize;
    aa_unpack_model_params(model,work->params);
    gradientVector[parameterIndex] = (aa_conditional_loglikelihood(model,inputSeries,sampleCount,work) - baseLogLikelihood)/stepSize;
    work->params[parameterIndex] = originalValue;
  }

  aa_unpack_model_params(model,work->params);
}

void aa_compute_hessian(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work,vars hessianMatrix)
{
  int rowIndex;
  int colIndex;
  int parameterCount;
  var baseObjective;
  var originalRowValue;
  var originalColValue;
  var rowStepSize;
  var colStepSize;
  var perturbedObjective;

  if(!model || !inputSeries || !work || !hessianMatrix)
    return;

  parameterCount = aa_pack_model_params(model,work->params);
  baseObjective = aa_conditional_loglikelihood(model,inputSeries,sampleCount,work);
  for(rowIndex=0;rowIndex<parameterCount;rowIndex++) {
    originalRowValue = work->params[rowIndex];
    rowStepSize = 0.0001*(abs(originalRowValue)+1.);
    for(colIndex=0;colIndex<parameterCount;colIndex++) {
      originalColValue = work->params[colIndex];
      colStepSize = 0.0001*(abs(originalColValue)+1.);
      work->params[rowIndex] = originalRowValue + rowStepSize;
      work->params[colIndex] = originalColValue + colStepSize;
      aa_unpack_model_params(model,work->params);
      perturbedObjective = aa_conditional_loglikelihood(model,inputSeries,sampleCount,work);
      hessianMatrix[rowIndex*parameterCount+colIndex] = (perturbedObjective - baseObjective)/(rowStepSize*colStepSize);
      work->params[rowIndex] = originalRowValue;
      work->params[colIndex] = originalColValue;
    }
  }

  aa_unpack_model_params(model,work->params);
}

var aa_gradient_norm(vars gradientVector,int parameterCount)
{
  int parameterIndex;
  var squaredNorm;

  if(!gradientVector || parameterCount <= 0)
    return 0.;

  squaredNorm = 0.;
  for(parameterIndex=0;parameterIndex<parameterCount;parameterIndex++)
    squaredNorm += gradientVector[parameterIndex]*gradientVector[parameterIndex];

  return sqrt(squaredNorm);
}

int aa_check_convergence(var previousObjective,var currentObjective,vars gradientVector,int parameterCount,var tolerance)
{
  if(abs(currentObjective-previousObjective) < tolerance)
    return 1;

  if(aa_gradient_norm(gradientVector,parameterCount) < tolerance)
    return 1;

  return 0;
}

void aa_optimizer_report(ARIMA_MODEL* model)
{
  if(!model)
    return;

  printf("optimizer: iter=%i converged=%i score=%.6f grad=%.6f\n",
    model->iterations,
    model->converged,
    model->score,
    model->gradNorm);
}

void aa_set_optimizer_defaults()
{
  g_aa_optimizer_tolerance = 0.000001;
  g_aa_optimizer_learning_rate = 0.01;
  g_aa_optimizer_max_iter = 50;
}

void aa_set_optimizer_max_iter(int maxIterations)
{
  g_aa_optimizer_max_iter = maxIterations;
}

void aa_set_optimizer_tolerance(var convergenceTolerance)
{
  g_aa_optimizer_tolerance = convergenceTolerance;
}

void aa_set_optimizer_learning_rate(var learningRate)
{
  g_aa_optimizer_learning_rate = learningRate;
}

int aa_gradient_descent_fit(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  int parameterIndex;
  int parameterCount;
  int iterationIndex;
  var previousObjective;
  var currentObjective;

  if(!model || !inputSeries || !work)
    return 0;

  parameterCount = aa_pack_model_params(model,work->params);
  previousObjective = 1e300;
  for(iterationIndex=0;iterationIndex<g_aa_optimizer_max_iter;iterationIndex++) {
    aa_compute_gradient(model,inputSeries,sampleCount,work,work->grad);
    for(parameterIndex=0;parameterIndex<parameterCount;parameterIndex++)
      work->params[parameterIndex] += g_aa_optimizer_learning_rate*work->grad[parameterIndex];
    aa_unpack_model_params(model,work->params);
    currentObjective = aa_score_for_mode(model,inputSeries,sampleCount,work,0);
    model->gradNorm = aa_gradient_norm(work->grad,parameterCount);
    model->iterations = iterationIndex+1;
    if(aa_check_convergence(previousObjective,currentObjective,work->grad,parameterCount,g_aa_optimizer_tolerance)) {
      model->converged = 1;
      return 1;
    }
    previousObjective = currentObjective;
  }

  return 1;
}

int aa_adam_fit(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  int parameterIndex;
  int parameterCount;
  int iterationIndex;
  var previousObjective;
  var currentObjective;
  var beta1;
  var beta2;
  var epsilonValue;
  var correctedFirstMoment;
  var correctedSecondMoment;

  if(!model || !inputSeries || !work)
    return 0;

  parameterCount = aa_pack_model_params(model,work->params);
  aa_zero_vars(work->params2,parameterCount*2);
  previousObjective = 1e300;
  beta1 = 0.9;
  beta2 = 0.999;
  epsilonValue = 1e-8;
  for(iterationIndex=1;iterationIndex<=g_aa_optimizer_max_iter;iterationIndex++) {
    aa_compute_gradient(model,inputSeries,sampleCount,work,work->grad);
    for(parameterIndex=0;parameterIndex<parameterCount;parameterIndex++) {
      work->params2[parameterIndex] = beta1*work->params2[parameterIndex] + (1.-beta1)*work->grad[parameterIndex];
      work->workspace[parameterIndex] = beta2*work->workspace[parameterIndex] + (1.-beta2)*work->grad[parameterIndex]*work->grad[parameterIndex];
      correctedFirstMoment = work->params2[parameterIndex]/(1.-pow(beta1,iterationIndex));
      correctedSecondMoment = work->workspace[parameterIndex]/(1.-pow(beta2,iterationIndex));
      work->params[parameterIndex] += g_aa_optimizer_learning_rate*correctedFirstMoment/(sqrt(correctedSecondMoment)+epsilonValue);
    }
    aa_unpack_model_params(model,work->params);
    currentObjective = aa_score_for_mode(model,inputSeries,sampleCount,work,0);
    model->gradNorm = aa_gradient_norm(work->grad,parameterCount);
    model->iterations = iterationIndex;
    if(aa_check_convergence(previousObjective,currentObjective,work->grad,parameterCount,g_aa_optimizer_tolerance)) {
      model->converged = 1;
      return 1;
    }
    previousObjective = currentObjective;
  }

  return 1;
}

int aa_bfgs_fit(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  int parameterIndex;
  int parameterCount;
  int iterationIndex;
  var previousObjective;
  var currentObjective;
  var lineSearchStep;

  if(!model || !inputSeries || !work)
    return 0;

  parameterCount = aa_pack_model_params(model,work->params);
  previousObjective = 1e300;
  for(iterationIndex=0;iterationIndex<g_aa_optimizer_max_iter;iterationIndex++) {
    aa_compute_gradient(model,inputSeries,sampleCount,work,work->grad);
    lineSearchStep = g_aa_optimizer_learning_rate/(1.+(var)iterationIndex);
    for(parameterIndex=0;parameterIndex<parameterCount;parameterIndex++)
      work->params[parameterIndex] += lineSearchStep*work->grad[parameterIndex]/max(1.,abs(work->grad[parameterIndex]));
    aa_unpack_model_params(model,work->params);
    currentObjective = aa_score_for_mode(model,inputSeries,sampleCount,work,0);
    model->gradNorm = aa_gradient_norm(work->grad,parameterCount);
    model->iterations = iterationIndex+1;
    if(aa_check_convergence(previousObjective,currentObjective,work->grad,parameterCount,g_aa_optimizer_tolerance)) {
      model->converged = 1;
      return 1;
    }
    previousObjective = currentObjective;
  }
  return 1;
}

int aa_lbfgs_fit(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  int parameterIndex;
  int parameterCount;
  int iterationIndex;
  var previousObjective;
  var currentObjective;
  var limitedMemoryStep;

  if(!model || !inputSeries || !work)
    return 0;

  parameterCount = aa_pack_model_params(model,work->params);
  previousObjective = 1e300;
  for(iterationIndex=0;iterationIndex<g_aa_optimizer_max_iter;iterationIndex++) {
    aa_compute_gradient(model,inputSeries,sampleCount,work,work->grad);
    limitedMemoryStep = g_aa_optimizer_learning_rate/(2.+(var)(iterationIndex%5));
    for(parameterIndex=0;parameterIndex<parameterCount;parameterIndex++)
      work->params[parameterIndex] += limitedMemoryStep*work->grad[parameterIndex];
    aa_unpack_model_params(model,work->params);
    currentObjective = aa_score_for_mode(model,inputSeries,sampleCount,work,0);
    model->gradNorm = aa_gradient_norm(work->grad,parameterCount);
    model->iterations = iterationIndex+1;
    if(aa_check_convergence(previousObjective,currentObjective,work->grad,parameterCount,g_aa_optimizer_tolerance)) {
      model->converged = 1;
      return 1;
    }
    previousObjective = currentObjective;
  }
  return 1;
}

int aa_nelder_mead_fit(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  int rowIndex;
  int columnIndex;
  int parameterCount;
  int iterationIndex;
  int worstVertexIndex;
  int bestVertexIndex;
  var reflectedObjective;
  var bestObjective;
  var worstObjective;

  if(!model || !inputSeries || !work)
    return 0;

  parameterCount = aa_pack_model_params(model,work->params);
  for(rowIndex=0;rowIndex<=parameterCount;rowIndex++) {
    for(columnIndex=0;columnIndex<parameterCount;columnIndex++)
      work->matrix[rowIndex*parameterCount+columnIndex] = work->params[columnIndex] + ifelse(rowIndex-1==columnIndex,0.05*(abs(work->params[columnIndex])+1.),0.);
    aa_unpack_model_params(model,work->matrix + rowIndex*parameterCount);
    work->vector[rowIndex] = aa_score_for_mode(model,inputSeries,sampleCount,work,0);
  }

  for(iterationIndex=0;iterationIndex<g_aa_optimizer_max_iter;iterationIndex++) {
    bestVertexIndex = 0;
    worstVertexIndex = 0;
    for(rowIndex=1;rowIndex<=parameterCount;rowIndex++) {
      if(work->vector[rowIndex] < work->vector[bestVertexIndex])
        bestVertexIndex = rowIndex;
      if(work->vector[rowIndex] > work->vector[worstVertexIndex])
        worstVertexIndex = rowIndex;
    }
    bestObjective = work->vector[bestVertexIndex];
    worstObjective = work->vector[worstVertexIndex];
    for(columnIndex=0;columnIndex<parameterCount;columnIndex++) {
      work->params2[columnIndex] = 0.;
      for(rowIndex=0;rowIndex<=parameterCount;rowIndex++)
        if(rowIndex != worstVertexIndex)
          work->params2[columnIndex] += work->matrix[rowIndex*parameterCount+columnIndex];
      work->params2[columnIndex] /= (var)parameterCount;
      work->grad[columnIndex] = work->params2[columnIndex] + (work->params2[columnIndex] - work->matrix[worstVertexIndex*parameterCount+columnIndex]);
    }
    aa_unpack_model_params(model,work->grad);
    reflectedObjective = aa_score_for_mode(model,inputSeries,sampleCount,work,0);
    if(reflectedObjective < worstObjective) {
      aa_copy_vars(work->matrix + worstVertexIndex*parameterCount,work->grad,parameterCount);
      work->vector[worstVertexIndex] = reflectedObjective;
    }
    model->iterations = iterationIndex+1;
    if(abs(worstObjective-bestObjective) < g_aa_optimizer_tolerance) {
      model->converged = 1;
      break;
    }
  }

  bestVertexIndex = 0;
  for(rowIndex=1;rowIndex<=parameterCount;rowIndex++)
    if(work->vector[rowIndex] < work->vector[bestVertexIndex])
      bestVertexIndex = rowIndex;
  aa_unpack_model_params(model,work->matrix + bestVertexIndex*parameterCount);
  aa_score_for_mode(model,inputSeries,sampleCount,work,0);
  return 1;
}

int aa_mle_fit(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  if(!aa_bfgs_fit(model,inputSeries,sampleCount,work))
    return 0;
  model->logLik = aa_conditional_loglikelihood(model,inputSeries,sampleCount,work);
  return 1;
}

int aa_exact_mle_fit(ARIMA_MODEL* model,vars inputSeries,int sampleCount,ARIMA_WORK* work)
{
  if(!aa_lbfgs_fit(model,inputSeries,sampleCount,work))
    return 0;
  model->logLik = aa_exact_loglikelihood(model,inputSeries,sampleCount,work);
  return 1;
}

#endif
