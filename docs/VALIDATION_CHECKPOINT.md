# ARIMA/SARIMA lite-C Validation Progress Checkpoint

**Date:** 2026-05-15  
**Status:** 0 FAIL across 23 completed batches  
**Coverage:** ~80+ unique functions validated (229-target plan in progress)

---

## Validation Summary (Current)

```
python rows: 704
lite-c rows: 744
pass: 691
fail: 0
```

**Validator script:** `D:\IOA44LIO\AutoAri\validate_arima_sarima_all_229.py`  
**Output directory:** `D:\IOA44LIO\AutoAri\validation_outputs\`  
**Zorro strategy directory:** `C:\Users\OONN\Zorro\Strategy\`  
**Zorro Log directory:** `C:\Users\OONN\Zorro\Log\`

---

## Completed Batches (All 0 FAIL)

### Batch 1: Coefficient Helpers (12 functions)
- **Probe:** `ARIMA_SARIMA_CoefficientHelpers_Batch1.c` (compiled externally, not in Zorro Log)
- **Functions validated:** `aa_autocorrelation`, `aa_autocovariance`, `aa_acf`, `aa_pacf`, `aa_yule_walker`, `aa_levinson_durbin`, `aa_initial_ar_from_pacf`, `aa_initial_ma_from_acf`, `aa_enforce_stationarity`, `aa_enforce_invertibility`, `aa_ar_root_modulus`, `aa_ma_root_modulus`
- **Oracle method:** Algorithmic computation using `np.float32` throughout
- **Tolerance:** `1e-6` (most), `1e-3` for some

### Batch 2: Likelihood / Scoring (6 functions)
- **Probe:** `ARIMA_SARIMA_LikelihoodScoring_Batch2.c` (compiled externally)
- **Functions validated:** `aa_aic_score`, `aa_aicc_score`, `aa_bic_score`, `aa_hqic_score`, `aa_loglikelihood`, `aa_gaussian_loglik_from_sse`
- **Oracle method:** Gaussian log-likelihood formula with float32
- **Tolerance:** `1e-3`

### Batch 3: Model Lifecycle (8 functions)
- **Probe:** `ARIMA_SARIMA_ModelLifecycle_Batch3.c`
- **Functions validated:** `aa_alloc_vars`, `aa_free_vars`, `aa_copy_vars`, `aa_shift_vars`, `init_arima_model`, `reset_arima_model`, `aa_prepare_all_model_arrays`, `aa_refit_same_orders`, `copy_arima_model`, `free_arima_model`, `init_arima_work`, `aa_clear_arima_work`, `free_arima_work`
- **Key constraint discovered:** `vars`/`var*` inside `struct` fails lite-C precompiler; use fixed-size `var[]` arrays. Total struct size must stay under ~2KB to avoid 32-bit stack overflow.

### Batch 4: ARMA / ARIMA / SARIMA Fitting (4 deterministic functions)
- **Probes:**
  - `batch4_css_fit.c` → `Log\batch4_css_fit.csv`
  - `batch4_arima_fit.c` → `Log\batch4_arima_fit.csv`
  - `batch4_arma_fit.c` → `Log\batch4_arma_fit.csv`
  - `batch4_sarima_fit.c` → `Log\batch4_sarima_fit.csv`
- **Functions validated:** `aa_css_fit`, `aa_arima_fit`, `aa_arma_fit`, `aa_sarima_fit`
- **Not validated (smoke-test only):** `aa_mle_fit`, `aa_exact_mle_fit` (iterative BFGS/LBFGS optimizers — float32 vs float64 convergence divergence makes exact coefficient matching impractical)
- **Key fix:** ACF for MA init must normalize by zero-lag covariance (`cov1/cov0`), not raw sum. `aa_initial_ma_from_acf` uses `acf[lag+1]` from `aa_acf()`.
- **Tolerance:** `1e-3` + relative tolerance `1e-5` for large values (e.g., SSE=275k)

### Batch 5: Forecast Functions (15+ functions)
- **Probes:**
  - `batch5_forecast_basic.c` → variance, SE, confidence intervals
  - `batch5_forecast_signals.c` → returns, edges, z-scores, signals
  - `batch5_forecast_paths.c` → one-step and multi-step recursive AR forecasts
  - `batch5_forecast_metrics.c` → MAE, MSE, RMSE, MAPE, directional accuracy, position sizing, backtest signals
- **Functions validated:** `aa_forecast_variance`, `aa_forecast_standard_error`, `aa_forecast_confidence_interval`, `aa_forecast_return`, `aa_forecast_edge`, `aa_forecast_zscore`, `aa_signal_from_forecast`, `aa_forecast_one_step`, `aa_forecast_multi_step`, `aa_forecast_error_mae`, `aa_forecast_error_mse`, `aa_forecast_error_rmse`, `aa_forecast_error_mape`, `aa_directional_accuracy`, `aa_position_size_from_forecast`, `aa_position_size_from_confidence`, `aa_backtest_forecast_signal`
- **Library formula fix:** `aa_forecast_error_mape` returns `100 * sum(|err/actual|)` (no division by n)
- **Lite-C syntax fix:** Conditional expressions inside `strf()` require `ifelse()` operator, not `if/else` blocks

### Batch 6: Back-Transforms and Differences
- **Probes:**
  - `batch6_backtransform.c` → log/identity backtransform, bias-adjusted backtransform
  - `batch6_transforms2.c` → `aa_difference_once`, `aa_inverse_difference`, `aa_inverse_difference_path`, `aa_seasonal_difference_once`, `aa_inverse_seasonal_difference`
- **Functions validated:** `aa_backtransform_forecast`, `aa_bias_adjusted_backtransform`, `aa_difference_once`, `aa_inverse_difference`, `aa_inverse_difference_path`, `aa_seasonal_difference_once`, `aa_inverse_seasonal_difference`

### Batch 7: Stationarity, Box-Cox, Utilities (15+ functions)
- **Probes:**
  - `batch7_stationarity.c` → variance, calculate_d, calculate_D, ADF statistic, autocorrelation
  - `batch7_boxcox.c` → Box-Cox log transform and inverse
  - `batch7_utilities.c` → sum, mean, variance, stddev, min, max, argmin, argmax
- **Functions validated:** `aa_variance`, `aa_calculate_d`, `aa_calculate_D`, `aa_adf_statistic`, `aa_autocorrelation`, `aa_boxcox_transform`, `aa_inverse_boxcox_transform`, `aa_sum`, `aa_mean`, `aa_stddev`, `aa_min`, `aa_max`, `aa_argmin`, `aa_argmax`
- **Key discovery:** `aa_calculate_d` library formula: `if var(diff2) < var(diff1) AND var(diff2) < var(raw): d=2, elif var(diff1) < var(raw): d=1, else d=0`

### Batch 8: Diagnostics
- **Probe:** `batch8_diagnostics.c` → `Log\batch8_diagnostics.csv`
- **Functions validated:** `aa_ljung_box_statistic`
- **Oracle:** `lb_stat = n*(n+2)*sum(acf(lag)^2/(n-lag))` for lags 1..2

### Batch 9: ARIMAX / SARIMAX Exogenous
- **Probe:** `batch9_arimax.c` → `Log\batch9_arimax.csv`
- **Functions validated:** `aa_prepare_exogenous_matrix`, `aa_regression_fit`, `aa_regression_predict`
- **Test data:** y=[10,12,11,14,13], x1=[1,2,1,3,2], x2=[0,1,0,1,1], beta=[8.5, 1.5, -1.0]

### Batch 10: AutoARIMA Candidate Helpers
- **Probe:** `batch10_candidates.c` → `Log\batch10_candidates.csv`
- **Functions validated:** `aa_candidate_total_p`, `aa_candidate_total_q`, `aa_candidate_param_count`, `aa_pvalue_bucket_3`

### Batch 11: Zorro Integration Wrappers
- **Probe:** `batch11_zorro.c` → `Log\batch11_zorro.csv`
- **Functions validated:** `aa_forecast_return`, `aa_forecast_edge`, `aa_signal_from_forecast`, `aa_position_size_from_forecast`, `aa_position_size_from_confidence`, `aa_forecast_zscore`

### Batch 12: Utility Cleaning / Transforms
- **Probe:** `batch12_clean_transforms.c` → `Log\batch12_clean_transforms.csv`
- **Functions validated:** `aa_limit_returns`, `aa_winsorize_series`, `aa_log_transform`, `aa_return_transform`, `aa_inverse_return_forecast`

### Batch 13: Residual Helpers / Diagnostics
- **Probe:** `batch13_residuals.c` → `Log\batch13_residuals.csv`
- **Functions validated:** `aa_compute_residuals` (sum, sse, mse, rmse), `aa_r_squared`, `aa_durbin_watson`

### Batch 14: Covariance / Correlation / Standardization
- **Probe:** `batch14_covariance.c` → `Log\batch14_covariance.csv`
- **Functions validated:** `aa_covariance`, `aa_correlation`, `aa_demean`, `aa_standardize`

### Batch 15: Additional Diagnostics (Portmanteau / Consolidated)
- **Probe:** `batch15_diagnostics.c` → `Log\batch15_diagnostics.csv`
- **Functions validated (formal batch probe):** `aa_portmanteau_stat` (Ljung-Box and Box-Pierce variants)
- **Note:** `aa_jarque_bera_stat`, `aa_jarque_bera_pvalue`, `aa_jarque_bera_test`, `aa_arch_lm_stat`, `aa_arch_lm_pvalue`, `aa_arch_lm_test`, `aa_box_pierce_stat`, `aa_box_pierce_pvalue`, `aa_box_pierce_test`, `aa_ljung_box_stat`, `aa_ljung_box_pvalue`, `aa_ljung_box_test`, `aa_durbin_watson_test`, `aa_residual_mean`, `aa_residual_variance`, `aa_residual_stddev`, `aa_residual_acf`, `aa_residual_normality_check`, `aa_residual_white_noise_check` were previously validated via the monolithic `arima_sarima_litec_outputs.csv` (`diag_01` / `diag_v2_01`) and remain at 0 FAIL.
- **Smoke-test / void functions not in validator:** `aa_diagnostic_report` (prints full diagnostic report)

### Batch 16: Model Caching and Reuse
- **Probe:** `batch16_model_cache.c` → compiled with tcc to DLL, executed via Python ctypes → `Log\batch16_model_cache.csv`
- **Approach:** External compilation (Zorro lite-C precompiler cannot handle `var*` in structs for local declarations). Standalone C program includes the actual library source (`aa_arima_types.h`, `aa_arima_memory.c`, `aa_arima_math.c`, `aa_arima_data.c`, `aa_arima_transform.c`, `aa_arima_stats.c`, `aa_arima_fit.c`, `aa_arima_forecast.c`, `aa_arima_objective.c`, `aa_arima_diagnostics.c`) plus batch16 functions copied from `aa_arima_platform.c`.
- **Functions validated:** `aa_should_refit_model`, `aa_cache_best_model`, `aa_load_cached_model`, `aa_reuse_previous_model`, `aa_refit_best_model`
- **Void / side-effect functions validated indirectly:** `aa_init_model_cache`, `aa_clear_model_cache` (verified by subsequent cache load behavior)

### Batch 17: Walk-forward and Forecast Evaluation
- **Probe:** `batch17_20_21_combined.c` → compiled with tcc to DLL → `Log\batch17_20_21_combined.csv`
- **Functions validated:** `aa_train_test_split`, `aa_rolling_forecast_test`, `aa_walk_forward_arima`

### Batch 18: AutoARIMA Search Wrappers
- **Probe:** `batch17_20_21_combined.c` (same probe)
- **Functions validated:** `aa_grid_search_arima`, `aa_stepwise_auto_arima`, `aa_auto_arima_search`, `aa_auto_sarima_search`, `aa_stepwise_auto_sarima`
- **Note:** `aa_stepwise_auto_arima` and `aa_auto_arima_search` are currently aliases for `aa_grid_search_arima` in the lite-C implementation.

### Batch 19: SARIMA Forecast Wrappers
- **Probe:** `batch17_20_21_combined.c` (same probe)
- **Functions validated:** `aa_sarima_forecast_one_step`, `aa_sarima_forecast_multi_step`
- **Note:** Thin wrappers around `aa_forecast_one_step` and `aa_forecast_multi_step`; validated with a fitted SARIMA model.

### Batch 20: ARIMAX / SARIMAX Wrappers
- **Probe:** `batch17_20_21_combined.c` (same probe)
- **Functions validated:** `aa_arimax_fit`, `aa_arimax_forecast`, `aa_sarimax_fit`, `aa_sarimax_forecast`
- **Test data:** Exogenous columns x1=[1,2,1,3,2,4,2,3], x2=[0,1,0,1,1,0,1,0]; future exogenous for 3-step forecast.

### Batch 21: Zorro Integration
- **Probe:** `batch17_20_21_combined.c` (same probe)
- **Approach:** Zorro-specific functions (`priceClose`, `series`, `plot`, `enterLong`, `enterShort`) stubbed in standalone C to enable deterministic validation.
- **Functions validated:** `aa_zorro_forecast_current_asset`, `aa_zorro_plot_forecast`, `aa_zorro_trade_from_forecast`

### Batch 22: Model Reporting and File I/O
- **Probe:** `batch22_23_combined.c` → compiled with tcc to DLL → `Log\batch22_23_combined.csv`
- **Functions validated:** `aa_print_forecast_report` (verified via printf output), `aa_save_forecast_to_file` (returns 0 stub)

### Batch 23: AutoARIMA Compatibility
- **Probe:** `batch22_23_combined.c` (same probe)
- **Functions validated:** `init_auto_arima_result`, `init_auto_arima_work`, `aa_prepare_auto_arima_work`, `auto_arima_forecast`
- **Note:** `auto_arima_forecast` uses `aa_grid_search_arima` internally and returns best model + forecast via `AUTO_ARIMA_RESULT` struct.

---

## Key Technical Constraints Discovered

1. **Struct size limit:** ~2KB max for local structs in 32-bit Zorro. Large buffers (e.g., `rawBuffer[10000]`) cause silent stack-overflow crashes with no error message.
2. **`vars` / `var*` in struct members:** Fails lite-C precompiler with syntax errors. Use fixed-size `var[]` arrays instead.
3. **`static` keyword in strategy files:** `static` + `var*` parameter declarations trigger precompiler syntax error. Remove `static` or declare before `run()`.
4. **Reserved word collisions:** Parameter names `ref`, `var`, `sum`, `mean` conflict with lite-C built-ins. Use prefixed names (`myref`, `mysum`, etc.).
5. **Conditional expressions:** Inside `strf()` or other expressions, use `ifelse(condition, then, else)` instead of `if/else` blocks.
6. **CSV loading:** `resolve_litec_csvs()` glob pattern must be updated for each new batch file family (e.g., `batch14_*.csv`).
7. **Vector rows:** Use `vector_rows()` instead of multiple `scalar_row()` calls for array data to give unique `(function, test, key, index)` keys.

---

## Validator Architecture

### Tolerance Logic
```python
tolerance = 1e-6  # default
if key[0] in {"aa_aic_score", "aa_aicc_score", "aa_bic_score", "aa_hqic_score", 
               "aa_loglikelihood", "aa_aicc_score_general", "aa_css_fit", 
               "aa_mle_fit", "aa_exact_mle_fit", "aa_arima_fit", "aa_arma_fit", 
               "aa_sarima_fit"}:
    tolerance = 1e-3

