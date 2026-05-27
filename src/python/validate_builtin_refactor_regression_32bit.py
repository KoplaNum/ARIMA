from __future__ import annotations

import csv
import math
import subprocess
from pathlib import Path


ROOT = Path(r"D:\IOA44LIO\AutoAri")
BUILD_DIR = ROOT / "build" / "zorro32"
OUTPUT_DIR = ROOT / "validation_outputs"
LIVE_CSV = OUTPUT_DIR / "builtin_refactor_live_outputs.csv"
REPORT_CSV = OUTPUT_DIR / "builtin_refactor_validation_report.csv"
SUMMARY_MD = OUTPUT_DIR / "builtin_refactor_validation_summary.md"


def run_checked(args: list[str], *, timeout: int = 240) -> None:
    result = subprocess.run(args, capture_output=True, text=True, timeout=timeout)
    if result.returncode != 0:
        raise RuntimeError(
            f"command failed: {' '.join(args)}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def build_live_artifacts() -> None:
    run_checked([
        "powershell.exe",
        "-NoProfile",
        "-ExecutionPolicy",
        "Bypass",
        "-Command",
        f'& "{BUILD_DIR / "build_autoari32_zorro32.bat"}"',
    ])
    run_checked([
        "powershell.exe",
        "-NoProfile",
        "-ExecutionPolicy",
        "Bypass",
        "-Command",
        f'& "{BUILD_DIR / "build_refactor_regression_harness_x86.bat"}"',
    ])


def run_harness() -> None:
    run_checked([str(BUILD_DIR / "refactor_regression_harness.exe")])


def scalar(function_name: str, test_id: str, output_key: str, value: float) -> dict[str, str]:
    return {
        "function_name": function_name,
        "test_id": test_id,
        "output_key": output_key,
        "output_index": "0",
        "output_kind": "scalar",
        "value": f"{value:.10f}",
        "status": "OK",
        "message": "",
    }


def integer(function_name: str, test_id: str, output_key: str, value: int) -> dict[str, str]:
    return {
        "function_name": function_name,
        "test_id": test_id,
        "output_key": output_key,
        "output_index": "0",
        "output_kind": "int",
        "value": str(value),
        "status": "OK",
        "message": "",
    }


def vector(function_name: str, test_id: str, output_key: str, values: list[float]) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    for index, value in enumerate(values):
        rows.append({
            "function_name": function_name,
            "test_id": test_id,
            "output_key": output_key,
            "output_index": str(index),
            "output_kind": "vector",
            "value": f"{float(value):.10f}",
            "status": "OK",
            "message": "",
        })
    return rows


def mean(values: list[float]) -> float:
    return sum(values) / len(values)


def variance(values: list[float]) -> float:
    mu = mean(values)
    return sum((x - mu) * (x - mu) for x in values) / len(values)


def stddev(values: list[float]) -> float:
    return math.sqrt(max(variance(values), 0.0))


def covariance(x: list[float], y: list[float]) -> float:
    mx = mean(x)
    my = mean(y)
    return sum((a - mx) * (b - my) for a, b in zip(x, y)) / len(x)


def correlation(x: list[float], y: list[float]) -> float:
    return covariance(x, y) / max(stddev(x) * stddev(y), 1e-10)


def durbin_watson(values: list[float]) -> float:
    num = sum((values[i] - values[i - 1]) ** 2 for i in range(1, len(values)))
    den = sum(v * v for v in values)
    return num / max(den, 1e-10)


def jarque_bera(values: list[float]) -> float:
    mu = mean(values)
    s = stddev(values)
    if s <= 1e-10:
        s = 1.0
    z = [(v - mu) / s for v in values]
    skew = sum(v ** 3 for v in z) / len(values)
    kurt = sum(v ** 4 for v in z) / len(values)
    return len(values) * (skew * skew + (kurt - 3.0) * (kurt - 3.0) * 0.25) / 6.0


def autocovariance(values: list[float], lag: int) -> float:
    mu = mean(values)
    if lag == 0:
        return sum((v - mu) * (v - mu) for v in values) / len(values)
    return sum((values[i] - mu) * (values[i - lag] - mu) for i in range(lag, len(values))) / len(values)


def autocorrelation(values: list[float], lag: int) -> float:
    return autocovariance(values, lag) / max(autocovariance(values, 0), 1e-10)


def levinson_durbin(acf_vals: list[float], order: int) -> list[float]:
    ar = [0.0] * order
    workspace = [0.0] * order
    prediction_error = float(acf_vals[0])
    if prediction_error <= 0:
        prediction_error = 1e-10
    ar[0] = float(acf_vals[1]) / prediction_error
    workspace[0] = ar[0]
    prediction_error = prediction_error * (1.0 - ar[0] ** 2)
    for k in range(1, order):
        reflection = float(acf_vals[k + 1])
        for j in range(k):
            reflection -= float(ar[j]) * float(acf_vals[k - j])
        reflection = reflection / prediction_error
        temp_ar = ar[:]
        temp_ar[k] = reflection
        for j in range(k):
            temp_ar[j] = workspace[j] - reflection * ar[k - 1 - j]
        for j in range(k + 1):
            workspace[j] = temp_ar[j]
            ar[j] = temp_ar[j]
        prediction_error = prediction_error * (1.0 - reflection ** 2)
        if prediction_error <= 0:
            prediction_error = 1e-10
    return ar


def seasonal_strength(values: list[float], seasonal_period: int) -> float:
    total_variance = variance(values)
    residuals = [(values[i] - values[i - seasonal_period]) for i in range(seasonal_period, len(values))]
    residual_variance = sum(v * v for v in residuals) / len(residuals)
    if total_variance <= 1e-10:
        return 0.0
    return max(0.0, 1.0 - residual_variance / total_variance)


def build_expected_rows() -> list[dict[str, str]]:
    fix_noisy = [10.0, 11.0, 9.0, 12.0, 8.0, 13.0, 9.0, 10.0]
    fix_price = [100.0, 102.0, 101.0, 105.0, 107.0, 106.0, 108.0, 110.0]
    x = [1.0, 2.0, 3.0, 4.0, 5.0]
    y = [2.0, 4.0, 3.0, 7.0, 6.0]
    forecast_actual = [110.0, 112.0, 115.0]
    forecast_pred = [111.0, 113.0, 114.0]
    actual_dir = [100.0, 102.0, 101.0, 105.0, 107.0]
    forecast_dir = [101.0, 100.0, 103.0, 106.0, 108.0]
    transformed = [4.605170186, 4.700480366, 4.787491743]
    forecast_se = [0.1, 0.2, 0.3]
    acf_values = [1.0] + [autocorrelation(fix_price, lag) for lag in range(1, 4)]
    rows: list[dict[str, str]] = []

    for name, test_id in [
        ("aa_abs", "removed_001"),
        ("aa_max_int", "removed_002"),
        ("aa_min_int", "removed_003"),
        ("aa_safe_div", "removed_004"),
        ("aa_safe_log", "removed_005"),
        ("aa_safe_sqrt", "removed_006"),
        ("aa_square", "removed_007"),
        ("aa_round_to_tick_size", "removed_008"),
    ]:
        rows.append(integer("export_check", test_id, f"{name}_missing", 1))

    rows += vector("aa_log_transform", "refactor_001", "log_output", [math.log(max(v, 1e-10)) for v in [100.0, 102.0, 101.0]])
    rows += vector("aa_boxcox_transform", "refactor_002", "boxcox_log", [math.log(max(v, 1e-10)) for v in [100.0, 102.0, 101.0]])
    rows += vector("aa_boxcox_transform", "refactor_003", "boxcox_half", [(pow(max(v, 1e-10), 0.5) - 1.0) / 0.5 for v in [100.0, 102.0, 101.0]])
    rows += vector("aa_return_transform", "refactor_004", "returns", [fix_price[i] / max(fix_price[i - 1], 1e-10) - 1.0 for i in range(1, len(fix_price))])
    rows.append(scalar("aa_stddev", "refactor_005", "stddev", stddev(fix_noisy)))
    rows.append(scalar("aa_correlation", "refactor_006", "correlation", correlation(x, y)))
    rows.append(scalar("aa_durbin_watson_test", "refactor_007", "dw", durbin_watson(fix_noisy)))
    rows.append(scalar("aa_jarque_bera_stat", "refactor_008", "jb", jarque_bera(fix_noisy)))
    rows.append(scalar("aa_forecast_error_mae", "refactor_009", "mae", (abs(110.0 - 111.0) + abs(112.0 - 113.0) + abs(115.0 - 114.0)) / 3.0))
    rows.append(scalar("aa_forecast_error_mse", "refactor_010", "mse", (((110.0 - 111.0) ** 2) + ((112.0 - 113.0) ** 2) + ((115.0 - 114.0) ** 2)) / 3.0))
    rows.append(scalar("aa_forecast_error_rmse", "refactor_011", "rmse", math.sqrt((((110.0 - 111.0) ** 2) + ((112.0 - 113.0) ** 2) + ((115.0 - 114.0) ** 2)) / 3.0)))
    hit_count = 0
    comparison_count = 0
    for i in range(1, len(actual_dir)):
        actual_change = actual_dir[i] - actual_dir[i - 1]
        forecast_change = forecast_dir[i] - actual_dir[i - 1]
        hit_count += 1 if ((actual_change >= 0 and forecast_change >= 0) or (actual_change < 0 and forecast_change < 0)) else 0
        comparison_count += 1
    rows.append(scalar("aa_directional_accuracy", "refactor_012", "accuracy", hit_count / max(comparison_count, 1)))
    rows.append(scalar("aa_forecast_return", "refactor_013", "return", (103.0 - 100.0) / 100.0))
    rows.append(scalar("aa_forecast_zscore", "refactor_014", "zscore", (103.0 - 100.0) / 2.0))
    rows.append(scalar("aa_position_size_from_forecast", "refactor_015", "position_size", 1000.0 * abs(0.02) / 0.05))
    rows.append(scalar("aa_autocorrelation", "refactor_016", "lag1", autocorrelation(fix_noisy, 1)))
    rows += vector("aa_levinson_durbin", "refactor_017", "ar_coeff", levinson_durbin(acf_values, 3))
    rows.append(scalar("aa_ar_root_modulus", "refactor_018", "root_modulus", 1.0 / (abs(0.8) + abs(0.5) + abs(0.3))))
    rows.append(scalar("aa_ma_root_modulus", "refactor_019", "root_modulus", 1.0 / (abs(0.8) + abs(0.5) + abs(0.3))))
    rows.append(scalar("aa_min_root_modulus", "refactor_020", "root_modulus", 1.0 / max(sum([0.2, -0.1, 0.05]), 1e-10)))
    rows.append(scalar("aa_seasonal_strength", "refactor_021", "strength", seasonal_strength(fix_price, 2)))
    rows += vector("aa_bias_adjusted_backtransform", "refactor_022", "bias_adjusted", [math.exp(transformed[i] + 0.5 * forecast_se[i] * forecast_se[i]) for i in range(3)])
    return rows


def load_rows(path: Path) -> dict[tuple[str, str, str, str], dict[str, str]]:
    with path.open("r", encoding="utf-8", newline="") as handle:
        return {
            (row["function_name"], row["test_id"], row["output_key"], row["output_index"]): row
            for row in csv.DictReader(handle)
        }


def write_report(rows: list[dict[str, str]]) -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    with REPORT_CSV.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.DictWriter(handle, fieldnames=[
            "function_name", "test_id", "output_key", "output_index", "expected", "actual", "abs_diff", "status", "message"
        ])
        writer.writeheader()
        writer.writerows(rows)


def write_summary(total: int, passed: int, failed: int) -> None:
    SUMMARY_MD.write_text(
        "# Builtin Refactor 32-bit Regression Summary\n\n"
        f"- total checks: **{total}**\n"
        f"- pass: **{passed}**\n"
        f"- fail: **{failed}**\n"
        f"- live csv: `{LIVE_CSV}`\n"
        f"- report csv: `{REPORT_CSV}`\n",
        encoding="utf-8",
    )


def compare() -> tuple[int, int, int]:
    expected = {(r["function_name"], r["test_id"], r["output_key"], r["output_index"]): r for r in build_expected_rows()}
    actual = load_rows(LIVE_CSV)
    report_rows: list[dict[str, str]] = []
    passed = 0
    failed = 0
    total = 0

    for key, expected_row in expected.items():
        total += 1
        actual_row = actual.get(key)
        if not actual_row:
            failed += 1
            report_rows.append({
                "function_name": key[0], "test_id": key[1], "output_key": key[2], "output_index": key[3],
                "expected": expected_row["value"], "actual": "", "abs_diff": "", "status": "FAIL", "message": "missing actual row"
            })
            continue
        if expected_row["output_kind"] == "int":
            ok = expected_row["value"] == actual_row["value"]
            diff = "0" if ok else "1"
        else:
            expected_value = float(expected_row["value"])
            actual_value = float(actual_row["value"])
            abs_diff = abs(expected_value - actual_value)
            ok = abs_diff <= 1e-6 or (abs(actual_value) > 1.0 and abs_diff / abs(actual_value) <= 1e-5)
            diff = f"{abs_diff:.10f}"
        if ok:
            passed += 1
        else:
            failed += 1
        report_rows.append({
            "function_name": key[0], "test_id": key[1], "output_key": key[2], "output_index": key[3],
            "expected": expected_row["value"], "actual": actual_row["value"], "abs_diff": diff, "status": "PASS" if ok else "FAIL", "message": ""
        })

    write_report(report_rows)
    write_summary(total, passed, failed)
    return total, passed, failed


def main() -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    build_live_artifacts()
    run_harness()
    total, passed, failed = compare()
    print(f"builtin refactor regression complete: total={total} pass={passed} fail={failed}")
    if failed:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
