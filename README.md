# IRG Calculator

Terminal application that calculates IRG (Impot sur le Revenu Global) with tri-lingual support: Arabic, English & French.

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
│   ├── core/irg_calculator.hpp   # tax brackets & calculation
│   ├── i18n/language.hpp         # supported languages
│   ├── i18n/translator.hpp       # text-key -> string lookup
│   └── cli/console_io.hpp        # generic prompt/print helpers
│   └── cli/application.hpp       # orchestrates a full run
├── src/                          # matching .cpp files
├── tests/                        # unit tests for irg_core
└── README.md
```

Rule of thumb: `core/` never includes `<iostream>`, and `cli/` never contains
tax-bracket numbers or translated strings inline — everything user-facing
goes through `Translator::translate(TextKey)`.

## Build & run

```bash
cmake -S . -B build
cmake --build build
./build/irg_calc
```

## Run tests

```bash
cmake --build build --target irg_calc_tests
./build/tests/irg_calc_tests
```

## Known placeholders (must fix before real use)

- `IrgCalculator::defaultBrackets()` in `src/core/irg_calculator.cpp` contains
  placeholder thresholds/rates, not the verified official IRG bracket table.
- Arabic strings in `src/i18n/translator.cpp` are `TODO_AR_*` placeholders.
- French/English strings avoid accented characters for terminal-encoding
  safety; revisit if your target terminal is confirmed UTF-8 clean.
