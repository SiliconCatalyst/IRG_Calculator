#include "irg/core/irg_calculator.hpp"

#include <algorithm>

namespace irg::core {

namespace {
constexpr double kOpenEndedSentinel = -1.0;

constexpr double kAbattementRate = 0.40;
constexpr double kAbattementMinimum = 1000.0;
constexpr double kAbattementMaximum = 1500.0;

constexpr double kFullExemptionThreshold = 30000.0;
constexpr double kSecondAbattementZoneUpperBound = 35000.0;
constexpr double kSecondAbattementMultiplier = 137.0 / 51.0;
constexpr double kSecondAbattementSubtrahend = 27925.0 / 8.0;
} // namespace

IrgCalculator::IrgCalculator() : brackets_(defaultBrackets()) {}

IrgCalculator::IrgCalculator(std::vector<IrgTaxBracket> brackets)
    : brackets_(std::move(brackets)) {}

bool IrgCalculator::isOpenEndedBracket(const IrgTaxBracket &bracket) {
  return bracket.upperBound == kOpenEndedSentinel;
}

std::vector<BracketContribution> IrgCalculator::computeBracketContributions(
    double remainingIncome, const std::vector<IrgTaxBracket> &brackets) {
  std::vector<BracketContribution> contributions;

  for (const auto &bracket : brackets) {
    if (remainingIncome <= bracket.lowerBound) {
      break;
    }

    const double bracketCeiling =
        isOpenEndedBracket(bracket) ? remainingIncome : bracket.upperBound;
    const double taxableAmount =
        std::min(remainingIncome, bracketCeiling) - bracket.lowerBound;
    const double taxAmount = taxableAmount * bracket.rate;

    contributions.push_back({bracket.lowerBound, bracket.upperBound,
                             bracket.rate, taxableAmount, taxAmount});
  }

  return contributions;
}

double IrgCalculator::calculateAbattement(double grossIrg) {
  if (grossIrg <= 0.0) {
    return 0.0;
  }
  const double rawAbattement = grossIrg * kAbattementRate;
  return std::clamp(rawAbattement, kAbattementMinimum, kAbattementMaximum);
}

bool IrgCalculator::isFullyExempt(double monthlyNetIncome) {
  return monthlyNetIncome <= kFullExemptionThreshold;
}

bool IrgCalculator::isInSecondAbattementZone(double monthlyNetIncome) {
  return monthlyNetIncome > kFullExemptionThreshold &&
         monthlyNetIncome <= kSecondAbattementZoneUpperBound;
}

double
IrgCalculator::applySecondAbattementFormula(double irgAfterFirstAbattement) {
  return irgAfterFirstAbattement * kSecondAbattementMultiplier -
         kSecondAbattementSubtrahend;
}

IrgCalculationBreakdown
IrgCalculator::calculateMonthlyIrgBreakdown(double monthlyNetIncome) const {
  IrgCalculationBreakdown breakdown;
  breakdown.netImposable = std::max(0.0, monthlyNetIncome);

  if (isFullyExempt(breakdown.netImposable)) {
    breakdown.isFullyExempt = true;
    breakdown.netPayable = breakdown.netImposable;
    return breakdown;
  }

  breakdown.bracketContributions =
      computeBracketContributions(breakdown.netImposable, brackets_);

  for (const auto &contribution : breakdown.bracketContributions) {
    breakdown.grossIrg += contribution.taxAmount;
  }

  breakdown.firstAbattement = calculateAbattement(breakdown.grossIrg);
  breakdown.irgAfterFirstAbattement =
      std::max(0.0, breakdown.grossIrg - breakdown.firstAbattement);

  breakdown.isInSecondAbattementZone =
      isInSecondAbattementZone(breakdown.netImposable);
  breakdown.finalIrg =
      breakdown.isInSecondAbattementZone
          ? std::max(0.0, applySecondAbattementFormula(
                              breakdown.irgAfterFirstAbattement))
          : breakdown.irgAfterFirstAbattement;

  breakdown.netPayable = breakdown.netImposable - breakdown.finalIrg;

  return breakdown;
}

// Bareme IRG 2022 (Article 104 CIDTA), still in force as of 2026.
std::vector<IrgTaxBracket> IrgCalculator::defaultBrackets() {
  return {{0.0, 20000.0, 0.00},       {20000.0, 40000.0, 0.23},
          {40000.0, 80000.0, 0.27},   {80000.0, 160000.0, 0.30},
          {160000.0, 320000.0, 0.33}, {320000.0, kOpenEndedSentinel, 0.35}};
}

} // namespace irg::core
