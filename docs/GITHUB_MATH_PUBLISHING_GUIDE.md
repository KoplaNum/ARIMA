# GitHub Math Publishing Guide

This guide records the rendering constraints discovered while preparing mathematical documentation for GitHub Markdown. Use it as the house style for any future publishable math content.

## Ground Truth

- GitHub Markdown math uses MathJax syntax, but some preview/publishing paths can be stricter than full MathJax.
- The safest approach is to use a conservative TeX subset and simple block structure.

## Required Rendering Rules

1. Use display math blocks exactly like this:

```md
$$
\frac{a}{b}
$$
```

2. Never nest inline math inside display math.

Bad:

```md
$$
$\frac{a}{b}$
$$
```

Good:

```md
$$
\frac{a}{b}
$$
```

3. Keep a blank line before and after every display math block.
4. Never place table rows immediately inside a math block.
5. Keep each formula on a single logical line when possible.

## Safe Macro Subset

Prefer these:

- fractions: \frac{a}{b}
- roots: \sqrt{x}
- sums/products: \sum, \prod
- superscripts/subscripts: x_t, y^2
- Greek symbols: \alpha, \beta, \sigma, \phi, \theta, \varepsilon
- comparisons: \le, \ge, \ne
- hats/bars: \hat y, \bar y

## Avoid In Published GitHub Docs

Avoid these unless you have explicitly verified the exact renderer path:

- \operatorname{...}
- \text{...}
- \mathrm{...}
- \mathbb{...}
- \mathcal{...}
- \begin{cases} ... \end{cases}
- any other \begin{...} / \end{...} environment

## Safe Rewrites

Use these replacements instead:

- \operatorname{free}(x) -> free(x)
- \operatorname{cap}(W) -> cap(W)
- \mathbb{R} -> R
- \mathcal{C} -> C
- \leftarrow -> <- if the renderer is unstable
- \rightarrow -> -> if the renderer is unstable
- piecewise/cases forms -> rewrite as max/min or plain conditional prose inside math

Examples:

- Instead of:

  $$
  b^* = \begin{cases} b, & |b| > \varepsilon \\ \varepsilon, & \text{otherwise} \end{cases}
  $$

  use:

  $$
  b^* = \max(|b|, \varepsilon)
  $$

- Instead of:

  $$
  \operatorname{cap}(W) \ge (N,p,q,P,Q,H)
  $$

  use:

  $$
  cap(W) \ge (N,p,q,P,Q,H)
  $$

## Authoring Checklist Before Publishing

- No nested `$...$` inside `$$...$$`
- No blocked presentation macros
- No `\begin{...}` environments
- Every `**Mathematical form**` section is followed by a clean display-math block
- Tables begin only after the closing `$$`
- Preview the final GitHub-facing copy, not only the source document

## Current Repo Practice

When fixing a rendering problem:

1. Fix the source document in `docs/`
2. Sync the same corrected content into the publish copy / `README.md`
3. Re-check the GitHub-facing preview
4. Commit and push only after the publish copy is verified


## Repeated Failure Modes and Permanent Fixes

### 1. Nested inline math inside display math

Problem:

```md
$$
$\frac{a}{b}$
$$
```

Symptoms:
- GitHub says the expression cannot render
- the preview shows raw LaTeX or an open basket problem

Permanent fix:

```md
$$
\frac{a}{b}
$$
```

Rule:
- Never place `$...$` inside `$$...$$`

### 2. Table bleed into a math block

Problem:
- a malformed generator can accidentally let the parameter table start inside the formula block
- example symptom: the formula preview shows table headers such as `| Parameter | Type | ... |`

Permanent fix:
- every `**Mathematical form**` section must be generated in this exact order:
  1. blank line
  2. opening `$$`
  3. exactly one formula line (or a deliberate multi-line formula block)
  4. closing `$$`
  5. blank line
  6. `**Parameters**`

### 3. Markdown table parsing broke formulas containing absolute-value bars

Problem:
- parsing a source markdown table by splitting on every `|` corrupts formulas such as:
  - `k^*=\min\{k:|\alpha_k|<\tau\}`
  - `E[y_{T+h}|F_T,M]`

Permanent fix:
- do **not** split markdown table rows on raw `|`
- parse rows with a strict regex that captures:
  - first backticked signature cell
  - second backticked formula cell
  - remaining note cell

Required parser shape:

```text
| `signature` | `formula` | note |
```

Use a regex similar to:

```text
^\|\s*`([^`]*)`\s*\|\s*`([^`]*)`\s*\|\s*(.*?)\s*\|?$
```

### 4. Conditional expectation bars can be fragile in some preview paths

Problem:
- formulas like `E[y_{T+h}|F_T,M]` may render badly in stricter preview pipelines

Preferred safe rewrite:

```text
E[y_{T+h} given F_T, M]
```

Rule:
- for GitHub publication, prefer `given` wording over conditional bars when preview stability matters more than textbook notation fidelity

### 5. Piecewise / cases syntax is fragile in publication workflows

Problem:
- expressions using `\begin{cases} ... \end{cases}` repeatedly caused renderer or sanitizer failures

Permanent fix:
- rewrite piecewise definitions using:
  - `max(...)` or `min(...)` when mathematically equivalent
  - plain conditional prose inside the formula when needed

Example safe rewrite:

Instead of:

```text
b^* = \begin{cases} b, & |b| > \varepsilon \\ \varepsilon, & otherwise \end{cases}
```

Use:

```text
b^* = \max(|b|, \varepsilon)
```

### 6. Presentation macros may be rejected by the publishing path

Problem macros observed:
- `\operatorname`
- `\text`
- `\mathrm`
- `\mathbb`
- `\mathcal`
- `\begin{...}` / `\end{...}`

Permanent fix:
- reduce them to plain math-safe text:
  - `\operatorname{free}(x)` -> `free(x)`
  - `\mathbb{R}` -> `R`
  - `\mathcal{C}` -> `C`
  - `\leftarrow` -> `<-` if needed for compatibility

### 7. Source and publish copies can diverge

Problem:
- the source document can be fixed while the GitHub publish copy still contains the broken math

Permanent fix:
- always treat publication as a two-file workflow:
  1. fix `docs/ARIMA_SARIMA_liteC_Function_Reference.md`
  2. sync the exact same content into the publish target (for example repo root `README.md`)
  3. preview the publish target, not only the source document

### 8. Final audit rule before pushing

Run a full-document audit that checks all `**Mathematical form**` sections for:
- opening `$$`
- closing `$$`
- no nested `$...$`
- no blocked macros
- no table bleed
- no brace / bracket imbalance
- no accidental truncation

Only push after the audit reports zero structural issues.

## Addendum: Notation Collision Fixes

### 9. Macro stripping can collapse notation tokens

Problem:
- aggressive sanitization can turn valid math into broken tokens like `\inN_s(M_k)` or `y_tarrow`.

Permanent fix:
- after macro stripping, run a normalization pass that restores spacing around semantic tokens:
  - `\inN` -> `\in N`
  - `inR` -> `in R`
  - `arrow` fragments -> explicit assignment arrow or plain `<-`

Rule:
- always run a post-sanitization notation-spacing audit across all math lines before publishing.
