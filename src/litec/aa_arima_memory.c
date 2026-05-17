#ifndef AA_ARIMA_MEMORY_C
#define AA_ARIMA_MEMORY_C

static var g_aa_optimizer_tolerance = 0.000001;
static var g_aa_optimizer_learning_rate = 0.01;
static int g_aa_optimizer_max_iter = 50;

static ARIMA_MODEL g_aa_cache_model;
static int g_aa_cache_ready = 0;
static string g_aa_cache_asset = 0;
static int g_aa_cache_barperiod = 0;

static int aa_prepare_model_array(var** targetPointer,int* capacityPointer,int requiredCount)
{
  var* allocatedPointer;

  if(requiredCount <= 0)
    return 1;
  if(*targetPointer && *capacityPointer >= requiredCount)
    return 1;

  if(*targetPointer)
    free(*targetPointer);

  allocatedPointer = (var*)malloc(requiredCount*sizeof(var));
  if(!allocatedPointer) {
    *targetPointer = 0;
    *capacityPointer = 0;
    return 0;
  }

  memset(allocatedPointer,0,requiredCount*sizeof(var));
  *targetPointer = allocatedPointer;
  *capacityPointer = requiredCount;
  return 1;
}

static int aa_prepare_all_model_arrays(ARIMA_MODEL* model,int arOrder,int maOrder,int seasonalArOrder,int seasonalMaOrder,int residualCount,int forecastHorizon,int betaCount)
{
  if(!model)
    return 0;

  return aa_prepare_model_array(&model->ar,&model->arCap,arOrder)
    && aa_prepare_model_array(&model->ma,&model->maCap,maOrder)
    && aa_prepare_model_array(&model->sar,&model->sarCap,seasonalArOrder)
    && aa_prepare_model_array(&model->sma,&model->smaCap,seasonalMaOrder)
    && aa_prepare_model_array(&model->residuals,&model->residualCap,residualCount)
    && aa_prepare_model_array(&model->forecastPath,&model->forecastCap,forecastHorizon)
    && aa_prepare_model_array(&model->beta,&model->betaCap,betaCount);
}

var* aa_alloc_vars(int elementCount)
{
  var* allocatedPointer;

  if(elementCount <= 0)
    return 0;

  allocatedPointer = (var*)malloc(elementCount*sizeof(var));
  if(!allocatedPointer)
    return 0;

  memset(allocatedPointer,0,elementCount*sizeof(var));
  return allocatedPointer;
}

void aa_free_vars(var* allocatedPointer)
{
  if(allocatedPointer)
    free(allocatedPointer);
}

void aa_zero_vars(vars dataArray,int elementCount)
{
  if(dataArray && elementCount > 0)
    memset(dataArray,0,elementCount*sizeof(var));
}

void aa_copy_vars(vars destinationArray,vars sourceArray,int elementCount)
{
  if(destinationArray && sourceArray && elementCount > 0)
    memcpy(destinationArray,sourceArray,elementCount*sizeof(var));
}

void aa_shift_vars(vars dataArray,var newValue,int elementCount)
{
  int elementIndex;

  if(!dataArray || elementCount <= 0)
    return;

  for(elementIndex=elementCount-1;elementIndex>0;elementIndex--)
    dataArray[elementIndex] = dataArray[elementIndex-1];

  dataArray[0] = newValue;
}

void init_arima_model(ARIMA_MODEL* model)
{
  if(!model)
    return;

  memset(model,0,sizeof(ARIMA_MODEL));
  model->aic = AA_BIG;
  model->aicc = AA_BIG;
  model->bic = AA_BIG;
  model->hqic = AA_BIG;
  model->score = AA_BIG;
}

