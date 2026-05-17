#ifndef AA_ARIMA_PLATFORM_C
#define AA_ARIMA_PLATFORM_C

static void aa_print_coeff_block(string coefficientLabel,vars coefficientArray,int coefficientCount)
{
  int coefficientIndex;

  if(!coefficientArray)
    return;

  for(coefficientIndex=0;coefficientIndex<coefficientCount;coefficientIndex++)
    printf("%s[%i]=%.6f\n",coefficientLabel,coefficientIndex+1,coefficientArray[coefficientIndex]);
}

static int aa_refit_same_orders(ARIMA_MODEL* model,vars closeSeries,int sampleCount,ARIMA_WORK* work)
{
  if(!model)
    return 0;

  if(model->P || model->D || model->Q)
    return aa_sarima_fit(model->p,model->d,model->q,model->P,model->D,model->Q,model->m,closeSeries,sampleCount,work,model);

  return aa_arima_fit(model->p,model->d,model->q,closeSeries,sampleCount,work,model);
}

void aa_print_model_summary(ARIMA_MODEL* model)
{
  if(model)
    printf("ARIMA(%i,%i,%i)(%i,%i,%i)_%i sse=%.6f aicc=%.6f forecast=%.6f\n",
      model->p,model->d,model->q,model->P,model->D,model->Q,model->m,
      model->sse,model->aicc,model->forecast);
}

void aa_print_coefficients(ARIMA_MODEL* model)
{
  if(!model)
    return;
  aa_print_coeff_block("AR",model->ar,model->p);
  aa_print_coeff_block("MA",model->ma,model->q);
  aa_print_coeff_block("SAR",model->sar,model->P);
  aa_print_coeff_block("SMA",model->sma,model->Q);
}

void aa_print_residual_diagnostics(ARIMA_MODEL* model,vars residualSeries,int sampleCount)
{
  if(model && residualSeries)
    printf("Residual mean=%.6f var=%.6f JB=%.6f LB=%.6f\n",
      aa_residual_mean(residualSeries,sampleCount),
      aa_residual_variance(residualSeries,sampleCount),
      aa_jarque_bera_stat(residualSeries,sampleCount),
      aa_ljung_box_stat(residualSeries,sampleCount,min(10,sampleCount-1)));
}

void aa_print_forecast_report(ARIMA_MODEL* model,vars forecastPath,int forecastHorizon)
{
  int horizonIndex;

  if(!forecastPath)
    return;

  for(horizonIndex=0;horizonIndex<forecastHorizon;horizonIndex++)
    printf("h=%i forecast=%.6f\n",horizonIndex+1,forecastPath[horizonIndex]);
}

int aa_save_model_to_file(ARIMA_MODEL* model,string fileName)
{
  return 0;
}

int aa_load_model_from_file(ARIMA_MODEL* model,string fileName,ARIMA_WORK* work)
{
  return 0;
}

int aa_save_forecast_to_file(vars forecastPath,int forecastHorizon,string fileName)
{
  return 0;
}

int aa_save_diagnostics_to_file(ARIMA_MODEL* model,string fileName)
{
  return 0;
}

void aa_init_model_cache()
{
  free_arima_model(&g_aa_cache_model);
  init_arima_model(&g_aa_cache_model);
  g_aa_cache_ready = 0;
}

void aa_clear_model_cache()
{
  aa_init_model_cache();
}

int aa_cache_best_model(string assetName,int barPeriodValue,ARIMA_MODEL* model)
{
  if(!model)
    return 0;
  copy_arima_model(&g_aa_cache_model,model);
  g_aa_cache_asset = assetName;
  g_aa_cache_barperiod = barPeriodValue;
  g_aa_cache_ready = 1;
  return 1;
}

int aa_load_cached_model(string assetName,int barPeriodValue,ARIMA_MODEL* model,ARIMA_WORK* work)
{
  if(!g_aa_cache_ready || !model)
    return 0;
  if(g_aa_cache_barperiod != barPeriodValue)
    return 0;
  copy_arima_model(model,&g_aa_cache_model);
  return 1;
}

int aa_reuse_previous_model(vars closeSeries,int sampleCount,ARIMA_MODEL* model,ARIMA_WORK* work)
{
  if(!model || !work)
    return 0;

  model->forecast = aa_forecast_one_step(model,closeSeries,sampleCount,work);
  return 1;
}

int aa_refit_best_model(vars closeSeries,int sampleCount,ARIMA_MODEL* model,ARIMA_WORK* work)
{
  return aa_refit_same_orders(model,closeSeries,sampleCount,work);
}

int aa_should_refit_model(int barsSinceLastFit,int refitInterval,var lastError,var errorLimit)
{
  if(barsSinceLastFit >= refitInterval)
    return 1;

  if(abs(lastError) > errorLimit)
    return 1;

  return 0;
}

int aa_zorro_get_close_series(vars closeSeries,int sampleCount)
{
  int barIndex;

  if(!closeSeries || sampleCount <= 0)
    return 0;

  for(barIndex=0;barIndex<sampleCount;barIndex++)
    closeSeries[barIndex] = priceClose(barIndex);

  return 1;
}

