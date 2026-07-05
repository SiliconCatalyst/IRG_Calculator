# IRG Calculator

A terminal application that calculates Algerian **IRG (Impot sur le Revenu Global)**
on a monthly net imposable salary, showing every step of the calculation so
students and lecturers can verify the result by hand.

UI available in English and French.

---

## Download and run

Go to the [Releases](../../releases) page and download the binary for your OS.

### Linux

```bash
chmod +x irg_calc-linux
./irg_calc-linux
```

### Windows

Double-click `irg_calc-windows.exe`, or run it from a terminal:

```
irg_calc-windows.exe
```

If Windows Defender shows a warning, click **More info** then **Run anyway** —
the binary is unsigned but safe. This warning appears because the executable
is not code-signed with a Microsoft certificate.

### macOS

```bash
chmod +x irg_calc-macos
xattr -d com.apple.quarantine ./irg_calc-macos
./irg_calc-macos
```

The `xattr` command is required once to remove Apple's quarantine flag on
unsigned binaries downloaded from the internet. Without it, Gatekeeper will
block the app from running.

---

## What it does

1. Asks you to select a language: `1` English, `2` French.
2. Asks for your monthly net imposable income in DA.
3. Prints the full step-by-step IRG calculation, including:
   - The decimal part being dropped before brackets are applied.
   - Each bracket showing the exact arithmetic: income slice, rate, and tax produced.
   - The gross IRG (sum of all bracket taxes).
   - The first abattement of 40%, showing whether the cap of 1,000 or 1,500 DA was applied.
   - The second abattement smoothing formula, if applicable.
   - The final IRG.
   - The net payable salary as a subtraction.

### Example output (52,000 DA)

```
1) English   2) French
Select / Choisissez : 1

Enter your monthly net imposable income (DA): 52000

Net imposable income: 52000.00 DA
Note: decimal part is dropped before applying brackets.
----------------------------------------
Bracket-by-bracket breakdown:

  [0.00 DA - 20000.00 DA] @ 0.00%: taxable amount: 20000.00 - 0.00 = 20000.00 @ 0.00% = 0.00 DA
  [20000.00 DA - 40000.00 DA] @ 23.00%: taxable amount: 40000.00 - 20000.00 = 20000.00 @ 23.00% = 4600.00 DA
  [40000.00 DA - 80000.00 DA] @ 27.00%: taxable amount: 52000.00 - 40000.00 = 12000.00 @ 27.00% = 3240.00 DA
----------------------------------------
Gross IRG (sum of all brackets): 7840.00 DA

First abattement (40%, capped between 1,000 and 1,500 DA): 7840.00 x 0.40 = 3136.00 DA > 1500.00 DA, applying cap of 1500.00 DA
IRG after first abattement: 7840.00 - 1500.00 = 6340.00 DA
----------------------------------------
Final IRG: 6340.00 DA

Net payable salary: 52000.00 - 6340.00 = 45660.00 DA
----------------------------------------
```

---

## Calculation method

Based on **Article 104 of the CIDTA**, barème IRG 2022, in force as of 2026.

### Step 0 — Floor the net imposable income

The decimal part is dropped before any bracket is applied.
Example: 52,001.75 DA is treated as 52,001 DA for bracket purposes, but the
original value is used for the final net payable subtraction.

### Step 1 — Apply the progressive brackets

| Tranche (DA/month) | Rate |
| ------------------ | ---- |
| 0 – 20,000         | 0%   |
| 20,001 – 40,000    | 23%  |
| 40,001 – 80,000    | 27%  |
| 80,001 – 160,000   | 30%  |
| 160,001 – 320,000  | 33%  |
| Above 320,000      | 35%  |

Only the income slice that falls within each bracket is taxed at that
bracket's rate — not the full income. The sum of all bracket taxes is the
**gross IRG**.

### Step 2 — First abattement (all incomes above 30,000 DA)

```
raw abattement = gross IRG x 40%
```

The result is then clamped:

- If raw abattement < 1,000 DA → use 1,000 DA
- If raw abattement > 1,500 DA → use 1,500 DA
- Otherwise → use the raw value

```
IRG after first abattement = gross IRG - abattement
```

### Step 3 — Exemption and smoothing zone rules

| Monthly net imposable | Rule                                                                                 |
| --------------------- | ------------------------------------------------------------------------------------ |
| ≤ 30,000 DA           | Fully exempt. IRG = 0.                                                               |
| 30,001 – 35,000 DA    | Apply steps 1 and 2, then apply the second abattement smoothing formula (see below). |
| > 35,000 DA           | Apply steps 1 and 2 only.                                                            |

### Second abattement formula (30,001 – 35,000 DA zone only)

```
Final IRG = IRG(after 1st abattement) x (137/51) - (27925/8)
```

This formula is designed so that IRG transitions smoothly from 0 at 30,000 DA
to the standard calculation result at 35,000 DA, with no sudden jump at either
boundary.

### Step 4 — Net payable salary

```
Net payable = net imposable (original, not floored) - final IRG
```

---

## Build from source

Requires CMake 3.16+ and a C++17 compiler (GCC, Clang, or MSVC).

```bash
cmake -S . -B build
cmake --build build
./build/irg_calc
```

### Run tests

```bash
cmake --build build --target irg_calc_tests
./build/tests/irg_calc_tests
```

Test cases encode hand-verified results for 30,000 DA (exempt), 32,000 DA
(smoothing zone), and 52,000 DA (standard case).