void reset_arima_model(ARIMA_MODEL* model)
{
  if(!model)
    return;
  model->converged = 0;
  model->iterations = 0;
  model->status = 0;
  model->sigma2 = 0.;
  model->sse = 0.;
  model->logLik = 0.;
  model->forecast = 0.;
  model->forecastSE = 0.;
  model->aic = AA_BIG;
  model->aicc = AA_BIG;
  model->bic = AA_BIG;
  model->hqic = AA_BIG;
  model->score = AA_BIG;
  if(model->ar) memset(model->ar,0,model->arCap*sizeof(var));
  if(model->ma) memset(model->ma,0,model->maCap*sizeof(var));
  if(model->sar) memset(model->sar,0,model->sarCap*sizeof(var));
  if(model->sma) memset(model->sma,0,model->smaCap*sizeof(var));
  if(model->beta) memset(model->beta,0,model->betaCap*sizeof(var));
}

void free_arima_model(ARIMA_MODEL* model)
{
  if(!model)
    return;
  aa_free_vars(model->ar);
  aa_free_vars(model->ma);
  aa_free_vars(model->sar);
  aa_free_vars(model->sma);
  aa_free_vars(model->beta);
  aa_free_vars(model->residuals);
  aa_free_vars(model->forecastPath);
  init_arima_model(model);
}

void copy_arima_model(ARIMA_MODEL* destinationModel,ARIMA_MODEL* sourceModel)
{
  if(!destinationModel || !sourceModel)
    return;
  if(!aa_prepare_all_model_arrays(destinationModel,sourceModel->p,sourceModel->q,sourceModel->P,sourceModel->Q,sourceModel->residualCount,sourceModel->forecastCount,sourceModel->betaCap))
    return;
  destinationModel->p = sourceModel->p; destinationModel->d = sourceModel->d; destinationModel->q = sourceModel->q;
  destinationModel->P = sourceModel->P; destinationModel->D = sourceModel->D; destinationModel->Q = sourceModel->Q; destinationModel->m = sourceModel->m;
  destinationModel->hasConstant = sourceModel->hasConstant; destinationModel->transformMode = sourceModel->transformMode;
  destinationModel->residualCount = sourceModel->residualCount; destinationModel->forecastCount = sourceModel->forecastCount;
  destinationModel->paramCount = sourceModel->paramCount; destinationModel->converged = sourceModel->converged;
  destinationModel->iterations = sourceModel->iterations; destinationModel->status = sourceModel->status;
  destinationModel->mean = sourceModel->mean; destinationModel->constant = sourceModel->constant; destinationModel->sigma2 = sourceModel->sigma2;
  destinationModel->sse = sourceModel->sse; destinationModel->logLik = sourceModel->logLik; destinationModel->aic = sourceModel->aic;
  destinationModel->aicc = sourceModel->aicc; destinationModel->bic = sourceModel->bic; destinationModel->hqic = sourceModel->hqic;
  destinationModel->score = sourceModel->score; destinationModel->forecast = sourceModel->forecast; destinationModel->forecastSE = sourceModel->forecastSE;
  destinationModel->lastPrice = sourceModel->lastPrice; destinationModel->lambda = sourceModel->lambda; destinationModel->gradNorm = sourceModel->gradNorm;
  if(sourceModel->ar && destinationModel->ar) aa_copy_vars(destinationModel->ar,sourceModel->ar,sourceModel->p);
  if(sourceModel->ma && destinationModel->ma) aa_copy_vars(destinationModel->ma,sourceModel->ma,sourceModel->q);
  if(sourceModel->sar && destinationModel->sar) aa_copy_vars(destinationModel->sar,sourceModel->sar,sourceModel->P);
  if(sourceModel->sma && destinationModel->sma) aa_copy_vars(destinationModel->sma,sourceModel->sma,sourceModel->Q);
  if(sourceModel->beta && destinationModel->beta) aa_copy_vars(destinationModel->beta,sourceModel->beta,sourceModel->betaCap);
  if(sourceModel->residuals && destinationModel->residuals) aa_copy_vars(destinationModel->residuals,sourceModel->residuals,sourceModel->residualCount);
  if(sourceModel->forecastPath && destinationModel->forecastPath) aa_copy_vars(destinationModel->forecastPath,sourceModel->forecastPath,sourceModel->forecastCount);
}

