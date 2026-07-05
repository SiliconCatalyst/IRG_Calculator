# IRG Calculator

Terminal application that calculates Algerian IRG (Impot sur le Revenu
Global) on a monthly net imposable salary, showing every calculation step
so students/lecturers can verify it by hand. UI available in
English, and French.

## How to run

Build the project, then run the executable:

```bash
cmake -S . -B build
cmake --build build
./build/irg_calc
```

On Windows (after building), run `build\irg_calc.exe` instead (or
`build\Release\irg_calc.exe` if using a multi-config generator like Visual
Studio).

The app will:

1. Ask you to select a language (`1` English, `2` French).
2. Ask for your monthly net imposable income (DA).
3. Print the full step-by-step IRG calculation and the final net payable
   salary.

Example run:

```
1) English   2) French
Select / Choisissez : 2
Enter your monthly net imposable income (DA): 32000
Net imposable income: 32000.00 DA
Bracket-by-bracket breakdown:
  [0.00 DA - 20000.00 DA] @ 0.00%: taxable amount 20000.00 DA, tax 0.00 DA
  [20000.00 DA - 40000.00 DA] @ 23.00%: taxable amount 12000.00 DA, tax 2760.00 DA
Gross IRG (sum of all brackets): 2760.00 DA
First abattement (40%, capped between 1,000 and 1,500 DA): 1104.00 DA
IRG after first abattement: 1656.00 DA
Income is in the 30,001-35,000 DA smoothing zone: applying second abattement formula.
Final IRG: 957.85 DA
Net payable salary: 31042.15 DA
Thank you for using the IRG calculator.
```

## Architecture

The project is split into three independent concerns, each its own library:

```
irg_core   Pure tax-calculation logic. No I/O, no strings for display.
irg_i18n   Language selection + text lookup. No calculation, no I/O.
irg_cli    Console I/O primitives + the Application orchestrator, which is
           the only place that wires irg_core and irg_i18n together.
```

```
irg_calc/
├── CMakeLists.txt
├── include/irg/
│   ├── core/irg_calculator.hpp   # tax brackets, breakdown struct, calculation
│   ├── i18n/language.hpp         # supported languages
│   ├── i18n/translator.hpp       # text-key -> string lookup
│   ├── cli/console_io.hpp        # generic prompt/print/format helpers
│   └── cli/application.hpp       # orchestrates a full run, step by step
├── src/                          # matching .cpp files
├── tests/                        # unit tests for irg_core (regression-tested
│                                   against hand-verified values)
└── README.md
```

Rule of thumb: `core/` never includes `<iostream>`, and `cli/` never contains
tax-bracket numbers or translated strings inline — everything user-facing
goes through `Translator::translate(TextKey)`.

## Calculation logic (Article 104 CIDTA, barème IRG 2022, in force 2026)

For a given monthly net imposable income:

1. **≤ 30,000 DA** → fully exempt, IRG = 0.
2. **30,001–35,000 DA** → progressive brackets are applied, then the
   standard 40% abattement (capped between 1,000 and 1,500 DA), then a
   second smoothing formula: `IRG = IRG(after 1st abattement) × (137/51) − (27925/8)`.
3. **> 35,000 DA** → progressive brackets, then the standard 40% abattement
   (capped between 1,000 and 1,500 DA). No second abattement.

`IrgCalculator::calculateMonthlyIrgBreakdown()` returns every intermediate
value above (per-bracket taxable amounts and tax, gross IRG, first
abattement, IRG after first abattement, final IRG, net payable) as an
`IrgCalculationBreakdown` struct — the CLI layer displays each field as a
separate step.

## Run tests

```bash
cmake --build build --target irg_calc_tests
./build/tests/irg_calc_tests
```

Test cases encode hand-verified results for 30,000 DA (exempt), 32,000 DA
(smoothing zone), and 52,000 DA (standard case).

abattement are implemented; the additional disabled/retired-worker
abattements described in Article 104 are not yet implemented.
