#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "irg/core/irg_calculator.hpp"

namespace {

int failureCount = 0;

void expectNear(double actual, double expected, const char* testName) {
    constexpr double kTolerance = 0.01;
    if (std::fabs(actual - expected) > kTolerance) {
        std::printf("FAIL: %s (expected %.3f, got %.3f)\n", testName, expected, actual);
        ++failureCount;
    } else {
        std::printf("PASS: %s\n", testName);
    }
}

void incomeAtOrBelow30000IsFullyExempt() {
    irg::core::IrgCalculator calculator;
    auto breakdown = calculator.calculateMonthlyIrgBreakdown(30000.0);
    expectNear(breakdown.finalIrg, 0.0, "incomeAtOrBelow30000IsFullyExempt (finalIrg)");
    expectNear(breakdown.netPayable, 30000.0, "incomeAtOrBelow30000IsFullyExempt (netPayable)");
}

void incomeOf32000UsesSecondAbattementFormula() {
    irg::core::IrgCalculator calculator;
    auto breakdown = calculator.calculateMonthlyIrgBreakdown(32000.0);
    expectNear(breakdown.grossIrg, 2760.0, "incomeOf32000 (grossIrg)");
    expectNear(breakdown.firstAbattement, 1104.0, "incomeOf32000 (firstAbattement)");
    expectNear(breakdown.finalIrg, 957.85, "incomeOf32000 (finalIrg)");
    expectNear(breakdown.netPayable, 31042.15, "incomeOf32000 (netPayable)");
}

void incomeOf52000UsesStandardRules() {
    irg::core::IrgCalculator calculator;
    auto breakdown = calculator.calculateMonthlyIrgBreakdown(52000.0);
    expectNear(breakdown.grossIrg, 7840.0, "incomeOf52000 (grossIrg)");
    expectNear(breakdown.firstAbattement, 1500.0, "incomeOf52000 (firstAbattement, capped)");
    expectNear(breakdown.finalIrg, 6340.0, "incomeOf52000 (finalIrg)");
    expectNear(breakdown.netPayable, 45660.0, "incomeOf52000 (netPayable)");
}

}  // namespace

int main() {
    incomeAtOrBelow30000IsFullyExempt();
    incomeOf32000UsesSecondAbattementFormula();
    incomeOf52000UsesStandardRules();

    if (failureCount > 0) {
        std::printf("\n%d test(s) failed.\n", failureCount);
        return EXIT_FAILURE;
    }
    std::printf("\nAll tests passed.\n");
    return EXIT_SUCCESS;
}
