#pragma once

#include <vector>

namespace irg::core {

// One progressive tax bracket: income between [lowerBound, upperBound)
// is taxed at `rate` (e.g. 0.23 for 23%).
// The topmost bracket is open-ended; see IrgCalculator::isOpenEndedBracket().
struct IrgTaxBracket {
    double lowerBound;
    double upperBound;
    double rate;
};

// The result of applying one bracket to a given income: how much of the
// income fell in this bracket, and how much tax that portion generated.
// Exists so the CLI can print a tranche-by-tranche breakdown for students.
struct BracketContribution {
    double lowerBound;
    double upperBound;     // kOpenEndedSentinel (-1) for the open-ended bracket
    double rate;
    double taxableAmount;  // portion of income taxed at this bracket's rate
    double taxAmount;      // taxableAmount * rate
};

// The full trail of a single IRG calculation, step by step, exactly as a
// student would write it out by hand. Nothing here is display text or
// language-specific - it's pure numbers and flags for the CLI to translate
// and print.
struct IrgCalculationBreakdown {
    double netImposable = 0.0;
    bool isFullyExempt = false;

    std::vector<BracketContribution> bracketContributions;
    double grossIrg = 0.0;

    double firstAbattement = 0.0;
    double irgAfterFirstAbattement = 0.0;

    bool isInSecondAbattementZone = false;
    double finalIrg = 0.0;

    double netPayable = 0.0;
};

// Pure business logic: turns a monthly net imposable income into a full
// IRG calculation breakdown using the official progressive tax brackets,
// the standard 40% abattement (clamped 1000-1500 DA), and the second
// smoothing abattement for the 30,001-35,000 DA band. Deliberately has no
// knowledge of the terminal, prompts, or which language is active.
class IrgCalculator {
public:
    IrgCalculator();
    explicit IrgCalculator(std::vector<IrgTaxBracket> brackets);

    IrgCalculationBreakdown calculateMonthlyIrgBreakdown(double monthlyNetIncome) const;

    static bool isOpenEndedBracket(const IrgTaxBracket& bracket);

private:
    std::vector<IrgTaxBracket> brackets_;

    static std::vector<IrgTaxBracket> defaultBrackets();

    static std::vector<BracketContribution> computeBracketContributions(
        double remainingIncome, const std::vector<IrgTaxBracket>& brackets);

    static double calculateAbattement(double grossIrg);

    static bool isFullyExempt(double monthlyNetIncome);
    static bool isInSecondAbattementZone(double monthlyNetIncome);
    static double applySecondAbattementFormula(double irgAfterFirstAbattement);
};

}
