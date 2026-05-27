# Zorro64 C++ ARIMA Library Variant

This directory contains the additive 64-bit C++ entry file for the ARIMA / AutoARIMA library.

## Files

- `aa_arima_all_64.cpp`: Zorro64 C++ library aggregation unit.

## Design

The 64-bit variant is intentionally thin.

It reuses the original modules from `src/litec/` so that:

1. Zorro built-ins remain the implementation source of truth.
2. The validated 32-bit logic is not forked into a second maintenance branch.
3. Pointer-based work buffers and low-allocation memory reuse stay unchanged.
4. Time-series semantics remain Zorro-native: `vars` with newest element at `[0]`.

## Porting Constraints Preserved

1. Keep `var` / `vars` types for Zorro math and series APIs.
2. Keep `series(priceClose(0))` style access when using current asset history.
3. Do not replace Zorro built-ins with custom copies when a built-in already exists.
4. Keep the library free of `run()` so only the owning strategy bridge defines the Zorro entry point.
5. Keep memory usage centered on the existing `ARIMA_WORK.rawBuffer` reuse model.

## Intended Use

Compile `src/cpp64/aa_arima_all_64.cpp` as part of a 64-bit Zorro C++ strategy or DLL build.

The owning strategy bridge should:

1. Include Zorro headers and macros first.
2. Include or compile this file after Zorro types like `var`, `vars`, and `string` are available.
3. Define `run()` in exactly one translation unit outside this library.

## Known C++ / Zorro64 Notes

1. This file enforces `_WIN64` and `__cplusplus` to prevent accidental 32-bit or lite-C compilation.
2. The original library already avoids a standalone `main()` and already uses Zorro built-ins directly.
3. The cache and work-buffer code remain pointer-based and 64-bit-safe under Zorro's `var*` model.