void init_arima_work(ARIMA_WORK* work)
{
  if(work)
    memset(work,0,sizeof(ARIMA_WORK));
}

void free_arima_work(ARIMA_WORK* work)
{
  if(!work)
    return;
  if(work->rawBuffer)
    free(work->rawBuffer);
  init_arima_work(work);
}

void aa_clear_arima_work(ARIMA_WORK* work)
{
  if(work && work->rawBuffer && work->rawBufferCount > 0)
    memset(work->rawBuffer,0,work->rawBufferCount*sizeof(var));
}

int aa_prepare_arima_work(ARIMA_WORK* work,int sampleCount,int maxArOrder,int maxMaOrder,int maxSeasonalArOrder,int maxSeasonalMaOrder,int forecastHorizon)
{
  int seriesCapacity;
  int vectorCapacity;
  int rawBufferCount;

  if(!work)
    return 0;

  seriesCapacity = max(sampleCount,1);
  vectorCapacity = max(max(maxArOrder,maxMaOrder),max(maxSeasonalArOrder,maxSeasonalMaOrder)) + forecastHorizon + 32;
  rawBufferCount = seriesCapacity*18 + vectorCapacity*20 + (vectorCapacity*vectorCapacity)*3 + 256;
  if(work->rawBuffer && work->rawBufferCount >= rawBufferCount) {
    work->capacityN = max(work->capacityN,sampleCount);
    work->capacityP = max(work->capacityP,maxArOrder);
    work->capacityQ = max(work->capacityQ,maxMaOrder);
    work->capacitySP = max(work->capacitySP,maxSeasonalArOrder);
    work->capacitySQ = max(work->capacitySQ,maxSeasonalMaOrder);
    work->capacityH = max(work->capacityH,forecastHorizon);
    return 1;
  }

  free_arima_work(work);
  work->rawBuffer = (var*)malloc(rawBufferCount*sizeof(var));
  if(!work->rawBuffer)
    return 0;
  memset(work->rawBuffer,0,rawBufferCount*sizeof(var));
  work->rawBufferCount = rawBufferCount;
  work->capacityN = sampleCount;
  work->capacityP = maxArOrder;
  work->capacityQ = maxMaOrder;
  work->capacitySP = maxSeasonalArOrder;
  work->capacitySQ = maxSeasonalMaOrder;
  work->capacityH = forecastHorizon;
  work->tmp1 = work->rawBuffer;
  work->tmp2 = work->tmp1 + seriesCapacity;
  work->tmp3 = work->tmp2 + seriesCapacity;
  work->tmp4 = work->tmp3 + seriesCapacity;
  work->tmp5 = work->tmp4 + seriesCapacity;
  work->tmp6 = work->tmp5 + seriesCapacity;
  work->tmp7 = work->tmp6 + seriesCapacity;
  work->tmp8 = work->tmp7 + seriesCapacity;
  work->acf = work->tmp8 + seriesCapacity;
  work->pacf = work->acf + seriesCapacity;
  work->residuals = work->pacf + seriesCapacity;
  work->forecast = work->residuals + seriesCapacity;
  work->lower = work->forecast + seriesCapacity;
  work->upper = work->lower + seriesCapacity;
  work->grad = work->upper + seriesCapacity;
  work->hessian = work->grad + vectorCapacity*vectorCapacity;
  work->matrix = work->hessian + vectorCapacity*vectorCapacity;
  work->vector = work->matrix + vectorCapacity*vectorCapacity;
  work->params = work->vector + vectorCapacity;
  work->params2 = work->params + vectorCapacity;
  work->workspace = work->params2 + vectorCapacity;
  return 1;
}

#endif
