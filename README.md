# ARIMA / SARIMA lite-C Function Reference

Professional GitHub reference for the documented ARIMA / SARIMA lite-C API. This document is intended to be readable on its own before the full repository is published.

## Coverage

- Documented, source-backed functions covered here: **227**
- Supporting documented data structures included in the appendix: **3**
- Source location base: `src/litec/`
- Formula source: `docs/ARIMA_SARIMA_liteC_Function_Formulas_Optimized.md`

## Reading Guide

- **Signature** reproduces the lite-C declaration documented for the function.
- **Mathematical form** gives the traditional mathematical expression or operator form.
- **Parameters** links the code arguments to their mathematical roles and meanings.
- **Implementation note** preserves the optimized-construction guidance from the formulas catalog.
- **Defined in** points to the current source file and the first detected definition line.

## Common Notation

| Symbol | Meaning |
|---|---|
| $y_t$ | Observed series value at time $t$ |
| $\hat y_{T+h}$ | Forecast at horizon $h$ from forecast origin $T$ |
| $\varepsilon_t$ | Residual or one-step-ahead forecast error |
| $N$ | Number of observations |
| $(p,d,q)$ | Non-seasonal ARIMA orders |
| $(P,D,Q,m)$ | Seasonal ARIMA orders and seasonal period |
| $\alpha$ | Confidence significance level |
| $\lambda$ | Box-Cox parameter |
| $X_t$ | Exogenous regressor vector or matrix row |
| $\beta$ | Regression coefficient vector |
| $M$ | ARIMA/SARIMA model structure |
| $W$ | Reusable work-buffer structure |

## Module Index

### 1. Memory and Pointer Management Functions (13)

