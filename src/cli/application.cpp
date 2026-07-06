#include "irg/cli/application.hpp"
#include "irg/cli/console_io.hpp"

#include <iostream>
#include <limits>

namespace irg::cli {

using irg::core::BracketContribution;
using irg::core::IrgCalculationBreakdown;
using irg::i18n::Language;
using irg::i18n::TextKey;
using irg::i18n::Translator;

namespace {

Language languageFromMenuChoice(int choice) {
  switch (choice) {
  case 1:
    return Language::English;
  default:
    return Language::French;
  }
}

} // namespace

Language Application::selectLanguage() {
  printLine("1) English   2) French");
  const int choice = promptForMenuChoice(
      "Select / Choisissez:", "Invalid choice. Please enter 1 or 2.", 1, 2);
  return languageFromMenuChoice(choice);
}

double Application::collectMonthlyNetIncome(const Translator &translator) {
  return promptForNonNegativeNumber(
      translator.translate(TextKey::EnterMonthlyIncomePrompt),
      translator.translate(TextKey::InvalidNumberInput));
}

void Application::displayFullyExemptResult(
    const Translator &translator, const IrgCalculationBreakdown &breakdown) {
  printSeparator();
  printLineWithDelay(translator.translate(TextKey::NetImposableLabel) + " " +
                     formatAmount(breakdown.netImposable) + " " +
                     translator.translate(TextKey::CurrencyUnitLabel));
  printLineWithDelay(translator.translate(TextKey::FullyExemptMessage));
  printSeparator();
}

void Application::displayBracketContributionLine(
    const Translator &translator, const BracketContribution &contribution) {
  const std::string currency = translator.translate(TextKey::CurrencyUnitLabel);

  const std::string upperBoundText =
      irg::core::IrgCalculator::isOpenEndedBracket(
          {contribution.lowerBound, contribution.upperBound, contribution.rate})
          ? translator.translate(TextKey::BracketRangeOpenEndedLabel)
          : formatAmount(contribution.upperBound) + " " + currency;

  // e.g. [40000 DA - 80000 DA] @ 27%: taxable: 52000 - 40000 = 12000 @ 27% =
  // 3240.00 DA
  printLineWithDelay(
      "  [" + formatAmount(contribution.lowerBound) + " " + currency + " - " +
      upperBoundText + "] @ " + formatAmount(contribution.rate * 100.0) +
      "%: " + translator.translate(TextKey::TaxableAmountLabel) + ": " +
      formatAmount(contribution.lowerBound + contribution.taxableAmount) +
      " - " + formatAmount(contribution.lowerBound) + " = " +
      formatAmount(contribution.taxableAmount) + " @ " +
      formatAmount(contribution.rate * 100.0) +
      "% = " + formatAmount(contribution.taxAmount) + " " + currency);
}

void Application::displayBracketBreakdown(
    const Translator &translator, const IrgCalculationBreakdown &breakdown) {
  printSeparator();
  printLineWithDelay(translator.translate(TextKey::BracketBreakdownHeader));
  printLine("");
  for (const auto &contribution : breakdown.bracketContributions) {
    displayBracketContributionLine(translator, contribution);
  }
}

void Application::displayAbattementSteps(
    const Translator &translator, const IrgCalculationBreakdown &breakdown) {
  const std::string currency = translator.translate(TextKey::CurrencyUnitLabel);

  const double rawAbattement = breakdown.grossIrg * 0.40;
  const bool abattementWasCapped = rawAbattement != breakdown.firstAbattement;

  printSeparator();
  printLineWithDelay(translator.translate(TextKey::GrossIrgLabel) + " " +
                     formatAmount(breakdown.grossIrg) + " " + currency);
  printLine("");

  // e.g. First abattement: 7840 x 0.40 = 3136.00 DA > 1500 DA, applying cap of
  // 1500 DA
  std::string abattementLine =
      translator.translate(TextKey::FirstAbattementLabel) + " " +
      formatAmount(breakdown.grossIrg) +
      " x 0.40 = " + formatAmount(rawAbattement) + " " + currency;
  if (abattementWasCapped) {
    abattementLine +=
        (rawAbattement > breakdown.firstAbattement ? " > " : " < ") +
        formatAmount(breakdown.firstAbattement) + " " + currency + ", " +
        translator.translate(TextKey::FirstAbattementCapAppliedLabel) + " " +
        formatAmount(breakdown.firstAbattement) + " " + currency;
  }
  printLineWithDelay(abattementLine);

  // e.g. IRG after first abattement: 7840.00 - 1500.00 = 6340.00 DA
  printLineWithDelay(
      translator.translate(TextKey::IrgAfterFirstAbattementLabel) + " " +
      formatAmount(breakdown.grossIrg) + " - " +
      formatAmount(breakdown.firstAbattement) + " = " +
      formatAmount(breakdown.irgAfterFirstAbattement) + " " + currency);

  if (breakdown.isInSecondAbattementZone) {
    printLine("");
    printLineWithDelay(
        translator.translate(TextKey::SecondAbattementZoneNotice));

    // e.g. Second abattement formula: IRG = 1656.00 x (137/51) - (27925/8) =
    // 957.85 DA
    printLineWithDelay(
        translator.translate(TextKey::SecondAbattementFormulaLine) + " " +
        formatAmount(breakdown.irgAfterFirstAbattement) +
        " x (137/51) - (27925/8) = " + formatAmount(breakdown.finalIrg) + " " +
        currency);
  }
}

void Application::displayFinalResult(const Translator &translator,
                                     const IrgCalculationBreakdown &breakdown) {
  const std::string currency = translator.translate(TextKey::CurrencyUnitLabel);

  printSeparator();
  printLineWithDelay(translator.translate(TextKey::FinalIrgLabel) + " " +
                     formatAmount(breakdown.finalIrg) + " " + currency);
  printLine("");
  // e.g. Net payable = 52000.00 DA - 6340.00 DA = 45660.00 DA
  printLineWithDelay(translator.translate(TextKey::NetPayableLabel) + ": " +
                     formatAmount(breakdown.netImposable) + " - " +
                     formatAmount(breakdown.finalIrg) + " = " +
                     formatAmount(breakdown.netPayable) + " " + currency);
  printSeparator();
}

void Application::displayBreakdown(const Translator &translator,
                                   const IrgCalculationBreakdown &breakdown) {
  printLine("");
  printLineWithDelay(translator.translate(TextKey::NetImposableLabel) + " " +
                     formatAmount(breakdown.netImposable) + " " +
                     translator.translate(TextKey::CurrencyUnitLabel));
  printLineWithDelay(translator.translate(TextKey::NetImposableFloorNote));

  if (breakdown.isFullyExempt) {
    displayFullyExemptResult(translator, breakdown);
    return;
  }

  displayBracketBreakdown(translator, breakdown);
  displayAbattementSteps(translator, breakdown);
  displayFinalResult(translator, breakdown);
}

void Application::run() {
  const Language language = selectLanguage();
  Translator translator(language);

  printLine("");
  const double monthlyNetIncome = collectMonthlyNetIncome(translator);
  const auto breakdown =
      calculator_.calculateMonthlyIrgBreakdown(monthlyNetIncome);

  displayBreakdown(translator, breakdown);
  printLine("");
  printLine(translator.translate(TextKey::ExitMessage));
  printLine("Contact: malek.boulellou@proton.me");
  printLine("");
}
#ifdef _WIN32
printLine("Press Enter to exit...");
std::cin.get();
#endif
} // namespace irg::cli
