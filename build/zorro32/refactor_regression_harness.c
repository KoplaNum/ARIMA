#include <math.h>
#include <stdio.h>
#include <windows.h>

typedef double var;
typedef double* vars;

#define AA_TRANSFORM_LOG 1

void aa_log_transform(vars inputSeries,int sampleCount,vars outputSeries);
void aa_boxcox_transform(vars inputSeries,int sampleCount,var lambdaValue,vars outputSeries);
void aa_return_transform(vars closeSeries,int sampleCount,vars returnSeries);
var aa_stddev(vars dataSeries,int sampleCount);
var aa_correlation(vars leftSeries,vars rightSeries,int sampleCount);
var aa_durbin_watson_test(vars residualSeries,int sampleCount);
var aa_jarque_bera_stat(vars residualSeries,int sampleCount);
var aa_forecast_error_mae(vars actualSeries,vars forecastSeries,int sampleCount);
var aa_forecast_error_mse(vars actualSeries,vars forecastSeries,int sampleCount);
var aa_forecast_error_rmse(vars actualSeries,vars forecastSeries,int sampleCount);
var aa_directional_accuracy(vars actualSeries,vars forecastSeries,int sampleCount);
var aa_forecast_return(var lastObservedValue,var pointForecast);
var aa_forecast_zscore(var pointForecast,var forecastStandardError,var lastObservedValue);
var aa_position_size_from_forecast(var forecastEdge,var riskBudget,var stopDistance);
var aa_autocorrelation(vars seriesValues,int sampleCount,int lagOrder);
void aa_levinson_durbin(vars autocorrelationValues,int orderCount,vars arCoefficients,vars workspace);
var aa_ar_root_modulus(vars arCoefficients,int arOrder);
var aa_ma_root_modulus(vars maCoefficients,int maOrder);
var aa_min_root_modulus(vars coefficientArray,int coefficientCount);
var aa_seasonal_strength(vars seriesValues,int sampleCount,int seasonalPeriod);
void aa_bias_adjusted_backtransform(vars transformedForecastPath,vars forecastStandardErrors,int forecastHorizon,int transformMode,var boxCoxLambda,vars biasAdjustedForecastPath);

static void write_header(FILE* handle)
{
  fprintf(handle,"function_name,test_id,output_key,output_index,output_kind,value,status,message\n");
}

static void write_scalar(FILE* handle,const char* function_name,const char* test_id,const char* output_key,double value)
{
  fprintf(handle,"%s,%s,%s,0,scalar,%.10f,OK,\n",function_name,test_id,output_key,value);
}

static void write_int(FILE* handle,const char* function_name,const char* test_id,const char* output_key,int value)
{
  fprintf(handle,"%s,%s,%s,0,int,%d,OK,\n",function_name,test_id,output_key,value);
}

static void write_vector(FILE* handle,const char* function_name,const char* test_id,const char* output_key,const double* values,int count)
{
  int i;
  for(i=0;i<count;i++)
    fprintf(handle,"%s,%s,%s,%d,vector,%.10f,OK,\n",function_name,test_id,output_key,i,values[i]);
}

static int export_missing(HMODULE module,const char* symbol)
{
  return GetProcAddress(module,symbol) == NULL;
}

