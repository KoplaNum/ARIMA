#ifndef AA_ARIMA_SEARCH_C
#define AA_ARIMA_SEARCH_C

static int aa_update_best_model(ARIMA_MODEL* candidateModel,int scoreMode,ARIMA_MODEL* bestModel)
{
  if(!candidateModel || !bestModel)
    return 0;

  candidateModel->score = aa_model_score(candidateModel,scoreMode);
  if(candidateModel->score < bestModel->score) {
    copy_arima_model(bestModel,candidateModel);
    return 1;
  }

  return 0;
}

static int aa_eval_arima_candidate(
  vars closeSeries,int sampleCount,
  int arOrder,int diffOrder,int maOrder,
  int scoreMode,ARIMA_WORK* work,
  ARIMA_MODEL* candidateModel,ARIMA_MODEL* bestModel)
{
  if(!aa_arima_fit(arOrder,diffOrder,maOrder,closeSeries,sampleCount,work,candidateModel))
    return 0;

  return aa_update_best_model(candidateModel,scoreMode,bestModel);
}

static int aa_eval_sarima_candidate(
  vars closeSeries,int sampleCount,
  int arOrder,int diffOrder,int maOrder,
  int seasonalArOrder,int seasonalDiffOrder,int seasonalMaOrder,int seasonalPeriod,
  int scoreMode,ARIMA_WORK* work,
  ARIMA_MODEL* candidateModel,ARIMA_MODEL* bestModel)
{
  if(!aa_sarima_fit(
    arOrder,diffOrder,maOrder,
    seasonalArOrder,seasonalDiffOrder,seasonalMaOrder,seasonalPeriod,
    closeSeries,sampleCount,work,candidateModel))
    return 0;

  return aa_update_best_model(candidateModel,scoreMode,bestModel);
}

int aa_grid_search_arima(vars closeSeries,int sampleCount,int maxArOrder,int maxDiffOrder,int maxMaOrder,int scoreMode,ARIMA_WORK* work,ARIMA_MODEL* bestModel)
{
  int diffOrder;
  int arOrder;
  int maOrder;
  ARIMA_MODEL candidateModel;

  if(!closeSeries || !work || !bestModel)
    return 0;

  init_arima_model(&candidateModel);
  reset_arima_model(bestModel);
  bestModel->score = AA_BIG;

  for(diffOrder=0;diffOrder<=maxDiffOrder;diffOrder++)
    for(arOrder=0;arOrder<=maxArOrder;arOrder++)
      for(maOrder=0;maOrder<=maxMaOrder;maOrder++)
        aa_eval_arima_candidate(closeSeries,sampleCount,arOrder,diffOrder,maOrder,scoreMode,work,&candidateModel,bestModel);

  free_arima_model(&candidateModel);
  return bestModel->converged;
}

int aa_auto_arima_search(vars closeSeries,int sampleCount,int maxArOrder,int maxDiffOrder,int maxMaOrder,int scoreMode,ARIMA_WORK* work,ARIMA_MODEL* bestModel)
{
  return aa_grid_search_arima(closeSeries,sampleCount,maxArOrder,maxDiffOrder,maxMaOrder,scoreMode,work,bestModel);
}

int aa_stepwise_auto_arima(vars closeSeries,int sampleCount,int maxArOrder,int maxDiffOrder,int maxMaOrder,int scoreMode,ARIMA_WORK* work,ARIMA_MODEL* bestModel)
{
  return aa_grid_search_arima(closeSeries,sampleCount,maxArOrder,maxDiffOrder,maxMaOrder,scoreMode,work,bestModel);
}

void aa_init_candidate(ARIMA_CANDIDATE* candidate)
{
  if(!candidate)
    return;

  memset(candidate,0,sizeof(ARIMA_CANDIDATE));
  candidate->score = AA_BIG;
}

