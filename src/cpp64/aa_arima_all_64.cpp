// Zorro64 C++ additive build of the ARIMA/AutoARIMA lite-C library.
//
// This file intentionally reuses the original src/litec modules instead of
// duplicating them. That keeps the 64-bit C++ variant behavior aligned with
// the validated 32-bit lite-C library while preserving Zorro built-ins such as
// series(), priceClose(), plot(), enterLong(), and enterShort().
//
// Porting rules preserved here:
// - Keep Zorro data typed as var / vars.
// - Keep series indexing semantics unchanged: newest bar is [0].
// - Keep pointer-owned work buffers and rawBuffer reuse for low allocation churn.
// - Keep built-in Zorro functions instead of re-implementing equivalents.
// - Keep this file as a library-only translation unit. No run() entry point here.

#ifndef AA_ARIMA_CPP64_ALL_CPP
#define AA_ARIMA_CPP64_ALL_CPP

#if !defined(_WIN64)
#error "aa_arima_all_64.cpp is intended for 64-bit Zorro C++ builds."
#endif

#if !defined(__cplusplus)
#error "aa_arima_all_64.cpp must be compiled as C++."
#endif

// C runtime declarations used by the original lite-C modules.
#include <math.h>
#include <memory.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Reuse the exact library modules so the 64-bit version remains additive and
// behavior-compatible with the existing 32-bit implementation.
#ifdef __cplusplus
extern "C" {
#endif

#include "../litec/aa_arima_types.h"
#include "../litec/aa_arima_memory.c"
#include "../litec/aa_arima_math.c"
#include "../litec/aa_arima_data.c"
#include "../litec/aa_arima_transform.c"
#include "../litec/aa_arima_stats.c"
#include "../litec/aa_arima_fit.c"
#include "../litec/aa_arima_objective.c"
#include "../litec/aa_arima_optimizers.c"
#include "../litec/aa_arima_search.c"
#include "../litec/aa_arima_diagnostics.c"
#include "../litec/aa_arima_forecast.c"
#include "../litec/aa_arima_exogenous.c"
#include "../litec/aa_arima_platform.c"

#ifdef __cplusplus
}
#endif

#endif