int main(void)
{
  const char* out_path = "D:\\IOA44LIO\\AutoAri\\validation_outputs\\builtin_refactor_live_outputs.csv";
  FILE* out = fopen(out_path,"w");
  HMODULE module;

  double fix_noisy[8] = {10.,11.,9.,12.,8.,13.,9.,10.};
  double fix_price[8] = {100.,102.,101.,105.,107.,106.,108.,110.};
  double x[5] = {1.,2.,3.,4.,5.};
  double y[5] = {2.,4.,3.,7.,6.};
  double forecast_actual[3] = {110.,112.,115.};
  double forecast_pred[3] = {111.,113.,114.};
  double actual_dir[5] = {100.,102.,101.,105.,107.};
  double forecast_dir[5] = {101.,100.,103.,106.,108.};
  double ar_coeff[3] = {0.8,0.5,0.3};
  double ma_coeff[3] = {0.8,0.5,0.3};
  double min_root_coeff[3] = {0.2,-0.1,0.05};
  double log_input[3] = {100.,102.,101.};
  double log_output[3] = {0.,0.,0.};
  double boxcox_input[3] = {100.,102.,101.};
  double boxcox_log_output[3] = {0.,0.,0.};
  double boxcox_sqrt_output[3] = {0.,0.,0.};
  double return_output[7] = {0.,0.,0.,0.,0.,0.,0.};
  double acf_values[4] = {0.,0.,0.,0.};
  double ld_output[3] = {0.,0.,0.};
  double ld_workspace[3] = {0.,0.,0.};
  double transformed_forecast[3] = {4.605170186,4.700480366,4.787491743};
  double forecast_se[3] = {0.1,0.2,0.3};
  double bias_adj_output[3] = {0.,0.,0.};
  int i;

  if(!out)
    return 1;

  write_header(out);

  module = LoadLibraryA("D:\\IOA44LIO\\AutoAri\\build\\zorro32\\AutoAri32.dll");
  if(!module) {
    fclose(out);
    return 2;
  }

  write_int(out,"export_check","removed_001","aa_abs_missing",export_missing(module,"aa_abs"));
  write_int(out,"export_check","removed_002","aa_max_int_missing",export_missing(module,"aa_max_int"));
  write_int(out,"export_check","removed_003","aa_min_int_missing",export_missing(module,"aa_min_int"));
  write_int(out,"export_check","removed_004","aa_safe_div_missing",export_missing(module,"aa_safe_div"));
  write_int(out,"export_check","removed_005","aa_safe_log_missing",export_missing(module,"aa_safe_log"));
  write_int(out,"export_check","removed_006","aa_safe_sqrt_missing",export_missing(module,"aa_safe_sqrt"));
  write_int(out,"export_check","removed_007","aa_square_missing",export_missing(module,"aa_square"));
  write_int(out,"export_check","removed_008","aa_round_to_tick_size_missing",export_missing(module,"aa_round_to_tick_size"));

  aa_log_transform(log_input,3,log_output);
  write_vector(out,"aa_log_transform","refactor_001","log_output",log_output,3);

  aa_boxcox_transform(boxcox_input,3,0.,boxcox_log_output);
  write_vector(out,"aa_boxcox_transform","refactor_002","boxcox_log",boxcox_log_output,3);

  aa_boxcox_transform(boxcox_input,3,0.5,boxcox_sqrt_output);
  write_vector(out,"aa_boxcox_transform","refactor_003","boxcox_half",boxcox_sqrt_output,3);

  aa_return_transform(fix_price,8,return_output);
  write_vector(out,"aa_return_transform","refactor_004","returns",return_output,7);

  write_scalar(out,"aa_stddev","refactor_005","stddev",aa_stddev(fix_noisy,8));
  write_scalar(out,"aa_correlation","refactor_006","correlation",aa_correlation(x,y,5));
  write_scalar(out,"aa_durbin_watson_test","refactor_007","dw",aa_durbin_watson_test(fix_noisy,8));
  write_scalar(out,"aa_jarque_bera_stat","refactor_008","jb",aa_jarque_bera_stat(fix_noisy,8));
  write_scalar(out,"aa_forecast_error_mae","refactor_009","mae",aa_forecast_error_mae(forecast_actual,forecast_pred,3));
  write_scalar(out,"aa_forecast_error_mse","refactor_010","mse",aa_forecast_error_mse(forecast_actual,forecast_pred,3));
  write_scalar(out,"aa_forecast_error_rmse","refactor_011","rmse",aa_forecast_error_rmse(forecast_actual,forecast_pred,3));
  write_scalar(out,"aa_directional_accuracy","refactor_012","accuracy",aa_directional_accuracy(actual_dir,forecast_dir,5));
  write_scalar(out,"aa_forecast_return","refactor_013","return",aa_forecast_return(100.,103.));
  write_scalar(out,"aa_forecast_zscore","refactor_014","zscore",aa_forecast_zscore(103.,2.,100.));
  write_scalar(out,"aa_position_size_from_forecast","refactor_015","position_size",aa_position_size_from_forecast(0.02,1000.,0.05));
  write_scalar(out,"aa_autocorrelation","refactor_016","lag1",aa_autocorrelation(fix_noisy,8,1));

  acf_values[0] = 1.;
  for(i=1;i<=3;i++)
    acf_values[i] = aa_autocorrelation(fix_price,8,i);
  aa_levinson_durbin(acf_values,3,ld_output,ld_workspace);
  write_vector(out,"aa_levinson_durbin","refactor_017","ar_coeff",ld_output,3);

  write_scalar(out,"aa_ar_root_modulus","refactor_018","root_modulus",aa_ar_root_modulus(ar_coeff,3));
  write_scalar(out,"aa_ma_root_modulus","refactor_019","root_modulus",aa_ma_root_modulus(ma_coeff,3));
  write_scalar(out,"aa_min_root_modulus","refactor_020","root_modulus",aa_min_root_modulus(min_root_coeff,3));
  write_scalar(out,"aa_seasonal_strength","refactor_021","strength",aa_seasonal_strength(fix_price,8,2));

  aa_bias_adjusted_backtransform(transformed_forecast,forecast_se,3,AA_TRANSFORM_LOG,0.,bias_adj_output);
  write_vector(out,"aa_bias_adjusted_backtransform","refactor_022","bias_adjusted",bias_adj_output,3);

  FreeLibrary(module);
  fclose(out);
  return 0;
}