void aa_set_candidate(ARIMA_CANDIDATE* candidate,int arOrder,int diffOrder,int maOrder,int seasonalArOrder,int seasonalDiffOrder,int seasonalMaOrder,int seasonalPeriod)
{
  if(!candidate)
    return;

  candidate->p = arOrder;
  candidate->d = diffOrder;
  candidate->q = maOrder;
  candidate->P = seasonalArOrder;
  candidate->D = seasonalDiffOrder;
  candidate->Q = seasonalMaOrder;
  candidate->m = seasonalPeriod;
  candidate->score = AA_BIG;
  candidate->valid = 1;
}

int aa_validate_candidate_model(ARIMA_CANDIDATE* candidate)
{
  if(!candidate)
    return 0;

  if(candidate->p < 0 || candidate->d < 0 || candidate->q < 0)
    return 0;
  if(candidate->P < 0 || candidate->D < 0 || candidate->Q < 0)
    return 0;

  return 1;
}

int aa_candidate_exists(ARIMA_CANDIDATE* candidateList,int candidateCount,ARIMA_CANDIDATE* candidate)
{
  int candidateIndex;

  if(!candidateList || !candidate)
    return 0;

  for(candidateIndex=0;candidateIndex<candidateCount;candidateIndex++) {
    if(candidateList[candidateIndex].p == candidate->p
    && candidateList[candidateIndex].d == candidate->d
    && candidateList[candidateIndex].q == candidate->q
    && candidateList[candidateIndex].P == candidate->P
    && candidateList[candidateIndex].D == candidate->D
    && candidateList[candidateIndex].Q == candidate->Q
    && candidateList[candidateIndex].m == candidate->m)
      return 1;
  }

  return 0;
}

void aa_expand_candidate_models(ARIMA_CANDIDATE* currentCandidate,ARIMA_CANDIDATE* candidateList,int* candidateCount)
{
  ARIMA_CANDIDATE newCandidate;

  if(!currentCandidate || !candidateList || !candidateCount)
    return;

  aa_set_candidate(&newCandidate,currentCandidate->p+1,currentCandidate->d,currentCandidate->q,currentCandidate->P,currentCandidate->D,currentCandidate->Q,currentCandidate->m);
  if(!aa_candidate_exists(candidateList,*candidateCount,&newCandidate))
    candidateList[(*candidateCount)++] = newCandidate;

  aa_set_candidate(&newCandidate,max(currentCandidate->p-1,0),currentCandidate->d,currentCandidate->q,currentCandidate->P,currentCandidate->D,currentCandidate->Q,currentCandidate->m);
  if(!aa_candidate_exists(candidateList,*candidateCount,&newCandidate))
    candidateList[(*candidateCount)++] = newCandidate;

  aa_set_candidate(&newCandidate,currentCandidate->p,currentCandidate->d,currentCandidate->q+1,currentCandidate->P,currentCandidate->D,currentCandidate->Q,currentCandidate->m);
  if(!aa_candidate_exists(candidateList,*candidateCount,&newCandidate))
    candidateList[(*candidateCount)++] = newCandidate;
}

void aa_try_neighbor_models(ARIMA_CANDIDATE* currentCandidate,ARIMA_CANDIDATE* candidateList,int* candidateCount)
{
  aa_expand_candidate_models(currentCandidate,candidateList,candidateCount);
}

int aa_select_best_model(ARIMA_CANDIDATE* candidateList,int candidateCount)
{
  int candidateIndex;
  int bestIndex;

  if(!candidateList || candidateCount <= 0)
    return -1;

  bestIndex = 0;
  for(candidateIndex=1;candidateIndex<candidateCount;candidateIndex++)
    if(candidateList[candidateIndex].score < candidateList[bestIndex].score)
      bestIndex = candidateIndex;

  return bestIndex;
}

int aa_fallback_model(vars closeSeries,int sampleCount,ARIMA_MODEL* model)
{
  if(!closeSeries || !model || sampleCount <= 0)
    return 0;

  init_arima_model(model);
  model->forecast = closeSeries[0];
  model->score = 0.;
  model->converged = 1;
  return 1;
}