- [aa_alloc_vars](#aaallocvars)
- [aa_clear_arima_work](#aacleararimawork)
- [aa_copy_vars](#aacopyvars)
- [aa_free_vars](#aafreevars)
- [aa_prepare_arima_work](#aapreparearimawork)
- [aa_shift_vars](#aashiftvars)
- [aa_zero_vars](#aazerovars)
- [copy_arima_model](#copyarimamodel)
- [free_arima_model](#freearimamodel)
- [free_arima_work](#freearimawork)
- [init_arima_model](#initarimamodel)
- [init_arima_work](#initarimawork)
- [reset_arima_model](#resetarimamodel)

### 2. Basic Math and Array Utilities (20)

- [aa_abs](#aaabs)
- [aa_argmax](#aaargmax)
- [aa_argmin](#aaargmin)
- [aa_correlation](#aacorrelation)
- [aa_covariance](#aacovariance)
- [aa_demean](#aademean)
- [aa_max](#aamax)
- [aa_max_int](#aamaxint)
- [aa_mean](#aamean)
- [aa_min](#aamin)
- [aa_min_int](#aaminint)
- [aa_normalize_minmax](#aanormalizeminmax)
- [aa_safe_div](#aasafediv)
- [aa_safe_log](#aasafelog)
- [aa_safe_sqrt](#aasafesqrt)
- [aa_square](#aasquare)
- [aa_standardize](#aastandardize)
- [aa_stddev](#aastddev)
- [aa_sum](#aasum)
- [aa_variance](#aavariance)

### 3. Input Validation and Data Cleaning (11)

- [aa_clip_outliers](#aaclipoutliers)
- [aa_count_invalid_values](#aacountinvalidvalues)
- [aa_fill_missing_forward](#aafillmissingforward)
- [aa_fill_missing_mean](#aafillmissingmean)
- [aa_has_invalid_values](#aahasinvalidvalues)
- [aa_limit_returns](#aalimitreturns)
- [aa_remove_invalid_values](#aaremoveinvalidvalues)
- [aa_replace_zero_prices](#aareplacezeroprices)
- [aa_validate_price_series](#aavalidatepriceseries)
- [aa_validate_series](#aavalidateseries)
- [aa_winsorize_series](#aawinsorizeseries)

### 4. Transformations and Differencing (15)

- [aa_boxcox_lambda](#aaboxcoxlambda)
- [aa_boxcox_transform](#aaboxcoxtransform)
- [aa_difference_once](#aadifferenceonce)
- [aa_difference_series](#aadifferenceseries)
- [aa_difference_twice](#aadifferencetwice)
- [aa_inverse_boxcox_transform](#aainverseboxcoxtransform)
- [aa_inverse_difference](#aainversedifference)
- [aa_inverse_difference_path](#aainversedifferencepath)
- [aa_inverse_log_transform](#aainverselogtransform)
- [aa_inverse_return_forecast](#aainversereturnforecast)
- [aa_inverse_seasonal_difference](#aainverseseasonaldifference)
- [aa_log_transform](#aalogtransform)
- [aa_return_transform](#aareturntransform)
- [aa_seasonal_difference_once](#aaseasonaldifferenceonce)
- [aa_seasonal_difference_series](#aaseasonaldifferenceseries)

### 5. Stationarity and Differencing Selection (16)

- [aa_adf_pvalue_approx](#aaadfpvalueapprox)
- [aa_adf_statistic](#aaadfstatistic)
- [aa_adf_test](#aaadftest)
- [aa_calculate_d](#aacalculated)
- [aa_calculate_D](#aacalculated)
- [aa_is_stationary](#aaisstationary)
- [aa_kpss_pvalue_approx](#aakpsspvalueapprox)
- [aa_kpss_statistic](#aakpssstatistic)
- [aa_kpss_test](#aakpsstest)
- [aa_ndiffs](#aandiffs)
- [aa_ndiffs_heuristic](#aandiffsheuristic)
- [aa_nsdiffs](#aansdiffs)
- [aa_nsdiffs_heuristic](#aansdiffsheuristic)
- [aa_pp_pvalue_approx](#aapppvalueapprox)
- [aa_pp_statistic](#aappstatistic)
- [aa_pp_test](#aapptest)

### 6. Autocorrelation and Partial Autocorrelation (10)

- [aa_acf](#aaacf)
- [aa_acf_cutoff_lag](#aaacfcutofflag)
- [aa_autocorrelation](#aaautocorrelation)
- [aa_autocovariance](#aaautocovariance)
- [aa_initial_ar_from_pacf](#aainitialarfrompacf)
- [aa_initial_ma_from_acf](#aainitialmafromacf)
- [aa_levinson_durbin](#aalevinsondurbin)
- [aa_pacf](#aapacf)
- [aa_pacf_cutoff_lag](#aapacfcutofflag)
- [aa_yule_walker](#aayulewalker)

### 7. AR / MA Validity and Stability (9)

- [aa_ar_root_modulus](#aaarrootmodulus)
- [aa_clamp_coefficients](#aaclampcoefficients)
- [aa_coefficients_are_valid](#aacoefficientsarevalid)
- [aa_enforce_invertibility](#aaenforceinvertibility)
- [aa_enforce_stationarity](#aaenforcestationarity)
- [aa_is_invertible_ma](#aaisinvertiblema)
- [aa_is_stationary_ar](#aaisstationaryar)
- [aa_ma_root_modulus](#aamarootmodulus)
- [aa_min_root_modulus](#aaminrootmodulus)

### 8. ARMA / ARIMA / SARIMA Fitting Functions (7)

- [aa_arima_fit](#aaarimafit)
- [aa_arma_fit](#aaarmafit)
- [aa_css_fit](#aacssfit)
- [aa_exact_mle_fit](#aaexactmlefit)
- [aa_kalman_loglikelihood](#aakalmanloglikelihood)
- [aa_mle_fit](#aamlefit)
- [aa_sarima_fit](#aasarimafit)

### 9. Optimizer Functions (14)

- [aa_adam_fit](#aaadamfit)
- [aa_bfgs_fit](#aabfgsfit)
- [aa_check_convergence](#aacheckconvergence)
- [aa_compute_gradient](#aacomputegradient)
- [aa_compute_hessian](#aacomputehessian)
- [aa_gradient_descent_fit](#aagradientdescentfit)
- [aa_gradient_norm](#aagradientnorm)
- [aa_lbfgs_fit](#aalbfgsfit)
- [aa_nelder_mead_fit](#aaneldermeadfit)
- [aa_optimizer_report](#aaoptimizerreport)
- [aa_set_optimizer_defaults](#aasetoptimizerdefaults)
- [aa_set_optimizer_learning_rate](#aasetoptimizerlearningrate)
- [aa_set_optimizer_max_iter](#aasetoptimizermaxiter)
- [aa_set_optimizer_tolerance](#aasetoptimizertolerance)

### 10. Likelihood and Model Scoring (10)

- [aa_aic_score](#aaaicscore)
- [aa_aicc_score](#aaaiccscore)
- [aa_aicc_score_general](#aaaiccscoregeneral)
- [aa_bic_score](#aabicscore)
- [aa_compare_ic](#aacompareic)
- [aa_conditional_loglikelihood](#aaconditionalloglikelihood)
- [aa_exact_loglikelihood](#aaexactloglikelihood)
- [aa_hqic_score](#aahqicscore)
- [aa_loglikelihood](#aaloglikelihood)
- [aa_model_score](#aamodelscore)

### 11. AutoARIMA Search Functions (11)

- [aa_auto_arima_search](#aaautoarimasearch)
- [aa_candidate_exists](#aacandidateexists)
- [aa_expand_candidate_models](#aaexpandcandidatemodels)
- [aa_fallback_model](#aafallbackmodel)
- [aa_grid_search_arima](#aagridsearcharima)
- [aa_init_candidate](#aainitcandidate)
- [aa_select_best_model](#aaselectbestmodel)
- [aa_set_candidate](#aasetcandidate)
- [aa_stepwise_auto_arima](#aastepwiseautoarima)
- [aa_try_neighbor_models](#aatryneighbormodels)
- [aa_validate_candidate_model](#aavalidatecandidatemodel)

### 12. Seasonal ARIMA / SARIMA Functions (7)

- [aa_auto_sarima_search](#aaautosarimasearch)
- [aa_detect_seasonal_period](#aadetectseasonalperiod)
- [aa_sarima_forecast_multi_step](#aasarimaforecastmultistep)
- [aa_sarima_forecast_one_step](#aasarimaforecastonestep)
- [aa_seasonal_acf_score](#aaseasonalacfscore)
- [aa_seasonal_strength](#aaseasonalstrength)
- [aa_stepwise_auto_sarima](#aastepwiseautosarima)

### 13. ARIMAX / SARIMAX Functions (8)

- [aa_arimax_fit](#aaarimaxfit)
- [aa_arimax_forecast](#aaarimaxforecast)
- [aa_prepare_exogenous_matrix](#aaprepareexogenousmatrix)
- [aa_regression_fit](#aaregressionfit)
- [aa_regression_predict](#aaregressionpredict)
- [aa_sarimax_fit](#aasarimaxfit)
- [aa_sarimax_forecast](#aasarimaxforecast)
- [aa_validate_exogenous_data](#aavalidateexogenousdata)

### 14. Forecasting Functions (9)

- [aa_backtransform_forecast](#aabacktransformforecast)
- [aa_bias_adjusted_backtransform](#aabiasadjustedbacktransform)
- [aa_forecast_bands](#aaforecastbands)
- [aa_forecast_confidence_interval](#aaforecastconfidenceinterval)
- [aa_forecast_multi_step](#aaforecastmultistep)
- [aa_forecast_one_step](#aaforecastonestep)
- [aa_forecast_standard_error](#aaforecaststandarderror)
- [aa_forecast_variance](#aaforecastvariance)
- [aa_integrate_forecast](#aaintegrateforecast)

### 15. Residual Diagnostics (21)

- [aa_arch_lm_pvalue](#aaarchlmpvalue)
- [aa_arch_lm_stat](#aaarchlmstat)
- [aa_arch_lm_test](#aaarchlmtest)
- [aa_box_pierce_pvalue](#aaboxpiercepvalue)
- [aa_box_pierce_stat](#aaboxpiercestat)
- [aa_box_pierce_test](#aaboxpiercetest)
- [aa_compute_residuals](#aacomputeresiduals)
- [aa_diagnostic_report](#aadiagnosticreport)
- [aa_durbin_watson_test](#aadurbinwatsontest)
- [aa_jarque_bera_pvalue](#aajarqueberapvalue)
- [aa_jarque_bera_stat](#aajarqueberastat)
- [aa_jarque_bera_test](#aajarqueberatest)
- [aa_ljung_box_pvalue](#aaljungboxpvalue)
- [aa_ljung_box_stat](#aaljungboxstat)
- [aa_ljung_box_test](#aaljungboxtest)
- [aa_residual_acf](#aaresidualacf)
- [aa_residual_mean](#aaresidualmean)
- [aa_residual_normality_check](#aaresidualnormalitycheck)
- [aa_residual_stddev](#aaresidualstddev)
- [aa_residual_variance](#aaresidualvariance)
- [aa_residual_white_noise_check](#aaresidualwhitenoisecheck)

### 16. Model Reporting and File I/O (8)

- [aa_load_model_from_file](#aaloadmodelfromfile)
- [aa_print_coefficients](#aaprintcoefficients)
- [aa_print_forecast_report](#aaprintforecastreport)
- [aa_print_model_summary](#aaprintmodelsummary)
- [aa_print_residual_diagnostics](#aaprintresidualdiagnostics)
- [aa_save_diagnostics_to_file](#aasavediagnosticstofile)
- [aa_save_forecast_to_file](#aasaveforecasttofile)
- [aa_save_model_to_file](#aasavemodeltofile)

### 17. Model Caching and Reuse (7)

- [aa_cache_best_model](#aacachebestmodel)
- [aa_clear_model_cache](#aaclearmodelcache)
- [aa_init_model_cache](#aainitmodelcache)
- [aa_load_cached_model](#aaloadcachedmodel)
- [aa_refit_best_model](#aarefitbestmodel)
- [aa_reuse_previous_model](#aareusepreviousmodel)
- [aa_should_refit_model](#aashouldrefitmodel)

### 18. Walk-Forward and Forecast Evaluation (8)

- [aa_directional_accuracy](#aadirectionalaccuracy)
- [aa_forecast_error_mae](#aaforecasterrormae)
- [aa_forecast_error_mape](#aaforecasterrormape)
- [aa_forecast_error_mse](#aaforecasterrormse)
- [aa_forecast_error_rmse](#aaforecasterrorrmse)
- [aa_rolling_forecast_test](#aarollingforecasttest)
- [aa_train_test_split](#aatraintestsplit)
- [aa_walk_forward_arima](#aawalkforwardarima)

### 19. Trading Signal Functions (9)

- [aa_backtest_forecast_signal](#aabacktestforecastsignal)
- [aa_forecast_edge](#aaforecastedge)
- [aa_forecast_return](#aaforecastreturn)
- [aa_forecast_zscore](#aaforecastzscore)
- [aa_position_size_from_confidence](#aapositionsizefromconfidence)
- [aa_position_size_from_forecast](#aapositionsizefromforecast)
- [aa_signal_from_confidence_band](#aasignalfromconfidenceband)
- [aa_signal_from_directional_accuracy](#aasignalfromdirectionalaccuracy)
- [aa_signal_from_forecast](#aasignalfromforecast)

### 20. Zorro Integration Functions (6)

- [aa_zorro_forecast_current_asset](#aazorroforecastcurrentasset)
- [aa_zorro_get_close_series](#aazorrogetcloseseries)
- [aa_zorro_plot_forecast](#aazorroplotforecast)
- [aa_zorro_plot_forecast_bands](#aazorroplotforecastbands)
- [aa_zorro_print_model](#aazorroprintmodel)
- [aa_zorro_trade_from_forecast](#aazorrotradefromforecast)

### 21. Current AutoARIMA Compatibility Functions (8)

- [aa_prepare_auto_arima_work](#aaprepareautoarimawork)
- [aa_round_to_tick_size](#aaroundtoticksize)
- [auto_arima_forecast](#autoarimaforecast)
- [auto_arima_forecast_with_work](#autoarimaforecastwithwork)
- [free_auto_arima_result](#freeautoarimaresult)
- [free_auto_arima_work](#freeautoarimawork)
- [init_auto_arima_result](#initautoarimaresult)
- [init_auto_arima_work](#initautoarimawork)

## 1. Memory and Pointer Management Functions

**Optimization requirement:** these functions define memory ownership rules for the whole project. Use them everywhere instead of direct scattered `malloc()` / `free()` calls, except inside the low-level memory helpers themselves.


These functions have operation formulas rather than statistical formulas.
| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_alloc_vars

**Purpose**  
Implements alloc vars within the memory and pointer management functions module.

**Signature**

```c
var* aa_alloc_vars(int Count)
```

**Defined in**  
`src/litec/aa_arima_memory.c:52`

**Mathematical form**

$$
x \in \mathbb{R}^{n},\quad x_i=0,\ i=0,\ldots,n-1
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Count` | `int` | $n$ | Number of elements to allocate or process. |

**Return value**  
Pointer to allocated or prepared memory/buffer.

**Implementation note**  
Allocate `Count*sizeof(var)` with `malloc()`, check for zero pointer, then zero-fill with `memset()`.

### aa_clear_arima_work

**Purpose**  
Implements clear arima work within the memory and pointer management functions module.

**Signature**

```c
void aa_clear_arima_work(ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_memory.c:190`

**Mathematical form**

$$
W_i \leftarrow 0
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Zero only active buffers, not the struct itself, so allocated memory remains reusable.

### aa_copy_vars

**Purpose**  
Implements copy vars within the memory and pointer management functions module.

**Signature**

```c
void aa_copy_vars(vars Dst,vars Src,int N)
```

**Defined in**  
`src/litec/aa_arima_memory.c:79`

**Mathematical form**

$$
d_i \leftarrow s_i,\quad i=0,\ldots,N-1
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Dst` | `vars` | $x^{(dst)}$ | Destination buffer or structure. |
| `Src` | `vars` | $x^{(src)}$ | Source buffer or structure. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Use a loop or `memcpy(Dst,Src,N*sizeof(var))`. Validate both pointers.

### aa_free_vars

**Purpose**  
Implements free vars within the memory and pointer management functions module.

**Signature**

```c
void aa_free_vars(var* Ptr)
```

**Defined in**  
`src/litec/aa_arima_memory.c:67`

**Mathematical form**

$$
\operatorname{free}(x)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Ptr` | `var*` | $x$ | Pointer to memory owned or inspected by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Call `free(Ptr)` only when `Ptr != 0`. The caller should set its pointer field to `0` after freeing.

### aa_prepare_arima_work

**Purpose**  
Implements prepare arima work within the memory and pointer management functions module.

**Signature**

```c
int aa_prepare_arima_work(ARIMA_WORK* Work,int N,int MaxP,int MaxQ,int MaxSP,int MaxSQ,int H)
```

**Defined in**  
`src/litec/aa_arima_memory.c:196`

**Mathematical form**

$$
\operatorname{cap}(W)\ge(N,p,q,P,Q,H)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxP` | `int` | $max_P$ | Maximum allowed value for P in the current routine. |
| `MaxQ` | `int` | $max_Q$ | Maximum allowed value for Q in the current routine. |
| `MaxSP` | `int` | $max_SP$ | Maximum allowed value for SP in the current routine. |
| `MaxSQ` | `int` | $max_SQ$ | Maximum allowed value for SQ in the current routine. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
If existing capacity is sufficient, reuse buffers. Otherwise free/reallocate all needed arrays once.

### aa_shift_vars

**Purpose**  
Implements shift vars within the memory and pointer management functions module.

**Signature**

```c
void aa_shift_vars(vars Data,var NewValue,int N)
```

**Defined in**  
`src/litec/aa_arima_memory.c:85`

**Mathematical form**

$$
x_i \leftarrow x_{i-1},\ i=N-1,\ldots,1;\quad x_0 \leftarrow v
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `NewValue` | `var` | $NewValue$ | Routine-specific input or output argument. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Loop backward so values are not overwritten. This matches newest-first series shifting.

### aa_zero_vars

**Purpose**  
Implements zero vars within the memory and pointer management functions module.

**Signature**

```c
void aa_zero_vars(vars Data,int N)
```

**Defined in**  
`src/litec/aa_arima_memory.c:73`

**Mathematical form**

$$
x_i \leftarrow 0,\quad i=0,\ldots,N-1
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Use `memset(Data,0,N*sizeof(var))` after validating `Data != 0` and `N > 0`.

### copy_arima_model

**Purpose**  
Implements arima model within the memory and pointer management functions module.

**Signature**

```c
void copy_arima_model(ARIMA_MODEL* Dst,ARIMA_MODEL* Src)
```

**Defined in**  
`src/litec/aa_arima_memory.c:149`

**Mathematical form**

$$
M_{\mathrm{dst}} \leftarrow M_{\mathrm{src}}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Dst` | `ARIMA_MODEL*` | $x^{(dst)}$ | Destination buffer or structure. |
| `Src` | `ARIMA_MODEL*` | $x^{(src)}$ | Source buffer or structure. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Copy scalar fields directly. Deep-copy pointer arrays into already prepared destination memory, or allocate before copying.

### free_arima_model

**Purpose**  
Implements arima model within the memory and pointer management functions module.

**Signature**

```c
void free_arima_model(ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_memory.c:135`

**Mathematical form**

$$
\forall a\in\{\phi,\vartheta,\Phi,\Theta,\epsilon\}: \operatorname{free}(a)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Free all owned dynamic arrays in the model and set pointers to `0`.

### free_arima_work

**Purpose**  
Implements arima work within the memory and pointer management functions module.

**Signature**

```c
void free_arima_work(ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_memory.c:181`

**Mathematical form**

$$
\forall w_i\in W: \operatorname{free}(w_i)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Free every allocated work buffer and reset capacities/pointers.

### init_arima_model

**Purpose**  
Implements arima model within the memory and pointer management functions module.

**Signature**

```c
void init_arima_model(ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_memory.c:98`

**Mathematical form**

$$
M \leftarrow 0,\quad p=d=q=P=D=Q=m=0
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Set all scalar fields to safe defaults and all pointer fields to `0`.

### init_arima_work

**Purpose**  
Implements arima work within the memory and pointer management functions module.

**Signature**

```c
void init_arima_work(ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_memory.c:175`

**Mathematical form**

$$
W \leftarrow 0
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Set all capacities to 0 and all work pointers to `0`.

### reset_arima_model

**Purpose**  
Implements arima model within the memory and pointer management functions module.

**Signature**

```c
void reset_arima_model(ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_memory.c:111`

**Mathematical form**

$$
M_{\text{scores}}\leftarrow\infty,\quad M_{\text{status}}\leftarrow0
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Reset scores/status and optionally zero coefficient arrays without freeing memory.

## 2. Basic Math and Array Utilities

**Optimization requirement:** use Zorro/lite-C built-ins such as `abs()`, `min()`, `max()`, `clamp()`, `fix0()`, `sqrt()`, and `log()` where applicable. Do not recreate built-ins unless a safety wrapper or consistent function interface is needed.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_abs

**Purpose**  
Implements abs within the basic math and array utilities module.

**Signature**

```c
var aa_abs(var X)
```

**Defined in**  
`src/litec/aa_arima_math.c:137`

**Mathematical form**

$$

$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `X` | `var` | $X_t$ | Exogenous regressor matrix or vector. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
x

### aa_argmax

**Purpose**  
Implements argmax within the basic math and array utilities module.

**Signature**

```c
int aa_argmax(vars Data,int N)
```

**Defined in**  
`src/litec/aa_arima_math.c:287`

**Mathematical form**

$$
\operatorname{argmax}_i y_i
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Index of the selected element.

**Implementation note**  
Track best value and best index while looping.

### aa_argmin

**Purpose**  
Implements argmin within the basic math and array utilities module.

**Signature**

```c
int aa_argmin(vars Data,int N)
```

**Defined in**  
`src/litec/aa_arima_math.c:271`

**Mathematical form**

$$
\operatorname{argmin}_i y_i
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Index of the selected element.

**Implementation note**  
Track best value and best index while looping.

### aa_correlation

**Purpose**  
Implements correlation within the basic math and array utilities module.

**Signature**

```c
var aa_correlation(vars X,vars Y,int N)
```

**Defined in**  
`src/litec/aa_arima_math.c:232`

**Mathematical form**

$$
\rho_{xy}=\frac{\operatorname{cov}(x,y)}{s_xs_y}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `X` | `vars` | $X_t$ | Exogenous regressor matrix or vector. |
| `Y` | `vars` | $Y$ | Vector input or output buffer used by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compute covariance and standard deviations; guard zero denominator.

### aa_covariance

**Purpose**  
Implements covariance within the basic math and array utilities module.

**Signature**

```c
var aa_covariance(vars X,vars Y,int N)
```

**Defined in**  
`src/litec/aa_arima_math.c:213`

**Mathematical form**

$$
\operatorname{cov}(x,y)=\frac{1}{N}\sum_{t=0}^{N-1}(x_t-\bar x)(y_t-\bar y)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `X` | `vars` | $X_t$ | Exogenous regressor matrix or vector. |
| `Y` | `vars` | $Y$ | Vector input or output buffer used by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compute both means, then loop through paired arrays.

### aa_demean

**Purpose**  
Implements demean within the basic math and array utilities module.

**Signature**

```c
void aa_demean(vars Data,int N,vars Out)
```

**Defined in**  
`src/litec/aa_arima_math.c:303`

**Mathematical form**

$$
z_t=y_t-\bar y
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Compute mean and write `Out[i] = Data[i]-Mu`.

### aa_max

**Purpose**  
Implements max within the basic math and array utilities module.

**Signature**

```c
var aa_max(vars Data,int N)
```

**Defined in**  
`src/litec/aa_arima_math.c:255`

**Mathematical form**

$$
\max_i y_i
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Initialize with `Data[0]`, loop and update.

### aa_max_int

**Purpose**  
Implements max int within the basic math and array utilities module.

**Signature**

```c
int aa_max_int(int A,int B)
```

**Defined in**  
`src/litec/aa_arima_math.c:127`

**Mathematical form**

$$
\max(A,B)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `A` | `int` | $A$ | Routine-specific input or output argument. |
| `B` | `int` | $B$ | Routine-specific input or output argument. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Return `A` when `A > B`, otherwise `B`.

### aa_mean

**Purpose**  
Implements mean within the basic math and array utilities module.

**Signature**

```c
var aa_mean(vars Data,int N)
```

**Defined in**  
`src/litec/aa_arima_math.c:183`

**Mathematical form**

$$
\bar y=\frac{1}{N}\sum_{t=0}^{N-1}y_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Call `aa_sum()` and divide by `(var)N`; return 0 when invalid.

### aa_min

**Purpose**  
Implements min within the basic math and array utilities module.

**Signature**

```c
var aa_min(vars Data,int N)
```

**Defined in**  
`src/litec/aa_arima_math.c:239`

**Mathematical form**

$$
\min_i y_i
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Initialize with `Data[0]`, loop and update.

### aa_min_int

**Purpose**  
Implements min int within the basic math and array utilities module.

**Signature**

```c
int aa_min_int(int A,int B)
```

**Defined in**  
`src/litec/aa_arima_math.c:132`

**Mathematical form**

$$
\min(A,B)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `A` | `int` | $A$ | Routine-specific input or output argument. |
| `B` | `int` | $B$ | Routine-specific input or output argument. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Return `A` when `A < B`, otherwise `B`.

### aa_normalize_minmax

**Purpose**  
Implements normalize minmax within the basic math and array utilities module.

**Signature**

```c
void aa_normalize_minmax(vars Data,int N,vars Out)
```

**Defined in**  
`src/litec/aa_arima_math.c:334`

**Mathematical form**

$$
z_t=\frac{y_t-\min(y)}{\max(y)-\min(y)}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Compute min/max; guard equal min/max; write output in 0..1 range.

### aa_safe_div

**Purpose**  
Implements safe div within the basic math and array utilities module.

**Signature**

```c
var aa_safe_div(var A,var B)
```

**Defined in**  
`src/litec/aa_arima_math.c:147`

**Mathematical form**

$$
$\frac{a}{b^\*},\quad b^\*=\begin{cases}b,&
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `A` | `var` | $A$ | Routine-specific input or output argument. |
| `B` | `var` | $B$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
b

### aa_safe_log

**Purpose**  
Implements safe log within the basic math and array utilities module.

**Signature**

```c
var aa_safe_log(var X)
```

**Defined in**  
`src/litec/aa_arima_math.c:152`

**Mathematical form**

$$
\log(x^\*),\quad x^\*=\max(x,\varepsilon)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `X` | `var` | $X_t$ | Exogenous regressor matrix or vector. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Clamp input to a small positive epsilon before calling `log()`.

### aa_safe_sqrt

**Purpose**  
Implements safe sqrt within the basic math and array utilities module.

**Signature**

```c
var aa_safe_sqrt(var X)
```

**Defined in**  
`src/litec/aa_arima_math.c:160`

**Mathematical form**

$$
\sqrt{x^\*},\quad x^\*=\max(x,0)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `X` | `var` | $X_t$ | Exogenous regressor matrix or vector. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Return `sqrt(max(0.,X))` or manual `if` guard.

### aa_square

**Purpose**  
Implements square within the basic math and array utilities module.

**Signature**

```c
var aa_square(var X)
```

**Defined in**  
`src/litec/aa_arima_math.c:142`

**Mathematical form**

$$
x^2
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `X` | `var` | $X_t$ | Exogenous regressor matrix or vector. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Return `X*X`; faster than `pow(X,2)`.

### aa_standardize

**Purpose**  
Implements standardize within the basic math and array utilities module.

**Signature**

```c
void aa_standardize(vars Data,int N,vars Out)
```

**Defined in**  
`src/litec/aa_arima_math.c:316`

**Mathematical form**

$$
z_t=\frac{y_t-\bar y}{s}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Compute mean and stddev; guard `s=0`; write standardized output.

### aa_stddev

**Purpose**  
Implements stddev within the basic math and array utilities module.

**Signature**

```c
var aa_stddev(vars Data,int N)
```

**Defined in**  
`src/litec/aa_arima_math.c:208`

**Mathematical form**

$$
s=\sqrt{\frac{1}{N}\sum_{t=0}^{N-1}(y_t-\bar y)^2}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Return `sqrt(aa_variance(Data,N))` with guard.

### aa_sum

**Purpose**  
Implements sum within the basic math and array utilities module.

**Signature**

```c
var aa_sum(vars Data,int N)
```

**Defined in**  
`src/litec/aa_arima_math.c:168`

**Mathematical form**

$$
S=\sum_{t=0}^{N-1}y_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Loop over `Data[i]` and accumulate in `var Sum`.

### aa_variance

**Purpose**  
Implements variance within the basic math and array utilities module.

**Signature**

```c
var aa_variance(vars Data,int N)
```

**Defined in**  
`src/litec/aa_arima_math.c:191`

**Mathematical form**

$$
s^2=\frac{1}{N}\sum_{t=0}^{N-1}(y_t-\bar y)^2
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Data` | `vars` | $y_t$ | Primary input vector processed by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compute mean first, then sum squared deviations.

## 3. Input Validation and Data Cleaning

**Optimization requirement:** combine validation and counting in one pass when possible. Do not copy or clean data unless the caller needs a new buffer; prefer returning status flags and indexes.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_clip_outliers

**Purpose**  
Implements clip outliers within the input validation and data cleaning module.

**Signature**

```c
void aa_clip_outliers(vars Series,int N,var Lower,var Upper)
```

**Defined in**  
`src/litec/aa_arima_data.c:153`

**Mathematical form**

$$
y_t\leftarrow\min(\max(y_t,L),U)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lower` | `var` | $L_h$ | Lower confidence or prediction bound written by reference. |
| `Upper` | `var` | $U_h$ | Upper confidence or prediction bound written by reference. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Clamp each value between `Lower` and `Upper`.

### aa_count_invalid_values

**Purpose**  
Implements count invalid values within the input validation and data cleaning module.

**Signature**

```c
int aa_count_invalid_values(vars Series,int N)
```

**Defined in**  
`src/litec/aa_arima_data.c:63`

**Mathematical form**

$$
C=\sum_t I(\operatorname{invalid}(y_t)\ne0)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Integer count determined by the routine.

**Implementation note**  
Loop and increment counter for invalid values.

### aa_fill_missing_forward

**Purpose**  
Implements fill missing forward within the input validation and data cleaning module.

**Signature**

```c
void aa_fill_missing_forward(vars Series,int N)
```

**Defined in**  
`src/litec/aa_arima_data.c:96`

**Mathematical form**

$$
y_t\leftarrow y_{t-1}\quad\text{when }y_t\text{ invalid}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Keep last valid value and replace invalid entries.

### aa_fill_missing_mean

**Purpose**  
Implements fill missing mean within the input validation and data cleaning module.

**Signature**

```c
void aa_fill_missing_mean(vars Series,int N)
```

**Defined in**  
`src/litec/aa_arima_data.c:113`

**Mathematical form**

$$
y_t\leftarrow\bar y_{\mathrm{valid}}\quad\text{when }y_t\text{ invalid}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Compute mean over valid values, then replace invalid entries.

### aa_has_invalid_values

**Purpose**  
Implements has invalid values within the input validation and data cleaning module.

**Signature**

```c
int aa_has_invalid_values(vars Series,int N)
```

**Defined in**  
`src/litec/aa_arima_data.c:49`

**Mathematical form**

$$
\exists t:\operatorname{invalid}(y_t)\ne0
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Loop and return 1 at first invalid value.

### aa_limit_returns

**Purpose**  
Implements limit returns within the input validation and data cleaning module.

**Signature**

```c
void aa_limit_returns(vars Series,int N,var MaxAbsReturn)
```

**Defined in**  
`src/litec/aa_arima_data.c:181`

**Mathematical form**

$$
$r_t=\frac{y_t}{y_{t-1}}-1,\quad
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxAbsReturn` | `var` | $max_AbsReturn$ | Maximum allowed value for AbsReturn in the current routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
r_t

### aa_remove_invalid_values

**Purpose**  
Implements remove invalid values within the input validation and data cleaning module.

**Signature**

```c
int aa_remove_invalid_values(vars In,int N,vars Out)
```

**Defined in**  
`src/litec/aa_arima_data.c:79`

**Mathematical form**

$$
z_j=y_t\ \text{for valid }y_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `In` | `vars` | $In$ | Vector input or output buffer used by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Loop through input, copy only valid observations, return new count.

### aa_replace_zero_prices

**Purpose**  
Implements replace zero prices within the input validation and data cleaning module.

**Signature**

```c
void aa_replace_zero_prices(vars Series,int N,var Replacement)
```

**Defined in**  
`src/litec/aa_arima_data.c:141`

**Mathematical form**

$$
y_t\leftarrow r\quad\text{when }y_t\le0
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Replacement` | `var` | $Replacement$ | Routine-specific input or output argument. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Loop and replace zero/nonpositive prices with the supplied replacement.

### aa_validate_price_series

**Purpose**  
Implements validate price series within the input validation and data cleaning module.

**Signature**

```c
int aa_validate_price_series(vars Close,int N)
```

**Defined in**  
`src/litec/aa_arima_data.c:35`

**Mathematical form**

$$
\forall t:\ y_t>0
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Check positive prices, no invalid numbers, and enough samples.

### aa_validate_series

**Purpose**  
Implements validate series within the input validation and data cleaning module.

**Signature**

```c
int aa_validate_series(vars Series,int N)
```

**Defined in**  
`src/litec/aa_arima_data.c:21`

**Mathematical form**

$$
\forall t:\ y_t\in\mathbb{R},\ N>N_{\min}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Check pointer, length, and invalid/infinite values with `invalid()`.

### aa_winsorize_series

**Purpose**  
Implements winsorize series within the input validation and data cleaning module.

**Signature**

```c
void aa_winsorize_series(vars Series,int N,var Percentile)
```

**Defined in**  
`src/litec/aa_arima_data.c:164`

**Mathematical form**

$$
y_t\leftarrow\min(\max(y_t,Q_\alpha),Q_{1-\alpha})
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Percentile` | `var` | $Percentile$ | Routine-specific input or output argument. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Estimate lower/upper percentiles, then clamp values. Requires either sorting copy or approximate quantile.

## 4. Transformations and Differencing

**Optimization requirement:** write transformed data into caller-provided or `ARIMA_WORK` buffers. Do not allocate temporary arrays inside differencing functions.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_boxcox_lambda

**Purpose**  
Implements boxcox lambda within the transformations and differencing module.

**Signature**

```c
var aa_boxcox_lambda(vars Series,int N)
```

**Defined in**  
`src/litec/aa_arima_transform.c:26`

**Mathematical form**

$$
\lambda^\*=\arg\max_\lambda \ell(\lambda)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Grid-search or optimize lambda over a range such as `-2..2`; use Box-Cox log-likelihood.

### aa_boxcox_transform

**Purpose**  
Implements boxcox transform within the transformations and differencing module.

**Signature**

```c
void aa_boxcox_transform(vars Series,int N,var Lambda,vars Out)
```

**Defined in**  
`src/litec/aa_arima_transform.c:31`

**Mathematical form**

$$
z_t=\begin{cases}\frac{y_t^\lambda-1}{\lambda},&\lambda\ne0\\\\\log(y_t),&\lambda=0\end{cases}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lambda` | `var` | $\lambda$ | Box-Cox transformation parameter. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Branch on `abs(Lambda)` near zero; validate positive input.

### aa_difference_once

**Purpose**  
Implements difference once within the transformations and differencing module.

**Signature**

```c
void aa_difference_once(vars Series,int N,vars Out)
```

**Defined in**  
`src/litec/aa_arima_transform.c:92`

**Mathematical form**

$$
\Delta y_t=y_t-y_{t-1}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
For chronological data, write `Out[t-1]=Series[t]-Series[t-1]` for `t=1..N-1`.

### aa_difference_series

**Purpose**  
Implements difference series within the transformations and differencing module.

**Signature**

```c
int aa_difference_series(int D,vars Close,int N,vars Diff)
```

**Defined in**  
`src/litec/aa_arima_transform.c:112`

**Mathematical form**

$$
z_t=(1-B)^D y_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `D` | `int` | $d$ | Non-seasonal differencing order. |
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Diff` | `vars` | $Diff$ | Vector input or output buffer used by the routine. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
If input is Zorro newest-first, write chronological differenced output into `Diff`.

### aa_difference_twice

**Purpose**  
Implements difference twice within the transformations and differencing module.

**Signature**

```c
void aa_difference_twice(vars Series,int N,vars Out)
```

**Defined in**  
`src/litec/aa_arima_transform.c:103`

**Mathematical form**

$$
\Delta^2 y_t=y_t-2y_{t-1}+y_{t-2}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Either difference once twice or compute direct formula from `t=2`.

### aa_inverse_boxcox_transform

**Purpose**  
Implements inverse boxcox transform within the transformations and differencing module.

**Signature**

```c
void aa_inverse_boxcox_transform(vars Series,int N,var Lambda,vars Out)
```

**Defined in**  
`src/litec/aa_arima_transform.c:46`

**Mathematical form**

$$
y_t=\begin{cases}(\lambda z_t+1)^{1/\lambda},&\lambda\ne0\\\\\exp(z_t),&\lambda=0\end{cases}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lambda` | `var` | $\lambda$ | Box-Cox transformation parameter. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Apply inverse branch; guard negative base when lambda is not zero.

### aa_inverse_difference

**Purpose**  
Implements inverse difference within the transformations and differencing module.

**Signature**

```c
void aa_inverse_difference(var LastPrice,vars DiffForecast,int H,int D,vars Out)
```

**Defined in**  
`src/litec/aa_arima_transform.c:132`

**Mathematical form**

$$
\hat y_{T+h}=\hat y_{T+h-1}+\widehat{\Delta y}_{T+h}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `LastPrice` | `var` | $LastPrice$ | Routine-specific input or output argument. |
| `DiffForecast` | `vars` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `D` | `int` | $d$ | Non-seasonal differencing order. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
For `D=1`, cumulatively add forecasts to last price; for `D=2`, carry last first-difference too.

### aa_inverse_difference_path

**Purpose**  
Implements inverse difference path within the transformations and differencing module.

**Signature**

```c
void aa_inverse_difference_path(vars Original,int N,vars DiffForecast,int H,int D,vars Out)
```

**Defined in**  
`src/litec/aa_arima_transform.c:147`

**Mathematical form**

$$
\hat y_{T+h}=\operatorname{integrate}_D(\widehat{\Delta^D y}_{T+h})
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Original` | `vars` | $y_t$ | Original non-differenced series used for inversion or back-transformation. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `DiffForecast` | `vars` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `D` | `int` | $d$ | Non-seasonal differencing order. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Read last price and recent differences from `Original`, then integrate the forecast path.

### aa_inverse_log_transform

**Purpose**  
Implements inverse log transform within the transformations and differencing module.

**Signature**

```c
void aa_inverse_log_transform(vars Series,int N,vars Out)
```

**Defined in**  
`src/litec/aa_arima_transform.c:15`

**Mathematical form**

$$
y_t=\exp(z_t)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Loop and write `Out[i]=exp(Series[i])`.

### aa_inverse_return_forecast

**Purpose**  
Implements inverse return forecast within the transformations and differencing module.

**Signature**

```c
void aa_inverse_return_forecast(var LastPrice,vars ReturnForecast,int H,vars PriceForecast)
```

**Defined in**  
`src/litec/aa_arima_transform.c:77`

**Mathematical form**

$$
\hat y_{T+h}=\hat y_{T+h-1}(1+\hat r_h)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `LastPrice` | `var` | $LastPrice$ | Routine-specific input or output argument. |
| `ReturnForecast` | `vars` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `PriceForecast` | `vars` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Start with `LastPrice`, multiply by `1+ReturnForecast[h]` for each horizon.

### aa_inverse_seasonal_difference

**Purpose**  
Implements inverse seasonal difference within the transformations and differencing module.

**Signature**

```c
void aa_inverse_seasonal_difference(vars Original,int N,vars Forecast,int H,int D,int M,vars Out)
```

**Defined in**  
`src/litec/aa_arima_transform.c:186`

**Mathematical form**

$$
\hat y_{T+h}=\hat z_{T+h}+y_{T+h-m}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Original` | `vars` | $y_t$ | Original non-differenced series used for inversion or back-transformation. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Forecast` | `vars` | $\hat y_h$ | Forecast value or one-step forecast input. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `D` | `int` | $d$ | Non-seasonal differencing order. |
| `M` | `int` | $m$ | Seasonal period. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
For each horizon, add back the value one seasonal period earlier, using actual history or earlier forecasts.

**Also documented under**  
12. Seasonal ARIMA / SARIMA Functions

### aa_log_transform

**Purpose**  
Implements log transform within the transformations and differencing module.

**Signature**

```c
void aa_log_transform(vars Series,int N,vars Out)
```

**Defined in**  
`src/litec/aa_arima_transform.c:4`

**Mathematical form**

$$
z_t=\log(y_t)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Validate `y_t > 0`, then write `Out[i]=log(Series[i])`.

### aa_return_transform

**Purpose**  
Implements return transform within the transformations and differencing module.

**Signature**

```c
void aa_return_transform(vars Close,int N,vars Returns)
```

**Defined in**  
`src/litec/aa_arima_transform.c:66`

**Mathematical form**

$$
r_t=\frac{y_t}{y_{t-1}}-1
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Returns` | `vars` | $Returns$ | Vector input or output buffer used by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
For chronological prices, loop from `t=1`; output length is `N-1`.

### aa_seasonal_difference_once

**Purpose**  
Implements seasonal difference once within the transformations and differencing module.

**Signature**

```c
void aa_seasonal_difference_once(vars Series,int N,int M,vars Out)
```

**Defined in**  
`src/litec/aa_arima_transform.c:155`

**Mathematical form**

$$
\Delta_m y_t=y_t-y_{t-m}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `M` | `int` | $m$ | Seasonal period. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
For chronological data, write `Out[t-m]=Series[t]-Series[t-m]`.

### aa_seasonal_difference_series

**Purpose**  
Implements seasonal difference series within the transformations and differencing module.

**Signature**

```c
int aa_seasonal_difference_series(vars Series,int N,int D,int M,vars Out)
```

**Defined in**  
`src/litec/aa_arima_transform.c:166`

**Mathematical form**

$$
z_t=(1-B^m)^D y_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `D` | `int` | $d$ | Non-seasonal differencing order. |
| `M` | `int` | $m$ | Seasonal period. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Apply seasonal difference `D` times into work buffers. Return final length.

**Also documented under**  
12. Seasonal ARIMA / SARIMA Functions

## 5. Stationarity and Differencing Selection

**Optimization requirement:** reuse variance, ACF, regression, and differencing helpers. Cache test results when the same window is checked repeatedly.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_adf_pvalue_approx

**Purpose**  
Implements adf pvalue approx within the stationarity and differencing selection module.

**Signature**

```c
var aa_adf_pvalue_approx(var Statistic,int N)
```

**Defined in**  
`src/litec/aa_arima_stats.c:402`

**Mathematical form**

$$
p\approx F_{\mathrm{ADF}}(\tau,N)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Statistic` | `var` | $Statistic$ | Routine-specific input or output argument. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Use a lookup/approximation table for ADF critical values; lite-C can use piecewise interpolation.

### aa_adf_statistic

**Purpose**  
Implements adf statistic within the stationarity and differencing selection module.

**Signature**

```c
var aa_adf_statistic(vars Series,int N,int Lag)
```

**Defined in**  
`src/litec/aa_arima_stats.c:395`

**Mathematical form**

$$
\Delta y_t=\alpha+\beta t+\gamma y_{t-1}+\sum_{i=1}^{L}\psi_i\Delta y_{t-i}+\epsilon_t,\quad \tau=\frac{\hat\gamma}{SE(\hat\gamma)}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lag` | `int` | $Lag$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Build lagged regression matrix and estimate OLS coefficients; return t-statistic of `gamma`.

### aa_adf_test

**Purpose**  
Implements adf test within the stationarity and differencing selection module.

**Signature**

```c
int aa_adf_test(vars Series,int N,int Lag,var Alpha)
```

**Defined in**  
`src/litec/aa_arima_stats.c:406`

**Mathematical form**

$$
I(p<\alpha)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lag` | `int` | $Lag$ | Routine-specific input or output argument. |
| `Alpha` | `var` | $\alpha$ | Tail probability or confidence significance level. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Compute ADF statistic and approximate p-value; return 1 when unit root is rejected.

### aa_calculate_d

**Purpose**  
Implements calculate d within the stationarity and differencing selection module.

**Signature**

```c
int aa_calculate_d(vars Series,int N)
```

**Defined in**  
`src/litec/aa_arima_stats.c:330`

**Mathematical form**

$$
d=\arg\min_{d\in\{0,1,2\}}\operatorname{Var}(\Delta^d y)\ \text{under heuristic rules}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Use existing variance-ratio heuristic; return 0, 1, or 2.

### aa_calculate_D

**Purpose**  
Implements calculate D within the stationarity and differencing selection module.

**Signature**

```c
int aa_calculate_D(vars Series,int N,int SeasonalPeriod)
```

**Defined in**  
`src/litec/aa_arima_stats.c:362`

**Mathematical form**

$$
D=\arg\min_{D\in\{0,1\}}\operatorname{Var}((1-B^m)^D y)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `SeasonalPeriod` | `int` | $SeasonalPeriod$ | Routine-specific input or output argument. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Compare seasonal differenced variance to original variance.

**Also documented under**  
12. Seasonal ARIMA / SARIMA Functions

### aa_is_stationary

**Purpose**  
Implements is stationary within the stationarity and differencing selection module.

**Signature**

```c
int aa_is_stationary(vars Series,int N)
```

**Defined in**  
`src/litec/aa_arima_stats.c:457`

**Mathematical form**

$$
\text{stationary}=I(\text{ADF reject})\land I(\text{KPSS not reject})
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Combine stationarity tests or fall back to heuristic when test functions are disabled.

### aa_kpss_pvalue_approx

**Purpose**  
Implements kpss pvalue approx within the stationarity and differencing selection module.

**Signature**

```c
var aa_kpss_pvalue_approx(var Statistic,int N)
```

**Defined in**  
`src/litec/aa_arima_stats.c:435`

**Mathematical form**

$$
p\approx F_{\mathrm{KPSS}}(\eta,N)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Statistic` | `var` | $Statistic$ | Routine-specific input or output argument. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Use KPSS critical-value interpolation. KPSS null is stationarity.

### aa_kpss_statistic

**Purpose**  
Implements kpss statistic within the stationarity and differencing selection module.

**Signature**

```c
var aa_kpss_statistic(vars Series,int N,int Lag)
```

**Defined in**  
`src/litec/aa_arima_stats.c:411`

**Mathematical form**

$$
\eta=\frac{1}{N^2\hat\sigma^2}\sum_{t=1}^{N}S_t^2,\quad S_t=\sum_{i=1}^{t}e_i
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lag` | `int` | $Lag$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Demean/detrend, compute cumulative residual sum, estimate long-run variance, return KPSS statistic.

### aa_kpss_test

**Purpose**  
Implements kpss test within the stationarity and differencing selection module.

**Signature**

```c
int aa_kpss_test(vars Series,int N,int Lag,var Alpha)
```

**Defined in**  
`src/litec/aa_arima_stats.c:439`

**Mathematical form**

$$
I(p>\alpha)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lag` | `int` | $Lag$ | Routine-specific input or output argument. |
| `Alpha` | `var` | $\alpha$ | Tail probability or confidence significance level. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Return 1 when KPSS does not reject stationarity; document null direction clearly.

### aa_ndiffs

**Purpose**  
Implements ndiffs within the stationarity and differencing selection module.

**Signature**

```c
int aa_ndiffs(vars Series,int N,int MaxD)
```

**Defined in**  
`src/litec/aa_arima_stats.c:466`

**Mathematical form**

$$
d=\min\{k: \Delta^k y \text{ is stationary}\}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxD` | `int` | $max_D$ | Maximum allowed value for D in the current routine. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Loop `k=0..MaxD`, call stationarity test on differenced data, return first passing `k`.

### aa_ndiffs_heuristic

**Purpose**  
Implements ndiffs heuristic within the stationarity and differencing selection module.

**Signature**

```c
int aa_ndiffs_heuristic(vars Series,int N,int MaxD)
```

**Defined in**  
`src/litec/aa_arima_stats.c:373`

**Mathematical form**

$$
d=\min\{k:\Delta^k y\text{ appears stationary}\}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxD` | `int` | $max_D$ | Maximum allowed value for D in the current routine. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Iteratively difference up to `MaxD`; stop when variance or autocorrelation heuristic passes.

### aa_nsdiffs

**Purpose**  
Implements nsdiffs within the stationarity and differencing selection module.

**Signature**

```c
int aa_nsdiffs(vars Series,int N,int MaxD,int SeasonalPeriod)
```

**Defined in**  
`src/litec/aa_arima_stats.c:495`

**Mathematical form**

$$
D=\min\{k: (1-B^m)^k y \text{ is seasonally stationary}\}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxD` | `int` | $max_D$ | Maximum allowed value for D in the current routine. |
| `SeasonalPeriod` | `int` | $SeasonalPeriod$ | Routine-specific input or output argument. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Loop seasonal differences and test seasonal autocorrelation/stationarity.

**Also documented under**  
12. Seasonal ARIMA / SARIMA Functions

### aa_nsdiffs_heuristic

**Purpose**  
Implements nsdiffs heuristic within the stationarity and differencing selection module.

**Signature**

```c
int aa_nsdiffs_heuristic(vars Series,int N,int MaxD,int M)
```

**Defined in**  
`src/litec/aa_arima_stats.c:384`

**Mathematical form**

$$
D=\min\{k:(1-B^m)^k y\text{ appears seasonally stationary}\}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxD` | `int` | $max_D$ | Maximum allowed value for D in the current routine. |
| `M` | `int` | $m$ | Seasonal period. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Apply seasonal differencing up to `MaxD` and evaluate seasonal autocorrelation/variance.

### aa_pp_pvalue_approx

**Purpose**  
Implements pp pvalue approx within the stationarity and differencing selection module.

**Signature**

```c
var aa_pp_pvalue_approx(var Statistic,int N)
```

**Defined in**  
`src/litec/aa_arima_stats.c:448`

**Mathematical form**

$$
p\approx F_{\mathrm{PP}}(Z_\tau,N)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Statistic` | `var` | $Statistic$ | Routine-specific input or output argument. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Use critical-value interpolation similar to ADF.

### aa_pp_statistic

**Purpose**  
Implements pp statistic within the stationarity and differencing selection module.

**Signature**

```c
var aa_pp_statistic(vars Series,int N,int Lag)
```

**Defined in**  
`src/litec/aa_arima_stats.c:444`

**Mathematical form**

$$
Z_\tau=\tau_{\gamma}+\text{Newey--West correction}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lag` | `int` | $Lag$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Fit unit-root regression and adjust statistic for serial correlation using long-run variance.

### aa_pp_test

**Purpose**  
Implements pp test within the stationarity and differencing selection module.

**Signature**

```c
int aa_pp_test(vars Series,int N,int Lag,var Alpha)
```

**Defined in**  
`src/litec/aa_arima_stats.c:452`

**Mathematical form**

$$
I(p<\alpha)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lag` | `int` | $Lag$ | Routine-specific input or output argument. |
| `Alpha` | `var` | $\alpha$ | Tail probability or confidence significance level. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Return 1 when unit root is rejected.

## 6. Autocorrelation and Partial Autocorrelation

**Optimization requirement:** compute mean once, reuse autocovariance results, and store ACF/PACF output in work buffers. PACF must reuse ACF and Levinson-Durbin rather than recomputing correlations repeatedly.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_acf

**Purpose**  
Implements acf within the autocorrelation and partial autocorrelation module.

**Signature**

```c
void aa_acf(vars Series,int N,int MaxLag,vars OutACF)
```

**Defined in**  
`src/litec/aa_arima_stats.c:65`

**Mathematical form**

$$
\rho_k,\quad k=0,\ldots,K
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxLag` | `int` | $max_Lag$ | Maximum allowed value for Lag in the current routine. |
| `OutACF` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Loop lags 0..MaxLag and fill `OutACF[k]`.

### aa_acf_cutoff_lag

**Purpose**  
Implements acf cutoff lag within the autocorrelation and partial autocorrelation module.

**Signature**

```c
int aa_acf_cutoff_lag(vars ACF,int MaxLag,var Threshold)
```

**Defined in**  
`src/litec/aa_arima_stats.c:137`

**Mathematical form**

$$
$k^\*=\min\{k:
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `ACF` | `vars` | $ACF$ | Vector input or output buffer used by the routine. |
| `MaxLag` | `int` | $max_Lag$ | Maximum allowed value for Lag in the current routine. |
| `Threshold` | `var` | $\tau$ | Decision threshold used for classification or trading logic. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
\rho_k

### aa_autocorrelation

**Purpose**  
Implements autocorrelation within the autocorrelation and partial autocorrelation module.

**Signature**

```c
var aa_autocorrelation(vars Series,int N,int Lag)
```

**Defined in**  
`src/litec/aa_arima_stats.c:60`

**Mathematical form**

$$
\rho_k=\frac{\gamma_k}{\gamma_0}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lag` | `int` | $Lag$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compute `aa_autocovariance(...,Lag)` divided by lag-0 autocovariance.

### aa_autocovariance

**Purpose**  
Implements autocovariance within the autocorrelation and partial autocorrelation module.

**Signature**

```c
var aa_autocovariance(vars Series,int N,int Lag)
```

**Defined in**  
`src/litec/aa_arima_stats.c:43`

**Mathematical form**

$$
\gamma_k=\frac{1}{N}\sum_{t=k}^{N-1}(y_t-\bar y)(y_{t-k}-\bar y)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lag` | `int` | $Lag$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compute mean, then loop from `Lag` to `N-1`.

### aa_initial_ar_from_pacf

**Purpose**  
Implements initial ar from pacf within the autocorrelation and partial autocorrelation module.

**Signature**

```c
void aa_initial_ar_from_pacf(vars PACF,int MaxP,vars ArOut)
```

**Defined in**  
`src/litec/aa_arima_stats.c:200`

**Mathematical form**

$$
\phi_i^{(0)}\approx\alpha_i
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `PACF` | `vars` | $PACF$ | Vector input or output buffer used by the routine. |
| `MaxP` | `int` | $max_P$ | Maximum allowed value for P in the current routine. |
| `ArOut` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Copy or shrink PACF values into initial AR coefficient guesses.

### aa_initial_ma_from_acf

**Purpose**  
Implements initial ma from acf within the autocorrelation and partial autocorrelation module.

**Signature**

```c
void aa_initial_ma_from_acf(vars ACF,int MaxQ,vars MaOut)
```

**Defined in**  
`src/litec/aa_arima_stats.c:211`

**Mathematical form**

$$
\vartheta_j^{(0)}\approx\rho_j
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `ACF` | `vars` | $ACF$ | Vector input or output buffer used by the routine. |
| `MaxQ` | `int` | $max_Q$ | Maximum allowed value for Q in the current routine. |
| `MaOut` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Copy or shrink ACF values into initial MA coefficient guesses.

### aa_levinson_durbin

**Purpose**  
Implements levinson durbin within the autocorrelation and partial autocorrelation module.

**Signature**

```c
void aa_levinson_durbin(vars ACF,int Order,vars Phi,vars Work)
```

**Defined in**  
`src/litec/aa_arima_stats.c:76`

**Mathematical form**

$$
\phi_{k,k}=\frac{\rho_k-\sum_{j=1}^{k-1}\phi_{k-1,j}\rho_{k-j}}{1-\sum_{j=1}^{k-1}\phi_{k-1,j}\rho_j}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `ACF` | `vars` | $ACF$ | Vector input or output buffer used by the routine. |
| `Order` | `int` | $Order$ | Routine-specific input or output argument. |
| `Phi` | `vars` | $Phi$ | Vector input or output buffer used by the routine. |
| `Work` | `vars` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Iteratively update AR coefficients and prediction error variance; efficient for Toeplitz systems.

### aa_pacf

**Purpose**  
Implements pacf within the autocorrelation and partial autocorrelation module.

**Signature**

```c
void aa_pacf(vars Series,int N,int MaxLag,vars OutPACF)
```

**Defined in**  
`src/litec/aa_arima_stats.c:108`

**Mathematical form**

$$
\alpha_k=\phi_{k,k}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxLag` | `int` | $max_Lag$ | Maximum allowed value for Lag in the current routine. |
| `OutPACF` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Use Levinson-Durbin or repeated Yule-Walker fits; output diagonal AR coefficient for each lag.

### aa_pacf_cutoff_lag

**Purpose**  
Implements pacf cutoff lag within the autocorrelation and partial autocorrelation module.

**Signature**

```c
int aa_pacf_cutoff_lag(vars PACF,int MaxLag,var Threshold)
```

**Defined in**  
`src/litec/aa_arima_stats.c:151`

**Mathematical form**

$$
$k^\*=\min\{k:
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `PACF` | `vars` | $PACF$ | Vector input or output buffer used by the routine. |
| `MaxLag` | `int` | $max_Lag$ | Maximum allowed value for Lag in the current routine. |
| `Threshold` | `var` | $\tau$ | Decision threshold used for classification or trading logic. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
\alpha_k

### aa_yule_walker

**Purpose**  
Implements yule walker within the autocorrelation and partial autocorrelation module.

**Signature**

```c
void aa_yule_walker(vars Series,int N,int Order,vars Phi)
```

**Defined in**  
`src/litec/aa_arima_stats.c:165`

**Mathematical form**

$$
\Gamma_p\phi=\gamma_p
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Order` | `int` | $Order$ | Routine-specific input or output argument. |
| `Phi` | `vars` | $Phi$ | Vector input or output buffer used by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Build Toeplitz autocovariance matrix from `gamma_k`; solve for AR coefficients.

## 7. AR / MA Validity and Stability

**Optimization requirement:** reuse polynomial/root helpers after they exist. Until root solving is implemented, use coefficient clamps only as a fallback, not as the final stationarity/invertibility test.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_ar_root_modulus

**Purpose**  
Implements ar root modulus within the ar / ma validity and stability module.

**Signature**

```c
var aa_ar_root_modulus(vars Ar,int P)
```

**Defined in**  
`src/litec/aa_arima_stats.c:222`

**Mathematical form**

$$
$\min
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Ar` | `vars` | $Ar$ | Vector input or output buffer used by the routine. |
| `P` | `int` | $p$ | Non-seasonal autoregressive order. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
z_i

### aa_clamp_coefficients

**Purpose**  
Implements clamp coefficients within the ar / ma validity and stability module.

**Signature**

```c
void aa_clamp_coefficients(vars Coeff,int N,var MinValue,var MaxValue)
```

**Defined in**  
`src/litec/aa_arima_stats.c:300`

**Mathematical form**

$$
c_i\leftarrow \min(\max(c_i,L),U)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Coeff` | `vars` | $Coeff$ | Vector input or output buffer used by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MinValue` | `var` | $MinValue$ | Routine-specific input or output argument. |
| `MaxValue` | `var` | $max_Value$ | Maximum allowed value for Value in the current routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Loop and clamp each coefficient. This is a simple safety fallback, not a full root check.

### aa_coefficients_are_valid

**Purpose**  
Implements coefficients are valid within the ar / ma validity and stability module.

**Signature**

```c
int aa_coefficients_are_valid(vars Coeff,int N)
```

**Defined in**  
`src/litec/aa_arima_stats.c:311`

**Mathematical form**

$$
I(\forall i:\ c_i\in\mathbb R)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Coeff` | `vars` | $Coeff$ | Vector input or output buffer used by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Check invalid/infinite values and optional maximum absolute bound.

### aa_enforce_invertibility

**Purpose**  
Implements enforce invertibility within the ar / ma validity and stability module.

**Signature**

```c
void aa_enforce_invertibility(vars Ma,int Q)
```

**Defined in**  
`src/litec/aa_arima_stats.c:284`

**Mathematical form**

$$
\vartheta \leftarrow \Pi_{\mathcal I}(\vartheta)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Ma` | `vars` | $Ma$ | Vector input or output buffer used by the routine. |
| `Q` | `int` | $q$ | Non-seasonal moving-average order. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
If non-invertible, shrink/reflection logic similar to AR.

### aa_enforce_stationarity

**Purpose**  
Implements enforce stationarity within the ar / ma validity and stability module.

**Signature**

```c
void aa_enforce_stationarity(vars Ar,int P)
```

**Defined in**  
`src/litec/aa_arima_stats.c:268`

**Mathematical form**

$$
\phi \leftarrow \Pi_{\mathcal S}(\phi)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Ar` | `vars` | $Ar$ | Vector input or output buffer used by the routine. |
| `P` | `int` | $p$ | Non-seasonal autoregressive order. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
If unstable, shrink coefficients, reflect roots, or repeatedly multiply coefficients by 0.95 until stable.

### aa_is_invertible_ma

**Purpose**  
Implements is invertible ma within the ar / ma validity and stability module.

**Signature**

```c
int aa_is_invertible_ma(vars Ma,int Q)
```

**Defined in**  
`src/litec/aa_arima_stats.c:260`

**Mathematical form**

$$
$\min_i
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Ma` | `vars` | $Ma$ | Vector input or output buffer used by the routine. |
| `Q` | `int` | $q$ | Non-seasonal moving-average order. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
z_i

### aa_is_stationary_ar

**Purpose**  
Implements is stationary ar within the ar / ma validity and stability module.

**Signature**

```c
int aa_is_stationary_ar(vars Ar,int P)
```

**Defined in**  
`src/litec/aa_arima_stats.c:252`

**Mathematical form**

$$
$\min_i
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Ar` | `vars` | $Ar$ | Vector input or output buffer used by the routine. |
| `P` | `int` | $p$ | Non-seasonal autoregressive order. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
z_i

### aa_ma_root_modulus

**Purpose**  
Implements ma root modulus within the ar / ma validity and stability module.

**Signature**

```c
var aa_ma_root_modulus(vars Ma,int Q)
```

**Defined in**  
`src/litec/aa_arima_stats.c:237`

**Mathematical form**

$$
$\min
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Ma` | `vars` | $Ma$ | Vector input or output buffer used by the routine. |
| `Q` | `int` | $q$ | Non-seasonal moving-average order. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
z_i

### aa_min_root_modulus

**Purpose**  
Implements min root modulus within the ar / ma validity and stability module.

**Signature**

```c
var aa_min_root_modulus(vars Coeff,int N)
```

**Defined in**  
`src/litec/aa_arima_stats.c:325`

**Mathematical form**

$$
$r_{\min}=\min_i
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Coeff` | `vars` | $Coeff$ | Vector input or output buffer used by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
z_i

## 8. ARMA / ARIMA / SARIMA Fitting Functions

**Optimization requirement:** all fitting functions must accept `ARIMA_WORK* Work`; no fitting routine may allocate inside the model-search loop. ARIMA and SARIMA fitting must reuse the ARMA residual recursion instead of duplicating it.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_arima_fit

**Purpose**  
Implements arima fit within the arma / arima / sarima fitting functions module.

**Signature**

```c
int aa_arima_fit(int P,int D,int Q,vars Close,int N,ARIMA_WORK* Work,ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_fit.c:104`

**Mathematical form**

$$
z_t=(1-B)^d y_t,\quad z_t\sim\operatorname{ARMA}(p,q)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `P` | `int` | $p$ | Non-seasonal autoregressive order. |
| `D` | `int` | $d$ | Non-seasonal differencing order. |
| `Q` | `int` | $q$ | Non-seasonal moving-average order. |
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Difference `Close` by `D`, call ARMA fit on `z`, then store orders, coefficients, residuals, and scores in `Model`.

### aa_arma_fit

**Purpose**  
Implements arma fit within the arma / arima / sarima fitting functions module.

**Signature**

```c
int aa_arma_fit(int P,int Q,vars Series,int N,var* OutSSE,var* OutC,vars ArOut,vars MaOut)
```

**Defined in**  
`src/litec/aa_arima_fit.c:40`

**Mathematical form**

$$
y_t=c+\sum_{i=1}^{p}\phi_i y_{t-i}+\sum_{j=1}^{q}\vartheta_j\epsilon_{t-j}+\epsilon_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `P` | `int` | $p$ | Non-seasonal autoregressive order. |
| `Q` | `int` | $q$ | Non-seasonal moving-average order. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `OutSSE` | `var*` | $z$ | Output location populated by the routine. |
| `OutC` | `var*` | $z$ | Output location populated by the routine. |
| `ArOut` | `vars` | $z$ | Output location populated by the routine. |
| `MaOut` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Fit ARMA on an already stationary chronological series. Optimize `c`, AR, and MA coefficients; output SSE and coefficients.

### aa_css_fit

**Purpose**  
Implements css fit within the arma / arima / sarima fitting functions module.

**Signature**

```c
int aa_css_fit(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_fit.c:190`

**Mathematical form**

$$
CSS(\theta)=\sum_{t=t_0}^{N-1}\epsilon_t(\theta)^2
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Given model orders, recursively compute residuals and minimize conditional sum of squares.

### aa_exact_mle_fit

**Purpose**  
Implements exact mle fit within the arma / arima / sarima fitting functions module.

**Signature**

```c
int aa_exact_mle_fit(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:366`

**Mathematical form**

$$
\theta^\*=\arg\max_\theta \ell_{\mathrm{exact}}(\theta)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Use Kalman/filter or exact innovations likelihood. Best implemented after matrix helpers are built.

### aa_kalman_loglikelihood

**Purpose**  
Implements kalman loglikelihood within the arma / arima / sarima fitting functions module.

**Signature**

```c
var aa_kalman_loglikelihood(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_objective.c:139`

**Mathematical form**

$$
\ell=-\frac{1}{2}\sum_t\left[\log(2\pi F_t)+\frac{v_t^2}{F_t}\right]
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Represent ARIMA in state-space form; recursively update innovation `v_t` and variance `F_t`.

### aa_mle_fit

**Purpose**  
Implements mle fit within the arma / arima / sarima fitting functions module.

**Signature**

```c
int aa_mle_fit(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:358`

**Mathematical form**

$$
\ell(\theta)=-\frac{N}{2}\left[\log(2\pi)+\log(\sigma^2)+1\right]
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Use residual variance from CSS or approximate likelihood; optimize parameters to maximize log-likelihood.

### aa_sarima_fit

**Purpose**  
Implements sarima fit within the arma / arima / sarima fitting functions module.

**Signature**

```c
int aa_sarima_fit(int P,int D,int Q,int SP,int SD,int SQ,int M,vars Close,int N,ARIMA_WORK* Work,ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_fit.c:138`

**Mathematical form**

$$
\Phi(B^m)\phi(B)(1-B)^d(1-B^m)^D y_t=c+\Theta(B^m)\vartheta(B)\epsilon_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `P` | `int` | $p$ | Non-seasonal autoregressive order. |
| `D` | `int` | $d$ | Non-seasonal differencing order. |
| `Q` | `int` | $q$ | Non-seasonal moving-average order. |
| `SP` | `int` | $P$ | Seasonal autoregressive order P. |
| `SD` | `int` | $D$ | Seasonal differencing order D. |
| `SQ` | `int` | $Q$ | Seasonal moving-average order Q. |
| `M` | `int` | $m$ | Seasonal period. |
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Apply ordinary and seasonal differencing, then fit ARMA with ordinary and seasonal lag terms.

## 9. Optimizer Functions

**Optimization requirement:** optimizer state arrays, gradients, Hessians, and step buffers must be preallocated in `ARIMA_WORK`. Reuse gradient and likelihood functions across Adam, BFGS, L-BFGS, and Nelder-Mead.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_adam_fit

**Purpose**  
Implements adam fit within the optimizer functions module.

**Signature**

```c
int aa_adam_fit(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:185`

**Mathematical form**

$$
m_k=\beta_1m_{k-1}+(1-\beta_1)g_k,\quad v_k=\beta_2v_{k-1}+(1-\beta_2)g_k^2,\quad \theta_{k+1}=\theta_k-\eta\frac{\hat m_k}{\sqrt{\hat v_k}+\epsilon}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Reuse the optimizer already present in your code, but expose it as a separate module.

### aa_bfgs_fit

**Purpose**  
Implements bfgs fit within the optimizer functions module.

**Signature**

```c
int aa_bfgs_fit(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:230`

**Mathematical form**

$$
H_{k+1}=(I-\rho s y^\top)H_k(I-\rho y s^\top)+\rho ss^\top
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Maintain inverse Hessian approximation; use line search and gradient vector.

### aa_check_convergence

**Purpose**  
Implements check convergence within the optimizer functions module.

**Signature**

```c
int aa_check_convergence(var PrevScore,var NewScore,vars Grad,int GradN,var Tolerance)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:109`

**Mathematical form**

$$

$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `PrevScore` | `var` | $PrevScore$ | Routine-specific input or output argument. |
| `NewScore` | `var` | $NewScore$ | Routine-specific input or output argument. |
| `Grad` | `vars` | $Grad$ | Vector input or output buffer used by the routine. |
| `GradN` | `int` | $GradN$ | Routine-specific input or output argument. |
| `Tolerance` | `var` | $Tolerance$ | Routine-specific input or output argument. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
\Delta S

### aa_compute_gradient

**Purpose**  
Implements compute gradient within the optimizer functions module.

**Signature**

```c
void aa_compute_gradient(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work,vars Grad)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:33`

**Mathematical form**

$$
g_i=\frac{\partial L}{\partial\theta_i}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Grad` | `vars` | $Grad$ | Vector input or output buffer used by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Compute analytic ARMA gradients or finite differences if analytic version is unavailable.

### aa_compute_hessian

**Purpose**  
Implements compute hessian within the optimizer functions module.

**Signature**

```c
void aa_compute_hessian(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work,vars Hessian)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:58`

**Mathematical form**

$$
H_{ij}=\frac{\partial^2L}{\partial\theta_i\partial\theta_j}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Hessian` | `vars` | $Hessian$ | Vector input or output buffer used by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Use finite differences of gradients or direct second derivatives. Store in row-major work matrix.

### aa_gradient_descent_fit

**Purpose**  
Implements gradient descent fit within the optimizer functions module.

**Signature**

```c
int aa_gradient_descent_fit(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:154`

**Mathematical form**

$$
\theta_{k+1}=\theta_k-\eta\nabla L(\theta_k)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Pack parameters into a vector, compute gradients, update until convergence.

### aa_gradient_norm

**Purpose**  
Implements gradient norm within the optimizer functions module.

**Signature**

```c
var aa_gradient_norm(vars Grad,int N)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:94`

**Mathematical form**

$$
$\
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Grad` | `vars` | $Grad$ | Vector input or output buffer used by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
g\

### aa_lbfgs_fit

**Purpose**  
Implements lbfgs fit within the optimizer functions module.

**Signature**

```c
int aa_lbfgs_fit(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:262`

**Mathematical form**

$$
\theta_{k+1}=\theta_k-\alpha H_k g_k,\quad H_k\text{ from last }m\text{ }(s,y)\text{ pairs}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Store only limited history of parameter/gradient differences to reduce memory.

### aa_nelder_mead_fit

**Purpose**  
Implements nelder mead fit within the optimizer functions module.

**Signature**

```c
int aa_nelder_mead_fit(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:294`

**Mathematical form**

$$
x_r=\bar x+\alpha(\bar x-x_h)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Maintain simplex of parameter vectors; reflect, expand, contract, or shrink based on objective values.

### aa_optimizer_report

**Purpose**  
Implements optimizer report within the optimizer functions module.

**Signature**

```c
void aa_optimizer_report(ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:120`

**Mathematical form**

$$
$R=\{\text{iterations},\text{converged},\text{score},\
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
g\

### aa_set_optimizer_defaults

**Purpose**  
Implements set optimizer defaults within the optimizer functions module.

**Signature**

```c
void aa_set_optimizer_defaults()
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:132`

**Mathematical form**

$$
\eta,\tau,K_{\max}\leftarrow\text{defaults}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| — | — | — | This routine has no explicit parameters. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Set global/static optimizer parameters to safe defaults.

### aa_set_optimizer_learning_rate

**Purpose**  
Implements set optimizer learning rate within the optimizer functions module.

**Signature**

```c
void aa_set_optimizer_learning_rate(var Eta)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:149`

**Mathematical form**

$$
\eta\leftarrow \eta_0
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Eta` | `var` | $Eta$ | Routine-specific input or output argument. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Set learning rate for gradient/Adam optimizers.

### aa_set_optimizer_max_iter

**Purpose**  
Implements set optimizer max iter within the optimizer functions module.

**Signature**

```c
void aa_set_optimizer_max_iter(int MaxIter)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:139`

**Mathematical form**

$$
K_{\max}\leftarrow K
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `MaxIter` | `int` | $max_Iter$ | Maximum allowed value for Iter in the current routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Set a global/static maximum iteration count.

### aa_set_optimizer_tolerance

**Purpose**  
Implements set optimizer tolerance within the optimizer functions module.

**Signature**

```c
void aa_set_optimizer_tolerance(var Tolerance)
```

**Defined in**  
`src/litec/aa_arima_optimizers.c:144`

**Mathematical form**

$$
\tau\leftarrow T
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Tolerance` | `var` | $Tolerance$ | Routine-specific input or output argument. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Set convergence tolerance.

## 10. Likelihood and Model Scoring

**Optimization requirement:** scoring functions must be allocation-free, pure calculations on already-computed residual/SSE/log-likelihood values. Reuse the same parameter-count function for AIC, AICc, BIC, and HQIC.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_aic_score

**Purpose**  
Implements aic score within the likelihood and model scoring module.

**Signature**

```c
var aa_aic_score(int N,var LogLik,int K)
```

**Defined in**  
`src/litec/aa_arima_stats.c:544`

**Mathematical form**

$$
AIC=2K-2\ell
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `LogLik` | `var` | $LogLik$ | Routine-specific input or output argument. |
| `K` | `int` | $K$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Return `2*K - 2*LogLik`.

### aa_aicc_score

**Purpose**  
Implements aicc score within the likelihood and model scoring module.

**Signature**

```c
var aa_aicc_score(int N,var SSE,int P,int Q)
```

**Defined in**  
`src/litec/aa_arima_stats.c:549`

**Mathematical form**

$$
AICc=N\log(SSE/N)+2K+\frac{2K(K+1)}{N-K-1}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `SSE` | `var` | $SSE$ | Routine-specific input or output argument. |
| `P` | `int` | $p$ | Non-seasonal autoregressive order. |
| `Q` | `int` | $q$ | Non-seasonal moving-average order. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Current SSE-based AICc for ARMA; guard `N-K-1 <= 0` and `SSE <= 0`.

### aa_aicc_score_general

**Purpose**  
Implements aicc score general within the likelihood and model scoring module.

**Signature**

```c
var aa_aicc_score_general(int N,var LogLik,int K)
```

**Defined in**  
`src/litec/aa_arima_stats.c:568`

**Mathematical form**

$$
AICc=AIC+\frac{2K(K+1)}{N-K-1}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `LogLik` | `var` | $LogLik$ | Routine-specific input or output argument. |
| `K` | `int` | $K$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compute AIC from likelihood, then small-sample correction.

### aa_bic_score

**Purpose**  
Implements bic score within the likelihood and model scoring module.

**Signature**

```c
var aa_bic_score(int N,var LogLik,int K)
```

**Defined in**  
`src/litec/aa_arima_stats.c:579`

**Mathematical form**

$$
BIC=K\log N-2\ell
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `LogLik` | `var` | $LogLik$ | Routine-specific input or output argument. |
| `K` | `int` | $K$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Return `K*log(N)-2*LogLik`.

### aa_compare_ic

**Purpose**  
Implements compare ic within the likelihood and model scoring module.

**Signature**

```c
int aa_compare_ic(var ScoreA,var ScoreB)
```

**Defined in**  
`src/litec/aa_arima_stats.c:610`

**Mathematical form**

$$
I(S_A<S_B)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `ScoreA` | `var` | $ScoreA$ | Routine-specific input or output argument. |
| `ScoreB` | `var` | $ScoreB$ | Routine-specific input or output argument. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Return whether candidate A is better than B; lower IC is better.

### aa_conditional_loglikelihood

**Purpose**  
Implements conditional loglikelihood within the likelihood and model scoring module.

**Signature**

```c
var aa_conditional_loglikelihood(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_objective.c:144`

**Mathematical form**

$$
$\ell_c(\theta)=\sum_{t=t_0}^{N-1}\log f(y_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
\mathcal F_{t-1};\theta)$

### aa_exact_loglikelihood

**Purpose**  
Implements exact loglikelihood within the likelihood and model scoring module.

**Signature**

```c
var aa_exact_loglikelihood(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_objective.c:149`

**Mathematical form**

$$
\ell_e(\theta)=\sum_t \log f(v_t,F_t)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Use Kalman innovations or exact state-space likelihood.

### aa_hqic_score

**Purpose**  
Implements hqic score within the likelihood and model scoring module.

**Signature**

```c
var aa_hqic_score(int N,var LogLik,int K)
```

**Defined in**  
`src/litec/aa_arima_stats.c:587`

**Mathematical form**

$$
HQIC=2K\log(\log N)-2\ell
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `LogLik` | `var` | $LogLik$ | Routine-specific input or output argument. |
| `K` | `int` | $K$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Return Hannan-Quinn information criterion.

### aa_loglikelihood

**Purpose**  
Implements loglikelihood within the likelihood and model scoring module.

**Signature**

```c
var aa_loglikelihood(vars Residuals,int N,var Sigma2)
```

**Defined in**  
`src/litec/aa_arima_stats.c:524`

**Mathematical form**

$$
\ell=-\frac{N}{2}\left[\log(2\pi)+\log(\sigma^2)\right]-\frac{1}{2\sigma^2}\sum_t e_t^2
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Sigma2` | `var` | $Sigma2$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compute Gaussian residual log-likelihood from residuals and variance.

### aa_model_score

**Purpose**  
Implements model score within the likelihood and model scoring module.

**Signature**

```c
var aa_model_score(ARIMA_MODEL* Model,int ScoreMode)
```

**Defined in**  
`src/litec/aa_arima_stats.c:595`

**Mathematical form**

$$
S(M)=\begin{cases}AIC,AICc,BIC,HQIC\end{cases}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `ScoreMode` | `int` | $m_{score}$ | Information-criterion or score-selection mode. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Switch on `ScoreMode` and return the chosen criterion.

## 11. AutoARIMA Search Functions

**Optimization requirement:** search functions must reuse candidate buffers and cached model scores. Stepwise search should avoid full grid search unless explicitly requested.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_auto_arima_search

**Purpose**  
Implements auto arima search within the autoarima search functions module.

**Signature**

```c
int aa_auto_arima_search(vars closeSeries,int sampleCount,int maxArOrder,int maxDiffOrder,int maxMaOrder,int scoreMode,ARIMA_WORK* work,ARIMA_MODEL* bestModel)
```

**Defined in**  
`src/litec/aa_arima_search.c:69`

**Mathematical form**

$$
(p^*,d^*,q^*) = \arg\min_{(p,d,q)\in\mathcal{G}} IC(p,d,q)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `closeSeries` | `vars` | $y_t$ | Input newest-first series values used by the computation. |
| `sampleCount` | `int` | $sampleCount$ | Routine-specific input or output argument. |
| `maxArOrder` | `int` | $max_ArOrder$ | Maximum allowed value for ArOrder in the current routine. |
| `maxDiffOrder` | `int` | $max_DiffOrder$ | Maximum allowed value for DiffOrder in the current routine. |
| `maxMaOrder` | `int` | $max_MaOrder$ | Maximum allowed value for MaOrder in the current routine. |
| `scoreMode` | `int` | $scoreMode$ | Routine-specific input or output argument. |
| `work` | `ARIMA_WORK*` | $z$ | Pointer argument passed by reference. |
| `bestModel` | `ARIMA_MODEL*` | $z$ | Pointer argument passed by reference. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Enumerate candidate non-seasonal ARIMA orders, reuse aa_arima_fit() for fitting, score each candidate with the selected information criterion, and retain the best-scoring model.

### aa_candidate_exists

**Purpose**  
Implements candidate exists within the autoarima search functions module.

**Signature**

```c
int aa_candidate_exists(ARIMA_CANDIDATE* List,int Count,ARIMA_CANDIDATE* Candidate)
```

**Defined in**  
`src/litec/aa_arima_search.c:117`

**Mathematical form**

$$
I(\exists i:C_i=C)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `List` | `ARIMA_CANDIDATE*` | $z$ | Pointer argument passed by reference. |
| `Count` | `int` | $n$ | Number of elements to allocate or process. |
| `Candidate` | `ARIMA_CANDIDATE*` | $C$ | Candidate model structure under evaluation. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Loop candidate list and compare all order fields.

### aa_expand_candidate_models

**Purpose**  
Implements expand candidate models within the autoarima search functions module.

**Signature**

```c
void aa_expand_candidate_models(ARIMA_CANDIDATE* Current,ARIMA_CANDIDATE* List,int* Count)
```

**Defined in**  
`src/litec/aa_arima_search.c:138`

**Mathematical form**

$$
\mathcal C\leftarrow\mathcal C\cup\mathcal N(C)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Current` | `ARIMA_CANDIDATE*` | $z$ | Pointer argument passed by reference. |
| `List` | `ARIMA_CANDIDATE*` | $z$ | Pointer argument passed by reference. |
| `Count` | `int*` | $n$ | Number of elements to allocate or process. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Add nearby order combinations, skipping duplicates and invalid candidates.

### aa_fallback_model

**Purpose**  
Implements fallback model within the autoarima search functions module.

**Signature**

```c
int aa_fallback_model(vars Close,int N,ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_search.c:179`

**Mathematical form**

$$
\hat y_{T+1}=y_T\ \text{or}\ \bar y
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
When all fits fail, build a naive model so the strategy still has a safe forecast.

### aa_grid_search_arima

**Purpose**  
Implements grid search arima within the autoarima search functions module.

**Signature**

```c
int aa_grid_search_arima(vars Close,int N,int MaxP,int MaxD,int MaxQ,int ScoreMode,ARIMA_WORK* Work,ARIMA_MODEL* BestModel)
```

**Defined in**  
`src/litec/aa_arima_search.c:46`

**Mathematical form**

$$
M^\*=\arg\min_{p,d,q} IC(M_{p,d,q})
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxP` | `int` | $max_P$ | Maximum allowed value for P in the current routine. |
| `MaxD` | `int` | $max_D$ | Maximum allowed value for D in the current routine. |
| `MaxQ` | `int` | $max_Q$ | Maximum allowed value for Q in the current routine. |
| `ScoreMode` | `int` | $m_{score}$ | Information-criterion or score-selection mode. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `BestModel` | `ARIMA_MODEL*` | $M^*$ | Best model found so far. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Loop all orders within limits, fit each model, score it, copy best to `BestModel`.

### aa_init_candidate

**Purpose**  
Implements init candidate within the autoarima search functions module.

**Signature**

```c
void aa_init_candidate(ARIMA_CANDIDATE* Candidate)
```

**Defined in**  
`src/litec/aa_arima_search.c:79`

**Mathematical form**

$$
C\leftarrow0,\quad S\leftarrow\infty
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Candidate` | `ARIMA_CANDIDATE*` | $C$ | Candidate model structure under evaluation. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Zero orders and set score to a large value.

### aa_select_best_model

**Purpose**  
Implements select best model within the autoarima search functions module.

**Signature**

```c
int aa_select_best_model(ARIMA_CANDIDATE* List,int Count)
```

**Defined in**  
`src/litec/aa_arima_search.c:163`

**Mathematical form**

$$
i^\*=\arg\min_i S_i
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `List` | `ARIMA_CANDIDATE*` | $z$ | Pointer argument passed by reference. |
| `Count` | `int` | $n$ | Number of elements to allocate or process. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Loop candidates and return index of lowest score among valid converged models.

### aa_set_candidate

**Purpose**  
Implements set candidate within the autoarima search functions module.

**Signature**

```c
void aa_set_candidate(ARIMA_CANDIDATE* Candidate,int P,int D,int Q,int SP,int SD,int SQ,int M)
```

**Defined in**  
`src/litec/aa_arima_search.c:88`

**Mathematical form**

$$
C=(p,d,q,P,D,Q,m)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Candidate` | `ARIMA_CANDIDATE*` | $C$ | Candidate model structure under evaluation. |
| `P` | `int` | $p$ | Non-seasonal autoregressive order. |
| `D` | `int` | $d$ | Non-seasonal differencing order. |
| `Q` | `int` | $q$ | Non-seasonal moving-average order. |
| `SP` | `int` | $P$ | Seasonal autoregressive order P. |
| `SD` | `int` | $D$ | Seasonal differencing order D. |
| `SQ` | `int` | $Q$ | Seasonal moving-average order Q. |
| `M` | `int` | $m$ | Seasonal period. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Assign all order fields and reset score/status.

### aa_stepwise_auto_arima

**Purpose**  
Implements stepwise auto arima within the autoarima search functions module.

**Signature**

```c
int aa_stepwise_auto_arima(vars Close,int N,int MaxP,int MaxD,int MaxQ,int ScoreMode,ARIMA_WORK* Work,ARIMA_MODEL* BestModel)
```

**Defined in**  
`src/litec/aa_arima_search.c:74`

**Mathematical form**

$$
M_{k+1}=\arg\min_{M\in\mathcal N(M_k)} IC(M)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxP` | `int` | $max_P$ | Maximum allowed value for P in the current routine. |
| `MaxD` | `int` | $max_D$ | Maximum allowed value for D in the current routine. |
| `MaxQ` | `int` | $max_Q$ | Maximum allowed value for Q in the current routine. |
| `ScoreMode` | `int` | $m_{score}$ | Information-criterion or score-selection mode. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `BestModel` | `ARIMA_MODEL*` | $M^*$ | Best model found so far. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Start from a simple model, evaluate neighbors, move to better model until no improvement.

### aa_try_neighbor_models

**Purpose**  
Implements try neighbor models within the autoarima search functions module.

**Signature**

```c
void aa_try_neighbor_models(ARIMA_CANDIDATE* Current,ARIMA_CANDIDATE* List,int* Count)
```

**Defined in**  
`src/litec/aa_arima_search.c:158`

**Mathematical form**

$$
\mathcal N(C)=\{C\pm e_p,C\pm e_q,C\pm e_P,C\pm e_Q,\ldots\}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Current` | `ARIMA_CANDIDATE*` | $z$ | Pointer argument passed by reference. |
| `List` | `ARIMA_CANDIDATE*` | $z$ | Pointer argument passed by reference. |
| `Count` | `int*` | $n$ | Number of elements to allocate or process. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Generate and evaluate local neighbor models around current best.

### aa_validate_candidate_model

**Purpose**  
Implements validate candidate model within the autoarima search functions module.

**Signature**

```c
int aa_validate_candidate_model(ARIMA_CANDIDATE* Candidate)
```

**Defined in**  
`src/litec/aa_arima_search.c:104`

**Mathematical form**

$$
0\le p\le p_{\max},\ 0\le d\le d_{\max},\ldots
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Candidate` | `ARIMA_CANDIDATE*` | $C$ | Candidate model structure under evaluation. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Check order bounds and sample-size feasibility.

## 12. Seasonal ARIMA / SARIMA Functions

**Optimization requirement:** seasonal functions must reuse ordinary differencing, residual recursion, scoring, and search helpers wherever possible. Only seasonal lag construction should be new.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_auto_sarima_search

**Purpose**  
Implements auto sarima search within the seasonal arima / sarima functions module.

**Signature**

```c
int aa_auto_sarima_search(vars Close,int N,int MaxP,int MaxD,int MaxQ,int MaxSP,int MaxSD,int MaxSQ,int M,int ScoreMode,ARIMA_WORK* Work,ARIMA_MODEL* BestModel)
```

**Defined in**  
`src/litec/aa_arima_search.c:246`

**Mathematical form**

$$
M^\*=\arg\min_{p,d,q,P,D,Q}IC(M)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxP` | `int` | $max_P$ | Maximum allowed value for P in the current routine. |
| `MaxD` | `int` | $max_D$ | Maximum allowed value for D in the current routine. |
| `MaxQ` | `int` | $max_Q$ | Maximum allowed value for Q in the current routine. |
| `MaxSP` | `int` | $max_SP$ | Maximum allowed value for SP in the current routine. |
| `MaxSD` | `int` | $max_SD$ | Maximum allowed value for SD in the current routine. |
| `MaxSQ` | `int` | $max_SQ$ | Maximum allowed value for SQ in the current routine. |
| `M` | `int` | $m$ | Seasonal period. |
| `ScoreMode` | `int` | $m_{score}$ | Information-criterion or score-selection mode. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `BestModel` | `ARIMA_MODEL*` | $M^*$ | Best model found so far. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Grid-search ordinary and seasonal orders; expensive, so use work-memory and strict max orders.

### aa_detect_seasonal_period

**Purpose**  
Implements detect seasonal period within the seasonal arima / sarima functions module.

**Signature**

```c
int aa_detect_seasonal_period(vars Series,int N,int MinM,int MaxM)
```

**Defined in**  
`src/litec/aa_arima_search.c:191`

**Mathematical form**

$$
$m^\*=\arg\max_{m\in[M_{\min},M_{\max}]}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MinM` | `int` | $MinM$ | Routine-specific input or output argument. |
| `MaxM` | `int` | $max_M$ | Maximum allowed value for M in the current routine. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
\rho_m

### aa_sarima_forecast_multi_step

**Purpose**  
Implements sarima forecast multi step within the seasonal arima / sarima functions module.

**Signature**

```c
int aa_sarima_forecast_multi_step(ARIMA_MODEL* Model,vars Close,int N,int H,ARIMA_WORK* Work,vars ForecastPath)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:182`

**Mathematical form**

$$
$\hat y_{T+h}=E[y_{T+h}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `ForecastPath` | `vars` | $\hat y_{T+h}$ | Vector of multi-step forecasts. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
\mathcal F_T,M],\quad h=1,\ldots,H$

### aa_sarima_forecast_one_step

**Purpose**  
Implements sarima forecast one step within the seasonal arima / sarima functions module.

**Signature**

```c
int aa_sarima_forecast_one_step(ARIMA_MODEL* Model,vars Close,int N,ARIMA_WORK* Work,var* Forecast)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:173`

**Mathematical form**

$$
$\hat y_{T+1}=E[y_{T+1}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Forecast` | `var*` | $\hat y_h$ | Forecast value or one-step forecast input. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
\mathcal F_T,M]$

### aa_seasonal_acf_score

**Purpose**  
Implements seasonal acf score within the seasonal arima / sarima functions module.

**Signature**

```c
var aa_seasonal_acf_score(vars Series,int N,int M)
```

**Defined in**  
`src/litec/aa_arima_search.c:214`

**Mathematical form**

$$
$S_m=
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `M` | `int` | $m$ | Seasonal period. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
\rho_m

### aa_seasonal_strength

**Purpose**  
Implements seasonal strength within the seasonal arima / sarima functions module.

**Signature**

```c
var aa_seasonal_strength(vars Series,int N,int M)
```

**Defined in**  
`src/litec/aa_arima_search.c:219`

**Mathematical form**

$$
F_s=\max(0,1-\frac{\operatorname{Var}(R_t)}{\operatorname{Var}(S_t+R_t)})
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `M` | `int` | $m$ | Seasonal period. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Estimate seasonal component by period means; compare residual variance to seasonal+residual variance.

### aa_stepwise_auto_sarima

**Purpose**  
Implements stepwise auto sarima within the seasonal arima / sarima functions module.

**Signature**

```c
int aa_stepwise_auto_sarima(vars Close,int N,int MaxP,int MaxD,int MaxQ,int MaxSP,int MaxSD,int MaxSQ,int M,int ScoreMode,ARIMA_WORK* Work,ARIMA_MODEL* BestModel)
```

**Defined in**  
`src/litec/aa_arima_search.c:283`

**Mathematical form**

$$
M_{k+1}=\arg\min_{M\in\mathcal N_s(M_k)}IC(M)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxP` | `int` | $max_P$ | Maximum allowed value for P in the current routine. |
| `MaxD` | `int` | $max_D$ | Maximum allowed value for D in the current routine. |
| `MaxQ` | `int` | $max_Q$ | Maximum allowed value for Q in the current routine. |
| `MaxSP` | `int` | $max_SP$ | Maximum allowed value for SP in the current routine. |
| `MaxSD` | `int` | $max_SD$ | Maximum allowed value for SD in the current routine. |
| `MaxSQ` | `int` | $max_SQ$ | Maximum allowed value for SQ in the current routine. |
| `M` | `int` | $m$ | Seasonal period. |
| `ScoreMode` | `int` | $m_{score}$ | Information-criterion or score-selection mode. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `BestModel` | `ARIMA_MODEL*` | $M^*$ | Best model found so far. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Stepwise SARIMA search over ordinary and seasonal neighbors.

## 13. ARIMAX / SARIMAX Functions

**Optimization requirement:** regression and exogenous matrix buffers must be owned by `ARIMA_WORK`. Reuse existing matrix/regression helpers if added; avoid rebuilding design matrices when only the newest row changed.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_arimax_fit

**Purpose**  
Implements arimax fit within the arimax / sarimax functions module.

**Signature**

```c
int aa_arimax_fit(int P,int D,int Q,vars Close,vars* X,int N,int XCols,ARIMA_WORK* Work,ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_exogenous.c:109`

**Mathematical form**

$$
z_t=c+x_t^\top\beta+\sum_i\phi_i z_{t-i}+\sum_j\vartheta_j e_{t-j}+e_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `P` | `int` | $p$ | Non-seasonal autoregressive order. |
| `D` | `int` | $d$ | Non-seasonal differencing order. |
| `Q` | `int` | $q$ | Non-seasonal moving-average order. |
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `X` | `vars*` | $X_t$ | Exogenous regressor matrix or vector. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `XCols` | `int` | $k$ | Number of exogenous columns or regressors. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Difference target, include regressors in prediction equation, and optimize ARMA plus beta coefficients.

### aa_arimax_forecast

**Purpose**  
Implements arimax forecast within the arimax / sarimax functions module.

**Signature**

```c
int aa_arimax_forecast(ARIMA_MODEL* Model,vars Close,vars* FutureX,int N,int H,ARIMA_WORK* Work,vars ForecastPath)
```

**Defined in**  
`src/litec/aa_arima_exogenous.c:147`

**Mathematical form**

$$
\hat y_{T+h}=f(\mathcal F_T,X_{T+h};\hat\theta)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `FutureX` | `vars*` | $z$ | Vector input or output buffer used by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `ForecastPath` | `vars` | $\hat y_{T+h}$ | Vector of multi-step forecasts. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Require future exogenous values. Forecast differenced scale, then inverse-transform to price level.

### aa_prepare_exogenous_matrix

**Purpose**  
Implements prepare exogenous matrix within the arimax / sarimax functions module.

**Signature**

```c
int aa_prepare_exogenous_matrix(vars* X,int Rows,int Cols,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_exogenous.c:36`

**Mathematical form**

$$
X\in\mathbb R^{N\times k}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `X` | `vars*` | $X_t$ | Exogenous regressor matrix or vector. |
| `Rows` | `int` | $Rows$ | Routine-specific input or output argument. |
| `Cols` | `int` | $Cols$ | Routine-specific input or output argument. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Validate matrix pointers and optionally copy columns into contiguous work memory for faster loops.

### aa_regression_fit

**Purpose**  
Implements regression fit within the arimax / sarimax functions module.

**Signature**

```c
int aa_regression_fit(vars Y,vars* X,int Rows,int Cols,vars BetaOut)
```

**Defined in**  
`src/litec/aa_arima_exogenous.c:70`

**Mathematical form**

$$
\hat\beta=(X^\top X)^{-1}X^\top y
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Y` | `vars` | $Y$ | Vector input or output buffer used by the routine. |
| `X` | `vars*` | $X_t$ | Exogenous regressor matrix or vector. |
| `Rows` | `int` | $Rows$ | Routine-specific input or output argument. |
| `Cols` | `int` | $Cols$ | Routine-specific input or output argument. |
| `BetaOut` | `vars` | $\beta$ | Regression coefficient buffer. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Implement OLS via normal equations or QR-like solver. Store coefficients in `BetaOut`.

### aa_regression_predict

**Purpose**  
Implements regression predict within the arimax / sarimax functions module.

**Signature**

```c
var aa_regression_predict(vars XRow,vars Beta,int Cols)
```

**Defined in**  
`src/litec/aa_arima_exogenous.c:94`

**Mathematical form**

$$
\hat y=x^\top\hat\beta
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `XRow` | `vars` | $XRow$ | Vector input or output buffer used by the routine. |
| `Beta` | `vars` | $\beta$ | Regression coefficient vector or coefficient storage. |
| `Cols` | `int` | $Cols$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Loop over columns and compute dot product.

### aa_sarimax_fit

**Purpose**  
Implements sarimax fit within the arimax / sarimax functions module.

**Signature**

```c
int aa_sarimax_fit(int P,int D,int Q,int SP,int SD,int SQ,int M,vars Close,vars* X,int N,int XCols,ARIMA_WORK* Work,ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_exogenous.c:159`

**Mathematical form**

$$
\Phi(B^m)\phi(B)(1-B)^d(1-B^m)^D y_t=c+x_t^\top\beta+\Theta(B^m)\vartheta(B)e_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `P` | `int` | $p$ | Non-seasonal autoregressive order. |
| `D` | `int` | $d$ | Non-seasonal differencing order. |
| `Q` | `int` | $q$ | Non-seasonal moving-average order. |
| `SP` | `int` | $P$ | Seasonal autoregressive order P. |
| `SD` | `int` | $D$ | Seasonal differencing order D. |
| `SQ` | `int` | $Q$ | Seasonal moving-average order Q. |
| `M` | `int` | $m$ | Seasonal period. |
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `X` | `vars*` | $X_t$ | Exogenous regressor matrix or vector. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `XCols` | `int` | $k$ | Number of exogenous columns or regressors. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Combine SARIMA recursion with exogenous regression terms.

### aa_sarimax_forecast

**Purpose**  
Implements sarimax forecast within the arimax / sarimax functions module.

**Signature**

```c
int aa_sarimax_forecast(ARIMA_MODEL* Model,vars Close,vars* FutureX,int N,int H,ARIMA_WORK* Work,vars ForecastPath)
```

**Defined in**  
`src/litec/aa_arima_exogenous.c:171`

**Mathematical form**

$$
\hat y_{T+h}=f_s(\mathcal F_T,X_{T+h};\hat\theta)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `FutureX` | `vars*` | $z$ | Vector input or output buffer used by the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `ForecastPath` | `vars` | $\hat y_{T+h}$ | Vector of multi-step forecasts. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Forecast SARIMAX path using future exogenous data and seasonal recursion.

### aa_validate_exogenous_data

**Purpose**  
Implements validate exogenous data within the arimax / sarimax functions module.

**Signature**

```c
int aa_validate_exogenous_data(vars* X,int Rows,int Cols)
```

**Defined in**  
`src/litec/aa_arima_exogenous.c:54`

**Mathematical form**

$$
\forall t,j:\ X_{t,j}\in\mathbb R
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `X` | `vars*` | $X_t$ | Exogenous regressor matrix or vector. |
| `Rows` | `int` | $Rows$ | Routine-specific input or output argument. |
| `Cols` | `int` | $Cols$ | Routine-specific input or output argument. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Check all columns/rows for valid values and matching length.

## 14. Forecasting Functions

**Optimization requirement:** forecast functions must reuse fitted residuals, coefficients, and differencing state from `ARIMA_MODEL`/`ARIMA_WORK`. Multi-step forecasts should reuse one-step recursion logic in a loop.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_backtransform_forecast

**Purpose**  
Implements backtransform forecast within the forecasting functions module.

**Signature**

```c
void aa_backtransform_forecast(vars Forecast,int H,int TransformMode,var Lambda,vars Out)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:148`

**Mathematical form**

$$
\hat y=T^{-1}(\hat z)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Forecast` | `vars` | $\hat y_h$ | Forecast value or one-step forecast input. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `TransformMode` | `int` | $m_{transform}$ | Transformation/inversion mode selector. |
| `Lambda` | `var` | $\lambda$ | Box-Cox transformation parameter. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Switch on transform mode: none, log, Box-Cox, returns, etc.

### aa_bias_adjusted_backtransform

**Purpose**  
Implements bias adjusted backtransform within the forecasting functions module.

**Signature**

```c
void aa_bias_adjusted_backtransform(vars Forecast,vars SE,int H,int TransformMode,var Lambda,vars Out)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:158`

**Mathematical form**

$$
E[e^X]=e^{\mu+\frac{1}{2}\sigma^2}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Forecast` | `vars` | $\hat y_h$ | Forecast value or one-step forecast input. |
| `SE` | `vars` | $SE_h$ | Forecast standard error at the requested horizon. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `TransformMode` | `int` | $m_{transform}$ | Transformation/inversion mode selector. |
| `Lambda` | `var` | $\lambda$ | Box-Cox transformation parameter. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
For log transform, apply variance correction. For Box-Cox, use approximation or simulation-style correction.

### aa_forecast_bands

**Purpose**  
Implements forecast bands within the forecasting functions module.

**Signature**

```c
int aa_forecast_bands(ARIMA_MODEL* Model,vars ForecastPath,int H,var Alpha,ARIMA_WORK* Work,vars Lower,vars Upper)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:125`

**Mathematical form**

$$
L_h=\hat y_h-zSE_h,\quad U_h=\hat y_h+zSE_h
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `ForecastPath` | `vars` | $\hat y_{T+h}$ | Vector of multi-step forecasts. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `Alpha` | `var` | $\alpha$ | Tail probability or confidence significance level. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Lower` | `vars` | $L_h$ | Lower confidence or prediction bound written by reference. |
| `Upper` | `vars` | $U_h$ | Upper confidence or prediction bound written by reference. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Loop horizons, compute SE for each, and fill lower/upper arrays.

### aa_forecast_confidence_interval

**Purpose**  
Implements forecast confidence interval within the forecasting functions module.

**Signature**

```c
void aa_forecast_confidence_interval(var Forecast,var SE,var Alpha,var* Lower,var* Upper)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:112`

**Mathematical form**

$$
[\hat y_h-z_{1-\alpha/2}SE_h,\ \hat y_h+z_{1-\alpha/2}SE_h]
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Forecast` | `var` | $\hat y_h$ | Forecast value or one-step forecast input. |
| `SE` | `var` | $SE_h$ | Forecast standard error at the requested horizon. |
| `Alpha` | `var` | $\alpha$ | Tail probability or confidence significance level. |
| `Lower` | `var*` | $L_h$ | Lower confidence or prediction bound written by reference. |
| `Upper` | `var*` | $U_h$ | Upper confidence or prediction bound written by reference. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Compute z critical value, e.g. 1.96 for 95%; write lower/upper through pointers.

### aa_forecast_multi_step

**Purpose**  
Implements forecast multi step within the forecasting functions module.

**Signature**

```c
int aa_forecast_multi_step(ARIMA_MODEL* Model,vars Series,int N,int H,ARIMA_WORK* Work,vars ForecastPath)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:80`

**Mathematical form**

$$
\hat z_{T+h}=c+\sum_i\phi_i \tilde z_{T+h-i}+\sum_j\vartheta_j \tilde e_{T+h-j}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `ForecastPath` | `vars` | $\hat y_{T+h}$ | Vector of multi-step forecasts. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
For future lags use earlier forecasts; future residuals are normally set to zero.

### aa_forecast_one_step

**Purpose**  
Implements forecast one step within the forecasting functions module.

**Signature**

```c
var aa_forecast_one_step(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:43`

**Mathematical form**

$$
\hat z_{T+1}=c+\sum_i\phi_i z_{T+1-i}+\sum_j\vartheta_j e_{T+1-j}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compute one-step prediction on stationary/differenced scale, then integrate/back-transform when needed.

### aa_forecast_standard_error

**Purpose**  
Implements forecast standard error within the forecasting functions module.

**Signature**

```c
var aa_forecast_standard_error(ARIMA_MODEL* Model,int Horizon)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:107`

**Mathematical form**

$$
SE_h=\sqrt{\operatorname{Var}(\hat e_{T+h})}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Horizon` | `int` | $Horizon$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Return sqrt of forecast variance.

### aa_forecast_variance

**Purpose**  
Implements forecast variance within the forecasting functions module.

**Signature**

```c
var aa_forecast_variance(ARIMA_MODEL* Model,int Horizon)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:99`

**Mathematical form**

$$
\operatorname{Var}(\hat e_{T+h})=\sigma^2\sum_{i=0}^{h-1}\psi_i^2
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Horizon` | `int` | $Horizon$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compute psi-weights from ARMA representation; sum squared weights up to horizon.

### aa_integrate_forecast

**Purpose**  
Implements integrate forecast within the forecasting functions module.

**Signature**

```c
void aa_integrate_forecast(vars Original,int N,vars DiffForecast,int H,int D,vars Out)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:143`

**Mathematical form**

$$
\hat y=\operatorname{integrate}_d(\hat z)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Original` | `vars` | $y_t$ | Original non-differenced series used for inversion or back-transformation. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `DiffForecast` | `vars` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `D` | `int` | $d$ | Non-seasonal differencing order. |
| `Out` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Reverse ordinary differencing for each horizon using original history.

## 15. Residual Diagnostics

**Optimization requirement:** residual diagnostics must reuse residual mean, variance, and ACF buffers. Do not recompute residuals when the fitted model already has them stored.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_arch_lm_pvalue

**Purpose**  
Implements arch lm pvalue within the residual diagnostics module.

**Signature**

```c
var aa_arch_lm_pvalue(var Stat,int Df)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:259`

**Mathematical form**

$$
p=1-F_{\chi^2_L}(LM)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Stat` | `var` | $Stat$ | Routine-specific input or output argument. |
| `Df` | `int` | $Df$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Approximate chi-square survival with `Lag` degrees of freedom.

### aa_arch_lm_stat

**Purpose**  
Implements arch lm stat within the residual diagnostics module.

**Signature**

```c
var aa_arch_lm_stat(vars Residuals,int N,int Lag)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:190`

**Mathematical form**

$$
e_t^2=\alpha_0+\sum_{i=1}^{L}\alpha_i e_{t-i}^2+u_t,\quad LM=N R^2
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lag` | `int` | $Lag$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Regress squared residuals on lagged squared residuals; return `N*R2`.

### aa_arch_lm_test

**Purpose**  
Implements arch lm test within the residual diagnostics module.

**Signature**

```c
int aa_arch_lm_test(vars Residuals,int N,int Lag,var Alpha)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:264`

**Mathematical form**

$$
I(p>\alpha)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Lag` | `int` | $Lag$ | Routine-specific input or output argument. |
| `Alpha` | `var` | $\alpha$ | Tail probability or confidence significance level. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Return 1 when ARCH effect is not significant.

### aa_box_pierce_pvalue

**Purpose**  
Implements box pierce pvalue within the residual diagnostics module.

**Signature**

```c
var aa_box_pierce_pvalue(var Stat,int Df)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:124`

**Mathematical form**

$$
p=1-F_{\chi^2_{\nu}}(Q_{BP})
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Stat` | `var` | $Stat$ | Routine-specific input or output argument. |
| `Df` | `int` | $Df$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Use same chi-square survival helper as Ljung-Box.

### aa_box_pierce_stat

**Purpose**  
Implements box pierce stat within the residual diagnostics module.

**Signature**

```c
var aa_box_pierce_stat(vars Residuals,int N,int MaxLag)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:119`

**Mathematical form**

$$
Q_{BP}=N\sum_{k=1}^{h}\rho_k^2
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxLag` | `int` | $max_Lag$ | Maximum allowed value for Lag in the current routine. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Simpler portmanteau statistic using residual ACF.

### aa_box_pierce_test

**Purpose**  
Implements box pierce test within the residual diagnostics module.

**Signature**

```c
int aa_box_pierce_test(vars Residuals,int N,int MaxLag,var Alpha)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:129`

**Mathematical form**

$$
I(p>\alpha)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxLag` | `int` | $max_Lag$ | Maximum allowed value for Lag in the current routine. |
| `Alpha` | `var` | $\alpha$ | Tail probability or confidence significance level. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Return 1 when residual autocorrelation is not significant.

### aa_compute_residuals

**Purpose**  
Implements compute residuals within the residual diagnostics module.

**Signature**

```c
void aa_compute_residuals(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work,vars Residuals)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:78`

**Mathematical form**

$$
e_t=y_t-\hat y_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Run model recursion across history and write residuals.

### aa_diagnostic_report

**Purpose**  
Implements diagnostic report within the residual diagnostics module.

**Signature**

```c
void aa_diagnostic_report(ARIMA_MODEL* Model,vars Series,int N,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:279`

**Mathematical form**

$$
D=\{\bar e,s_e^2,\rho_k,Q,DW,JB,ARCH\}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Compute diagnostics and print/store summary fields.

### aa_durbin_watson_test

**Purpose**  
Implements durbin watson test within the residual diagnostics module.

**Signature**

```c
var aa_durbin_watson_test(vars Residuals,int N)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:134`

**Mathematical form**

$$
DW=\frac{\sum_{t=1}^{N-1}(e_t-e_{t-1})^2}{\sum_{t=0}^{N-1}e_t^2}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Loop once for numerator and denominator; guard denominator.

### aa_jarque_bera_pvalue

**Purpose**  
Implements jarque bera pvalue within the residual diagnostics module.

**Signature**

```c
var aa_jarque_bera_pvalue(var Stat)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:181`

**Mathematical form**

$$
p=1-F_{\chi^2_2}(JB)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Stat` | `var` | $Stat$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Approximate chi-square survival with 2 degrees of freedom.

### aa_jarque_bera_stat

**Purpose**  
Implements jarque bera stat within the residual diagnostics module.

**Signature**

```c
var aa_jarque_bera_stat(vars Residuals,int N)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:151`

**Mathematical form**

$$
JB=\frac{N}{6}\left(S^2+\frac{(K-3)^2}{4}\right)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compute residual skewness and kurtosis, then statistic.

### aa_jarque_bera_test

**Purpose**  
Implements jarque bera test within the residual diagnostics module.

**Signature**

```c
int aa_jarque_bera_test(vars Residuals,int N,var Alpha)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:185`

**Mathematical form**

$$
I(p>\alpha)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Alpha` | `var` | $\alpha$ | Tail probability or confidence significance level. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Return 1 when normality is not rejected.

### aa_ljung_box_pvalue

**Purpose**  
Implements ljung box pvalue within the residual diagnostics module.

**Signature**

```c
var aa_ljung_box_pvalue(var Stat,int Df)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:109`

**Mathematical form**

$$
p=1-F_{\chi^2_{\nu}}(Q)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Stat` | `var` | $Stat$ | Routine-specific input or output argument. |
| `Df` | `int` | $Df$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Approximate chi-square survival function. For lite-C, implement incomplete gamma approximation or table.

### aa_ljung_box_stat

**Purpose**  
Implements ljung box stat within the residual diagnostics module.

**Signature**

```c
var aa_ljung_box_stat(vars Residuals,int N,int MaxLag)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:104`

**Mathematical form**

$$
Q=N(N+2)\sum_{k=1}^{h}\frac{\rho_k^2}{N-k}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxLag` | `int` | $max_Lag$ | Maximum allowed value for Lag in the current routine. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compute residual ACF up to `MaxLag`; sum Ljung-Box statistic.

### aa_ljung_box_test

**Purpose**  
Implements ljung box test within the residual diagnostics module.

**Signature**

```c
int aa_ljung_box_test(vars Residuals,int N,int MaxLag,var Alpha)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:114`

**Mathematical form**

$$
I(p>\alpha)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxLag` | `int` | $max_Lag$ | Maximum allowed value for Lag in the current routine. |
| `Alpha` | `var` | $\alpha$ | Tail probability or confidence significance level. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Return 1 when residuals pass white-noise test; null is no autocorrelation.

### aa_residual_acf

**Purpose**  
Implements residual acf within the residual diagnostics module.

**Signature**

```c
void aa_residual_acf(vars Residuals,int N,int MaxLag,vars OutACF)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:99`

**Mathematical form**

$$
\rho_k(e)=\frac{\gamma_k(e)}{\gamma_0(e)}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxLag` | `int` | $max_Lag$ | Maximum allowed value for Lag in the current routine. |
| `OutACF` | `vars` | $z$ | Output location populated by the routine. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Call `aa_acf()` on residuals.

### aa_residual_mean

**Purpose**  
Implements residual mean within the residual diagnostics module.

**Signature**

```c
var aa_residual_mean(vars Residuals,int N)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:84`

**Mathematical form**

$$
\bar e=\frac{1}{N}\sum_t e_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Call `aa_mean()` on residuals.

### aa_residual_normality_check

**Purpose**  
Implements residual normality check within the residual diagnostics module.

**Signature**

```c
int aa_residual_normality_check(vars Residuals,int N)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:269`

**Mathematical form**

$$
I(p_{JB}>\alpha)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Call Jarque-Bera test with chosen alpha.

### aa_residual_stddev

**Purpose**  
Implements residual stddev within the residual diagnostics module.

**Signature**

```c
var aa_residual_stddev(vars Residuals,int N)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:94`

**Mathematical form**

$$
s_e=\sqrt{s_e^2}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Call `aa_stddev()` or sqrt of residual variance.

### aa_residual_variance

**Purpose**  
Implements residual variance within the residual diagnostics module.

**Signature**

```c
var aa_residual_variance(vars Residuals,int N)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:89`

**Mathematical form**

$$
s_e^2=\frac{1}{N}\sum_t(e_t-\bar e)^2
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Call `aa_variance()` on residuals.

### aa_residual_white_noise_check

**Purpose**  
Implements residual white noise check within the residual diagnostics module.

**Signature**

```c
int aa_residual_white_noise_check(vars Residuals,int N,int MaxLag)
```

**Defined in**  
`src/litec/aa_arima_diagnostics.c:274`

**Mathematical form**

$$
I(p_{LB}>\alpha)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxLag` | `int` | $max_Lag$ | Maximum allowed value for Lag in the current routine. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Call Ljung-Box test with chosen alpha and max lag.

## 16. Model Reporting and File I/O

**Optimization requirement:** reporting functions should not affect model state. File I/O must be optional and kept outside inner optimization loops.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_load_model_from_file

**Purpose**  
Implements load model from file within the model reporting and file i/o module.

**Signature**

```c
int aa_load_model_from_file(ARIMA_MODEL* Model,string Filename,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_platform.c:70`

**Mathematical form**

$$
\text{file}\rightarrow M
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Filename` | `string` | $Filename$ | Routine-specific input or output argument. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Read saved model fields and allocate/copy coefficient arrays into `Model`/`Work`.

### aa_print_coefficients

**Purpose**  
Implements print coefficients within the model reporting and file i/o module.

**Signature**

```c
void aa_print_coefficients(ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_platform.c:34`

**Mathematical form**

$$
\{\phi_i,\vartheta_j,\Phi_i,\Theta_j,c\}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Loop coefficient arrays and print each value.

### aa_print_forecast_report

**Purpose**  
Implements print forecast report within the model reporting and file i/o module.

**Signature**

```c
void aa_print_forecast_report(ARIMA_MODEL* Model,vars ForecastPath,int H)
```

**Defined in**  
`src/litec/aa_arima_platform.c:54`

**Mathematical form**

$$
\{\hat y_h,L_h,U_h\}_{h=1}^{H}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `ForecastPath` | `vars` | $\hat y_{T+h}$ | Vector of multi-step forecasts. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Loop forecast path and optional confidence bands.

### aa_print_model_summary

**Purpose**  
Implements print model summary within the model reporting and file i/o module.

**Signature**

```c
void aa_print_model_summary(ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_platform.c:26`

**Mathematical form**

$$
\text{summary}(M)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Print orders, scores, convergence status, and forecast fields with `printf()`.

### aa_print_residual_diagnostics

**Purpose**  
Implements print residual diagnostics within the model reporting and file i/o module.

**Signature**

```c
void aa_print_residual_diagnostics(ARIMA_MODEL* Model,vars Residuals,int N)
```

**Defined in**  
`src/litec/aa_arima_platform.c:44`

**Mathematical form**

$$
D(e)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Residuals` | `vars` | $\varepsilon_t$ | Residual sequence used for diagnostics, likelihood, or recursion. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Print residual mean/variance, ACF checks, and diagnostic test statistics.

### aa_save_diagnostics_to_file

**Purpose**  
Implements save diagnostics to file within the model reporting and file i/o module.

**Signature**

```c
int aa_save_diagnostics_to_file(ARIMA_MODEL* Model,string Filename)
```

**Defined in**  
`src/litec/aa_arima_platform.c:80`

**Mathematical form**

$$
D(M)\rightarrow\text{file}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Filename` | `string` | $Filename$ | Routine-specific input or output argument. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Write diagnostic metrics to CSV or text log.

### aa_save_forecast_to_file

**Purpose**  
Implements save forecast to file within the model reporting and file i/o module.

**Signature**

```c
int aa_save_forecast_to_file(vars ForecastPath,int H,string Filename)
```

**Defined in**  
`src/litec/aa_arima_platform.c:75`

**Mathematical form**

$$
\{\hat y_h\}_{h=1}^{H}\rightarrow\text{file}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `ForecastPath` | `vars` | $\hat y_{T+h}$ | Vector of multi-step forecasts. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |
| `Filename` | `string` | $Filename$ | Routine-specific input or output argument. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Write horizon and forecast value rows to CSV.

### aa_save_model_to_file

**Purpose**  
Implements save model to file within the model reporting and file i/o module.

**Signature**

```c
int aa_save_model_to_file(ARIMA_MODEL* Model,string Filename)
```

**Defined in**  
`src/litec/aa_arima_platform.c:65`

**Mathematical form**

$$
M\rightarrow\text{file}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Filename` | `string` | $Filename$ | Routine-specific input or output argument. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Serialize scalar fields and coefficient arrays with `file_append()` or CSV format.

## 17. Model Caching and Reuse

**Optimization requirement:** caching functions are mandatory for trading/backtesting performance. Prefer reusing the last best model and refitting only when the window, asset, timeframe, or error threshold changes.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_cache_best_model

**Purpose**  
Implements cache best model within the model caching and reuse module.

**Signature**

```c
int aa_cache_best_model(string AssetName,int BarPeriodValue,ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_platform.c:97`

**Mathematical form**

$$
\mathcal C[(asset,barperiod)]\leftarrow M^\*
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `AssetName` | `string` | $a$ | Asset identifier used by cache or Zorro integration. |
| `BarPeriodValue` | `int` | $\Delta t$ | Bar interval or sampling period associated with cached state. |
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Store best model under asset/timeframe key. Use in-memory or file cache.

### aa_clear_model_cache

**Purpose**  
Implements clear model cache within the model caching and reuse module.

**Signature**

```c
void aa_clear_model_cache()
```

**Defined in**  
`src/litec/aa_arima_platform.c:92`

**Mathematical form**

$$
\mathcal C\leftarrow\varnothing
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| — | — | — | This routine has no explicit parameters. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Clear cached entries and reset counters; free dynamic arrays if owned.

### aa_init_model_cache

**Purpose**  
Implements init model cache within the model caching and reuse module.

**Signature**

```c
void aa_init_model_cache()
```

**Defined in**  
`src/litec/aa_arima_platform.c:85`

**Mathematical form**

$$
\mathcal C\leftarrow\varnothing
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| — | — | — | This routine has no explicit parameters. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Initialize global/static cache arrays or file-based cache state.

### aa_load_cached_model

**Purpose**  
Implements load cached model within the model caching and reuse module.

**Signature**

```c
int aa_load_cached_model(string AssetName,int BarPeriodValue,ARIMA_MODEL* Model,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_platform.c:108`

**Mathematical form**

$$
M\leftarrow\mathcal C[(asset,barperiod)]
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `AssetName` | `string` | $a$ | Asset identifier used by cache or Zorro integration. |
| `BarPeriodValue` | `int` | $\Delta t$ | Bar interval or sampling period associated with cached state. |
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Find matching cache key and copy/load model fields.

### aa_refit_best_model

**Purpose**  
Implements refit best model within the model caching and reuse module.

**Signature**

```c
int aa_refit_best_model(vars Close,int N,ARIMA_MODEL* Model,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_platform.c:127`

**Mathematical form**

$$
$\hat\theta_{\text{new}}=\arg\min_{\theta
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
p,d,q\text{ fixed}}L(\theta)$

### aa_reuse_previous_model

**Purpose**  
Implements reuse previous model within the model caching and reuse module.

**Signature**

```c
int aa_reuse_previous_model(vars Close,int N,ARIMA_MODEL* Model,ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_platform.c:118`

**Mathematical form**

$$
\hat y_{T+1}=f(y_{0:T};\hat\theta_{\text{old}})
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Use old orders/coefficients on new data without searching all models again.

### aa_should_refit_model

**Purpose**  
Implements should refit model within the model caching and reuse module.

**Signature**

```c
int aa_should_refit_model(int BarsSinceLastFit,int RefitInterval,var LastError,var ErrorLimit)
```

**Defined in**  
`src/litec/aa_arima_platform.c:132`

**Mathematical form**

$$
$I(b\ge B_{\max}\ \lor\
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `BarsSinceLastFit` | `int` | $BarsSinceLastFit$ | Routine-specific input or output argument. |
| `RefitInterval` | `int` | $RefitInterval$ | Routine-specific input or output argument. |
| `LastError` | `var` | $LastError$ | Routine-specific input or output argument. |
| `ErrorLimit` | `var` | $ErrorLimit$ | Routine-specific input or output argument. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
e

## 18. Walk-Forward and Forecast Evaluation

**Optimization requirement:** walk-forward functions must reuse one `ARIMA_WORK` object across all rolling windows. Avoid full AutoARIMA refits on every step unless explicitly requested.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_directional_accuracy

**Purpose**  
Implements directional accuracy within the walk-forward and forecast evaluation module.

**Signature**

```c
var aa_directional_accuracy(vars Actual,vars Forecast,int N)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:274`

**Mathematical form**

$$
DA=\frac{1}{N-1}\sum_{t=1}^{N-1}I[\operatorname{sign}(y_t-y_{t-1})=\operatorname{sign}(\hat y_t-y_{t-1})]
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Actual` | `vars` | $y_{T+h}$ | Observed realization used in forecast evaluation. |
| `Forecast` | `vars` | $\hat y_h$ | Forecast value or one-step forecast input. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compare actual and forecast direction relative to previous price.

### aa_forecast_error_mae

**Purpose**  
Implements forecast error mae within the walk-forward and forecast evaluation module.

**Signature**

```c
var aa_forecast_error_mae(vars Actual,vars Forecast,int N)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:254`

**Mathematical form**

$$
$MAE=\frac{1}{N}\sum_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Actual` | `vars` | $y_{T+h}$ | Observed realization used in forecast evaluation. |
| `Forecast` | `vars` | $\hat y_h$ | Forecast value or one-step forecast input. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
y_t-\hat y_t

### aa_forecast_error_mape

**Purpose**  
Implements forecast error mape within the walk-forward and forecast evaluation module.

**Signature**

```c
var aa_forecast_error_mape(vars Actual,vars Forecast,int N)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:269`

**Mathematical form**

$$
$MAPE=\frac{100}{N}\sum_t\left
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Actual` | `vars` | $y_{T+h}$ | Observed realization used in forecast evaluation. |
| `Forecast` | `vars` | $\hat y_h$ | Forecast value or one-step forecast input. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
\frac{y_t-\hat y_t}{y_t}\right

### aa_forecast_error_mse

**Purpose**  
Implements forecast error mse within the walk-forward and forecast evaluation module.

**Signature**

```c
var aa_forecast_error_mse(vars Actual,vars Forecast,int N)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:259`

**Mathematical form**

$$
MSE=\frac{1}{N}\sum_t(y_t-\hat y_t)^2
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Actual` | `vars` | $y_{T+h}$ | Observed realization used in forecast evaluation. |
| `Forecast` | `vars` | $\hat y_h$ | Forecast value or one-step forecast input. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Loop squared forecast errors.

### aa_forecast_error_rmse

**Purpose**  
Implements forecast error rmse within the walk-forward and forecast evaluation module.

**Signature**

```c
var aa_forecast_error_rmse(vars Actual,vars Forecast,int N)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:264`

**Mathematical form**

$$
RMSE=\sqrt{MSE}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Actual` | `vars` | $y_{T+h}$ | Observed realization used in forecast evaluation. |
| `Forecast` | `vars` | $\hat y_h$ | Forecast value or one-step forecast input. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Return sqrt of MSE.

### aa_rolling_forecast_test

**Purpose**  
Implements rolling forecast test within the walk-forward and forecast evaluation module.

**Signature**

```c
int aa_rolling_forecast_test(vars Series,int N,int Window,int Horizon,ARIMA_WORK* Work,vars Forecasts,vars Actuals)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:204`

**Mathematical form**

$$
$\hat y_{t+h
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Window` | `int` | $Window$ | Routine-specific input or output argument. |
| `Horizon` | `int` | $Horizon$ | Routine-specific input or output argument. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Forecasts` | `vars` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |
| `Actuals` | `vars` | $y_{T+h}$ | Observed values used to score forecast accuracy. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
t}=f(y_{t-W+1:t})$

### aa_train_test_split

**Purpose**  
Implements train test split within the walk-forward and forecast evaluation module.

**Signature**

```c
void aa_train_test_split(vars Series,int N,var TrainRatio,vars Train,vars Test,int* TrainN,int* TestN)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:187`

**Mathematical form**

$$
N_{\text{train}}=\lfloor rN\rfloor,\quad N_{\text{test}}=N-N_{\text{train}}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `TrainRatio` | `var` | $TrainRatio$ | Routine-specific input or output argument. |
| `Train` | `vars` | $Train$ | Vector input or output buffer used by the routine. |
| `Test` | `vars` | $Test$ | Vector input or output buffer used by the routine. |
| `TrainN` | `int*` | $z$ | Pointer argument passed by reference. |
| `TestN` | `int*` | $z$ | Pointer argument passed by reference. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Copy first part to train, remaining part to test; return lengths by pointer.

### aa_walk_forward_arima

**Purpose**  
Implements walk forward arima within the walk-forward and forecast evaluation module.

**Signature**

```c
int aa_walk_forward_arima(vars Series,int N,int Window,int Horizon,int RefitInterval,ARIMA_WORK* Work,vars Forecasts)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:227`

**Mathematical form**

$$
M_t=\operatorname{fit}(y_{t-W+1:t}),\quad \hat y_{t+h}=f(M_t)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Series` | `vars` | $y_t$ | Primary series argument supplied to the routine. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Window` | `int` | $Window$ | Routine-specific input or output argument. |
| `Horizon` | `int` | $Horizon$ | Routine-specific input or output argument. |
| `RefitInterval` | `int` | $RefitInterval$ | Routine-specific input or output argument. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Forecasts` | `vars` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Refit every `RefitInterval` bars, otherwise reuse/refit cached model.

## 19. Trading Signal Functions

**Optimization requirement:** signal functions should be allocation-free and use already-computed forecasts, bands, and diagnostics. Keep model fitting separate from trading decision logic.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_backtest_forecast_signal

**Purpose**  
Implements backtest forecast signal within the trading signal functions module.

**Signature**

```c
int aa_backtest_forecast_signal(vars Close,vars Forecast,int N,var Threshold,vars SignalOut)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:350`

**Mathematical form**

$$
s_t=g(y_t,\hat y_{t+1},\tau)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `Forecast` | `vars` | $\hat y_h$ | Forecast value or one-step forecast input. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `Threshold` | `var` | $\tau$ | Decision threshold used for classification or trading logic. |
| `SignalOut` | `vars` | $s_t$ | Output buffer that receives the generated signal. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Loop historical close/forecast arrays and fill signal output.

### aa_forecast_edge

**Purpose**  
Implements forecast edge within the trading signal functions module.

**Signature**

```c
var aa_forecast_edge(var LastPrice,var ForecastPrice,var Cost)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:302`

**Mathematical form**

$$
E=\frac{\hat y-y_T}{y_T}-c_{\text{cost}}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `LastPrice` | `var` | $LastPrice$ | Routine-specific input or output argument. |
| `ForecastPrice` | `var` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |
| `Cost` | `var` | $Cost$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Forecast return minus estimated transaction cost.

### aa_forecast_return

**Purpose**  
Implements forecast return within the trading signal functions module.

**Signature**

```c
var aa_forecast_return(var LastPrice,var ForecastPrice)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:297`

**Mathematical form**

$$
\hat r=\frac{\hat y_{T+1}-y_T}{y_T}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `LastPrice` | `var` | $LastPrice$ | Routine-specific input or output argument. |
| `ForecastPrice` | `var` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Subtract last price from forecast and divide by last price.

### aa_forecast_zscore

**Purpose**  
Implements forecast zscore within the trading signal functions module.

**Signature**

```c
var aa_forecast_zscore(var Forecast,var ForecastSE,var LastPrice)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:307`

**Mathematical form**

$$
z=\frac{\hat y-y_T}{SE}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Forecast` | `var` | $\hat y_h$ | Forecast value or one-step forecast input. |
| `ForecastSE` | `var` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |
| `LastPrice` | `var` | $LastPrice$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Compute forecast deviation divided by forecast standard error.

### aa_position_size_from_confidence

**Purpose**  
Implements position size from confidence within the trading signal functions module.

**Signature**

```c
var aa_position_size_from_confidence(var ForecastZ,var BaseLots)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:345`

**Mathematical form**

$$
$Lots=BaseLots\cdot \min(
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `ForecastZ` | `var` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |
| `BaseLots` | `var` | $BaseLots$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
z

### aa_position_size_from_forecast

**Purpose**  
Implements position size from forecast within the trading signal functions module.

**Signature**

```c
var aa_position_size_from_forecast(var ForecastEdge,var RiskPerTrade,var StopDistance)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:340`

**Mathematical form**

$$
$Lots=\frac{RiskPerTrade\cdot
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `ForecastEdge` | `var` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |
| `RiskPerTrade` | `var` | $RiskPerTrade$ | Routine-specific input or output argument. |
| `StopDistance` | `var` | $StopDistance$ | Routine-specific input or output argument. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
E

### aa_signal_from_confidence_band

**Purpose**  
Implements signal from confidence band within the trading signal functions module.

**Signature**

```c
int aa_signal_from_confidence_band(var LastPrice,var Lower,var Upper)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:325`

**Mathematical form**

$$
s=1\ \text{if }L>y_T,\quad s=-1\ \text{if }U<y_T,\quad 0\text{ otherwise}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `LastPrice` | `var` | $LastPrice$ | Routine-specific input or output argument. |
| `Lower` | `var` | $L_h$ | Lower confidence or prediction bound written by reference. |
| `Upper` | `var` | $U_h$ | Upper confidence or prediction bound written by reference. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Long when entire band is above current price; short when entire band is below.

### aa_signal_from_directional_accuracy

**Purpose**  
Implements signal from directional accuracy within the trading signal functions module.

**Signature**

```c
int aa_signal_from_directional_accuracy(var DirectionalAccuracy,var MinAccuracy)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:335`

**Mathematical form**

$$
s=I(DA\ge DA_{\min})
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `DirectionalAccuracy` | `var` | $DirectionalAccuracy$ | Routine-specific input or output argument. |
| `MinAccuracy` | `var` | $MinAccuracy$ | Routine-specific input or output argument. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Allow trading only when directional accuracy exceeds threshold.

### aa_signal_from_forecast

**Purpose**  
Implements signal from forecast within the trading signal functions module.

**Signature**

```c
int aa_signal_from_forecast(var LastPrice,var ForecastPrice,var Threshold)
```

**Defined in**  
`src/litec/aa_arima_forecast.c:312`

**Mathematical form**

$$
s=\begin{cases}1,&\hat r>\tau\\\\-1,&\hat r<-\tau\\\\0,&\text{otherwise}\end{cases}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `LastPrice` | `var` | $LastPrice$ | Routine-specific input or output argument. |
| `ForecastPrice` | `var` | $\hat y_{T+h}$ | Forecast series or forecast-related buffer. |
| `Threshold` | `var` | $\tau$ | Decision threshold used for classification or trading logic. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Use `if/else`, not ternary. Return long/short/flat signal.

## 20. Zorro Integration Functions

**Optimization requirement:** use Zorro built-ins for price series, plotting, logging, and strategy lifecycle. Allocate work memory in initialization, reuse during bars, and release on exit.


| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_zorro_forecast_current_asset

**Purpose**  
Implements zorro forecast current asset within the zorro integration functions module.

**Signature**

```c
int aa_zorro_forecast_current_asset(int N,int MaxP,int MaxQ,var TickSize,ARIMA_WORK* Work,ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_platform.c:156`

**Mathematical form**

$$
\hat y_{T+1}=AutoARIMA(Close,N)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxP` | `int` | $max_P$ | Maximum allowed value for P in the current routine. |
| `MaxQ` | `int` | $max_Q$ | Maximum allowed value for Q in the current routine. |
| `TickSize` | `var` | $\delta$ | Minimum price increment used to quantize forecasts or orders. |
| `Work` | `ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Select current asset data, prepare work memory, call AutoARIMA/SARIMA forecast, store model.

### aa_zorro_get_close_series

**Purpose**  
Implements zorro get close series within the zorro integration functions module.

**Signature**

```c
int aa_zorro_get_close_series(vars Close,int N)
```

**Defined in**  
`src/litec/aa_arima_platform.c:143`

**Mathematical form**

$$
Close_i=\operatorname{priceClose}(i)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Fill a provided array or use `series(priceClose())`. Remember Zorro series are newest-first.

### aa_zorro_plot_forecast

**Purpose**  
Implements zorro plot forecast within the zorro integration functions module.

**Signature**

```c
void aa_zorro_plot_forecast(ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_platform.c:170`

**Mathematical form**

$$
plot(\hat y_{T+1})
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Call `plot()` with forecast value and chosen color/style.

### aa_zorro_plot_forecast_bands

**Purpose**  
Implements zorro plot forecast bands within the zorro integration functions module.

**Signature**

```c
void aa_zorro_plot_forecast_bands(vars ForecastPath,vars Lower,vars Upper,int H)
```

**Defined in**  
`src/litec/aa_arima_platform.c:176`

**Mathematical form**

$$
plot(L_h),\ plot(U_h)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `ForecastPath` | `vars` | $\hat y_{T+h}$ | Vector of multi-step forecasts. |
| `Lower` | `vars` | $L_h$ | Lower confidence or prediction bound written by reference. |
| `Upper` | `vars` | $U_h$ | Upper confidence or prediction bound written by reference. |
| `H` | `int` | $H$ | Forecast horizon or number of forward steps. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Plot upper/lower bands, usually for horizon 1 or latest path value.

### aa_zorro_print_model

**Purpose**  
Implements zorro print model within the zorro integration functions module.

**Signature**

```c
void aa_zorro_print_model(ARIMA_MODEL* Model)
```

**Defined in**  
`src/litec/aa_arima_platform.c:186`

**Mathematical form**

$$
printf(M)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Wrap `aa_print_model_summary()` for current asset context.

### aa_zorro_trade_from_forecast

**Purpose**  
Implements zorro trade from forecast within the zorro integration functions module.

**Signature**

```c
int aa_zorro_trade_from_forecast(ARIMA_MODEL* Model,var Threshold)
```

**Defined in**  
`src/litec/aa_arima_platform.c:191`

**Mathematical form**

$$
enterLong/enterShort\ \text{from }s_t
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Model` | `ARIMA_MODEL*` | $M$ | Mutable ARIMA/SARIMA model structure. |
| `Threshold` | `var` | $\tau$ | Decision threshold used for classification or trading logic. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Convert model forecast to signal, then call Zorro entry/exit functions outside lookback.

## 21. Current AutoARIMA Compatibility Functions

**Optimization requirement:** compatibility wrappers should call the optimized work-memory versions internally. Keep old signatures only as a convenience layer, not as the performance path.


These are names already present in the current code or optimized work-memory version.
| Function / object | LaTeX formula code | How to construct in optimized lite-C (reuse + built-ins required) |
|---|---|---|

### aa_prepare_auto_arima_work

**Purpose**  
Implements prepare auto arima work within the current autoarima compatibility functions module.

**Signature**

```c
int aa_prepare_auto_arima_work(AUTO_ARIMA_WORK* Work,int N,int MaxP,int MaxQ)
```

**Defined in**  
`src/litec/aa_arima_platform.c:253`

**Mathematical form**

$$
\operatorname{cap}(W)\ge(N,MaxP,MaxQ)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Work` | `AUTO_ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `MaxP` | `int` | $max_P$ | Maximum allowed value for P in the current routine. |
| `MaxQ` | `int` | $max_Q$ | Maximum allowed value for Q in the current routine. |

**Return value**  
Status flag, decision code, or success indicator returned by the routine.

**Implementation note**  
Ensure existing optimized work memory can hold current data and P/Q search sizes.

### aa_round_to_tick_size

**Purpose**  
Implements round to tick size within the current autoarima compatibility functions module.

**Signature**

```c
var aa_round_to_tick_size(var Price,var TickSize)
```

**Defined in**  
`src/litec/aa_arima_platform.c:233`

**Mathematical form**

$$
\hat y_{\text{tick}}=\operatorname{round}\left(\frac{\hat y}{\tau}\right)\tau
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Price` | `var` | $Price$ | Routine-specific input or output argument. |
| `TickSize` | `var` | $\delta$ | Minimum price increment used to quantize forecasts or orders. |

**Return value**  
Scalar quantity defined by the mathematical form above.

**Implementation note**  
Return original price if tick size is not positive; otherwise round to nearest tick.

### auto_arima_forecast

**Purpose**  
Implements forecast within the current autoarima compatibility functions module.

**Signature**

```c
int auto_arima_forecast(vars Close,int N,var TickSize,int MaxP,int MaxQ,AUTO_ARIMA_RESULT* Result)
```

**Defined in**  
`src/litec/aa_arima_platform.c:302`

**Mathematical form**

$$
\hat y_{T+1}=AutoARIMA(y_{0:T})
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `TickSize` | `var` | $\delta$ | Minimum price increment used to quantize forecasts or orders. |
| `MaxP` | `int` | $max_P$ | Maximum allowed value for P in the current routine. |
| `MaxQ` | `int` | $max_Q$ | Maximum allowed value for Q in the current routine. |
| `Result` | `AUTO_ARIMA_RESULT*` | $z$ | Output location populated by the routine. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Compatibility wrapper: allocate temporary work memory, fit best ARIMA model, return one-step forecast.

### auto_arima_forecast_with_work

**Purpose**  
Implements forecast with work within the current autoarima compatibility functions module.

**Signature**

```c
int auto_arima_forecast_with_work(vars Close,int N,var TickSize,int MaxP,int MaxQ,AUTO_ARIMA_WORK* Work,AUTO_ARIMA_RESULT* Result)
```

**Defined in**  
`src/litec/aa_arima_platform.c:261`

**Mathematical form**

$$
\hat y_{T+1}=AutoARIMA(y_{0:T};W)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Close` | `vars` | $y_t$ | Observed price or close series used as the target variable. |
| `N` | `int` | $N$ | Sample length or active number of observations. |
| `TickSize` | `var` | $\delta$ | Minimum price increment used to quantize forecasts or orders. |
| `MaxP` | `int` | $max_P$ | Maximum allowed value for P in the current routine. |
| `MaxQ` | `int` | $max_Q$ | Maximum allowed value for Q in the current routine. |
| `Work` | `AUTO_ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |
| `Result` | `AUTO_ARIMA_RESULT*` | $z$ | Output location populated by the routine. |

**Return value**  
Integer result defined by the procedure.

**Implementation note**  
Preferred fast wrapper: reuse work memory during P/Q optimization.

### free_auto_arima_result

**Purpose**  
Implements auto arima result within the current autoarima compatibility functions module.

**Signature**

```c
void free_auto_arima_result(AUTO_ARIMA_RESULT* R)
```

**Defined in**  
`src/litec/aa_arima_platform.c:223`

**Mathematical form**

$$
\operatorname{free}(R.ar),\operatorname{free}(R.ma)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `R` | `AUTO_ARIMA_RESULT*` | $z$ | Pointer argument passed by reference. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Free result-owned coefficient arrays. Do not use this when result pointers refer to reusable work memory.

### free_auto_arima_work

**Purpose**  
Implements auto arima work within the current autoarima compatibility functions module.

**Signature**

```c
void free_auto_arima_work(AUTO_ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_platform.c:247`

**Mathematical form**

$$
\forall w_i\in W:\operatorname{free}(w_i)
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Work` | `AUTO_ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Free all arrays owned by existing optimized work-memory struct.

### init_auto_arima_result

**Purpose**  
Implements auto arima result within the current autoarima compatibility functions module.

**Signature**

```c
void init_auto_arima_result(AUTO_ARIMA_RESULT* R)
```

**Defined in**  
`src/litec/aa_arima_platform.c:211`

**Mathematical form**

$$
R\leftarrow\{p=-1,q=-1,d=0,SSE=\infty,AICc=\infty\}
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `R` | `AUTO_ARIMA_RESULT*` | $z$ | Pointer argument passed by reference. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Initialize the existing `AUTO_ARIMA_RESULT` struct and set AR/MA pointers to zero.

### init_auto_arima_work

**Purpose**  
Implements auto arima work within the current autoarima compatibility functions module.

**Signature**

```c
void init_auto_arima_work(AUTO_ARIMA_WORK* Work)
```

**Defined in**  
`src/litec/aa_arima_platform.c:241`

**Mathematical form**

$$
W\leftarrow0
$$

| Parameter | Type | Symbol / role | Description |
|---|---|---|---|
| `Work` | `AUTO_ARIMA_WORK*` | $W$ | Reusable working buffers and scratch arrays. |

**Return value**  
No direct return value. Results are written to output parameters or mutable structures.

**Implementation note**  
Initialize current optimized work-memory struct.

## Appendix A. Supporting Documented Data Structures

These items are documented in the formulas source as structural mathematical objects rather than callable functions.

### ARIMA_MODEL

**Documented form**

```text
ARIMA_MODEL(...)
```

**Mathematical form**

$$
\theta = \{p,d,q,P,D,Q,m,c,\mu,\phi,\vartheta,\Phi,\Theta,\epsilon,\mathrm{IC}\}
$$

**Interpretation**  
Create a struct that stores model orders, coefficients, residuals, likelihood scores, forecast output, and optimizer status. Use pointer fields for variable-size arrays.

### ARIMA_WORK

**Documented form**

```text
ARIMA_WORK(...)
```

**Mathematical form**

$$
W = \{\text{temporary arrays used by fitting, forecasting, diagnostics}\}
$$

**Interpretation**  
Create one reusable work-memory struct. Allocate with `malloc()` once, reuse across model search, and release with `free()` at session end.

### ARIMA_CANDIDATE

**Documented form**

```text
ARIMA_CANDIDATE(...)
```

**Mathematical form**

$$
C_i=(p_i,d_i,q_i,P_i,D_i,Q_i,m_i,S_i)
$$

**Interpretation**  
Store one candidate model and its score. Use arrays of candidates for grid or stepwise search.

## Appendix B. Source Alignment Notes

- This reference includes only functions that are both present in source and explicitly documented in the formulas catalog, plus one explicitly mentioned AutoARIMA wrapper entry promoted into full documentation.
- Functions repeated in multiple conceptual sections are documented once under their first occurrence and cross-linked with **Also documented under** notes.
- Supporting documented structures are kept in the appendix so the main body remains function-centric.