int aa_zorro_forecast_current_asset(int sampleCount,int maxArOrder,int maxMaOrder,var tickSize,ARIMA_WORK* work,ARIMA_MODEL* model)
{
  vars closeSeries = series(priceClose(0));

  if(!closeSeries)
    return 0;

  if(!aa_grid_search_arima(closeSeries,sampleCount,maxArOrder,2,maxMaOrder,AA_SCORE_AICC,work,model))
    return 0;

  model->forecast = aa_round_to_tick_size(aa_forecast_one_step(model,closeSeries,sampleCount,work),tickSize);
  return 1;
}

void aa_zorro_plot_forecast(ARIMA_MODEL* model)
{
  if(model)
    plot("AA.Forecast",model->forecast,LINE,BLUE);
}

void aa_zorro_plot_forecast_bands(vars forecastPath,vars lowerBand,vars upperBand,int forecastHorizon)
{
  if(!forecastPath || !lowerBand || !upperBand || forecastHorizon <= 0)
    return;

  plot("AA.F",forecastPath[0],LINE,BLUE);
  plot("AA.L",lowerBand[0],LINE,RED);
  plot("AA.U",upperBand[0],LINE,GREEN);
}

void aa_zorro_print_model(ARIMA_MODEL* model)
{
  aa_print_model_summary(model);
}

int aa_zorro_trade_from_forecast(ARIMA_MODEL* model,var threshold)
{
  int signal;

  if(!model)
    return 0;

  signal = aa_signal_from_forecast(priceClose(0),model->forecast,threshold);
  if(signal > 0) {
    enterLong();
    return 1;
  }
  if(signal < 0) {
    enterShort();
    return -1;
  }

  return 0;
}

void init_auto_arima_result(AUTO_ARIMA_RESULT* result)
{
  if(!result)
    return;

  memset(result,0,sizeof(AUTO_ARIMA_RESULT));
  result->p = -1;
  result->q = -1;
  result->aicc = AA_BIG;
  result->sse = AA_BIG;
}

void free_auto_arima_result(AUTO_ARIMA_RESULT* result)
{
  if(!result)
    return;

  aa_free_vars(result->ar);
  aa_free_vars(result->ma);
  init_auto_arima_result(result);
}

var aa_round_to_tick_size(var price,var tickSize)
{
  if(tickSize <= 0.)
    return price;

  return round(price/tickSize)*tickSize;
}

void init_auto_arima_work(AUTO_ARIMA_WORK* work)
{
  if(work)
    init_arima_work(&work->core);
}

void free_auto_arima_work(AUTO_ARIMA_WORK* work)
{
  if(work)
    free_arima_work(&work->core);
}

int aa_prepare_auto_arima_work(AUTO_ARIMA_WORK* work,int sampleCount,int maxArOrder,int maxMaOrder)
{
  if(!work)
    return 0;

  return aa_prepare_arima_work(&work->core,sampleCount,maxArOrder,maxMaOrder,0,0,8);
}

int auto_arima_forecast_with_work(vars closeSeries,int sampleCount,var tickSize,int maxArOrder,int maxMaOrder,AUTO_ARIMA_WORK* work,AUTO_ARIMA_RESULT* result)
{
  ARIMA_MODEL model;

  if(!closeSeries || !work || !result)
    return 0;

  init_arima_model(&model);
  if(!aa_grid_search_arima(closeSeries,sampleCount,maxArOrder,2,maxMaOrder,AA_SCORE_AICC,&work->core,&model)) {
    free_arima_model(&model);
    return 0;
  }

  if(result->arCap < model.p) {
    aa_free_vars(result->ar);
    result->ar = aa_alloc_vars(model.p);
    result->arCap = model.p;
  }
  if(result->maCap < model.q) {
    aa_free_vars(result->ma);
    result->ma = aa_alloc_vars(model.q);
    result->maCap = model.q;
  }

  result->p = model.p;
  result->d = model.d;
  result->q = model.q;
  result->converged = model.converged;
  result->sse = model.sse;
  result->aicc = model.aicc;
  result->forecast = aa_round_to_tick_size(aa_forecast_one_step(&model,closeSeries,sampleCount,&work->core),tickSize);

  if(result->ar && model.ar && model.p > 0)
    aa_copy_vars(result->ar,model.ar,model.p);
  if(result->ma && model.ma && model.q > 0)
    aa_copy_vars(result->ma,model.ma,model.q);

  free_arima_model(&model);
  return 1;
}

int auto_arima_forecast(vars closeSeries,int sampleCount,var tickSize,int maxArOrder,int maxMaOrder,AUTO_ARIMA_RESULT* result)
{
  AUTO_ARIMA_WORK work;

  init_auto_arima_work(&work);
  if(!aa_prepare_auto_arima_work(&work,sampleCount,maxArOrder,maxMaOrder))
    return 0;
  if(!auto_arima_forecast_with_work(closeSeries,sampleCount,tickSize,maxArOrder,maxMaOrder,&work,result)) {
    free_auto_arima_work(&work);
    return 0;
  }
  free_auto_arima_work(&work);
  return 1;
}

#endif