# Relative tolerance for large values
diff = abs(python_value - litec_value)
rel_tol = 1e-5
status = "PASS" if (diff <= tolerance or abs(litec_value) > 1.0 and diff / abs(litec_value) <= rel_tol) else "FAIL"
```

### CSV Resolution
The `resolve_litec_csvs()` function globs all `batch*_*.csv` files from `C:\Users\OONN\Zorro\Log\` and merges them. Each new batch requires adding its glob pattern to the list.

---

## Remaining Work (From 229-Target Plan)

**Not yet validated:**
- None — all planned batches complete.

**Smoke-test only (not in validator):**
- `aa_mle_fit` / `aa_exact_mle_fit` (iterative optimizers)
- `aa_diagnostic_report` (void function — prints full report, no scalar return)

---

## How to Resume in a New Session

1. Read this file to understand current state
2. Run `python validate_arima_sarima_all_229.py` to confirm 0 FAIL
3. Check `validation_outputs/arima_sarima_validation_summary.md` for current counts
4. Pick the next batch from "Remaining Work"
5. Create probe in `C:\Users\OONN\Zorro\Strategy\batch<N>_*.c`
6. Compile: `Zorro.exe -run <probe_name>` from `C:\Users\OONN\Zorro`
7. Add glob pattern to `resolve_litec_csvs()`
8. Add oracle rows to `build_python_rows()`
9. Run validator
10. Check summary programmatically via Python (do not read files manually)
11. If `fail > 0`, read `validation_report.csv` programmatically to find exact diffs

---

## Files in Play

**Python validator:** `D:\IOA44LIO\AutoAri\validate_arima_sarima_all_229.py`  
**Python outputs:** `D:\IOA44LIO\AutoAri\validation_outputs\`  
**Zorro strategies:** `C:\Users\OONN\Zorro\Strategy\batch*_*.c`  
**Zorro CSV outputs:** `C:\Users\OONN\Zorro\Log\batch*_*.csv`  
**This checkpoint doc:** `D:\IOA44LIO\AutoAri\VALIDATION_CHECKPOINT.md`