int aa_detect_seasonal_period(vars seriesValues,int sampleCount,int minSeasonalPeriod,int maxSeasonalPeriod)
{
  int seasonalPeriod;
  int bestSeasonalPeriod;
  var bestScore;
  var currentScore;

  if(!seriesValues || sampleCount <= 4)
    return 0;

  bestSeasonalPeriod = minSeasonalPeriod;
  bestScore = 0.;
  for(seasonalPeriod=minSeasonalPeriod;seasonalPeriod<=maxSeasonalPeriod;seasonalPeriod++) {
    currentScore = abs(aa_autocorrelation(seriesValues,sampleCount,seasonalPeriod));
    if(currentScore > bestScore) {
      bestScore = currentScore;
      bestSeasonalPeriod = seasonalPeriod;
    }
  }

  return bestSeasonalPeriod;
}

var aa_seasonal_acf_score(vars seriesValues,int sampleCount,int seasonalPeriod)
{
  return abs(aa_autocorrelation(seriesValues,sampleCount,seasonalPeriod));
}

var aa_seasonal_strength(vars seriesValues,int sampleCount,int seasonalPeriod)
{
  int sampleIndex;
  int residualCount;
  var residualVariance;
  var totalVariance;
  var seasonalReference;

  if(!seriesValues || sampleCount <= seasonalPeriod || seasonalPeriod <= 1)
    return 0.;

  totalVariance = aa_variance(seriesValues,sampleCount);
  residualVariance = 0.;
  residualCount = 0;
  for(sampleIndex=seasonalPeriod;sampleIndex<sampleCount;sampleIndex++) {
    seasonalReference = seriesValues[sampleIndex-seasonalPeriod];
    residualVariance += aa_square(seriesValues[sampleIndex]-seasonalReference);
    residualCount++;
  }

  if(residualCount <= 0 || totalVariance <= AA_EPS)
    return 0.;

  residualVariance /= (var)residualCount;
  return max(0.,1.-residualVariance/totalVariance);
}

int aa_auto_sarima_search(
  vars closeSeries,int sampleCount,
  int maxArOrder,int maxDiffOrder,int maxMaOrder,
  int maxSeasonalArOrder,int maxSeasonalDiffOrder,int maxSeasonalMaOrder,
  int seasonalPeriod,int scoreMode,
  ARIMA_WORK* work,ARIMA_MODEL* bestModel)
{
  int diffOrder;
  int seasonalDiffOrder;
  int arOrder;
  int maOrder;
  int seasonalArOrder;
  int seasonalMaOrder;
  ARIMA_MODEL candidateModel;

  if(!closeSeries || !work || !bestModel)
    return 0;

  init_arima_model(&candidateModel);
  bestModel->score = AA_BIG;

  for(diffOrder=0;diffOrder<=maxDiffOrder;diffOrder++)
    for(seasonalDiffOrder=0;seasonalDiffOrder<=maxSeasonalDiffOrder;seasonalDiffOrder++)
      for(arOrder=0;arOrder<=maxArOrder;arOrder++)
        for(maOrder=0;maOrder<=maxMaOrder;maOrder++)
          for(seasonalArOrder=0;seasonalArOrder<=maxSeasonalArOrder;seasonalArOrder++)
            for(seasonalMaOrder=0;seasonalMaOrder<=maxSeasonalMaOrder;seasonalMaOrder++)
              aa_eval_sarima_candidate(
                closeSeries,sampleCount,
                arOrder,diffOrder,maOrder,
                seasonalArOrder,seasonalDiffOrder,seasonalMaOrder,seasonalPeriod,
                scoreMode,work,&candidateModel,bestModel);

  free_arima_model(&candidateModel);
  return bestModel->converged;
}

int aa_stepwise_auto_sarima(
  vars closeSeries,int sampleCount,
  int maxArOrder,int maxDiffOrder,int maxMaOrder,
  int maxSeasonalArOrder,int maxSeasonalDiffOrder,int maxSeasonalMaOrder,
  int seasonalPeriod,int scoreMode,
  ARIMA_WORK* work,ARIMA_MODEL* bestModel)
{
  return aa_auto_sarima_search(
    closeSeries,sampleCount,
    maxArOrder,maxDiffOrder,maxMaOrder,
    maxSeasonalArOrder,maxSeasonalDiffOrder,maxSeasonalMaOrder,
    seasonalPeriod,scoreMode,work,bestModel);
}

#endif
