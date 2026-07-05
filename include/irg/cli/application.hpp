#pragma once

#include "irg/core/irg_calculator.hpp"
#include "irg/i18n/translator.hpp"

namespace irg::cli {

// Orchestrates a single run of the terminal app:
// select language -> collect income -> calculate -> display every step.
// This is the only file allowed to #include both irg/core and irg/i18n -
// it is the seam where the two independent branches meet.
class Application {
public:
    void run();

private:
    irg::i18n::Language selectLanguage();
    double collectMonthlyNetIncome(const irg::i18n::Translator& translator);

    void displayBreakdown(const irg::i18n::Translator& translator,
                           const irg::core::IrgCalculationBreakdown& breakdown);

    void displayFullyExemptResult(const irg::i18n::Translator& translator,
                                   const irg::core::IrgCalculationBreakdown& breakdown);

    void displayBracketBreakdown(const irg::i18n::Translator& translator,
                                  const irg::core::IrgCalculationBreakdown& breakdown);

    void displayBracketContributionLine(const irg::i18n::Translator& translator,
                                         const irg::core::BracketContribution& contribution);

    void displayAbattementSteps(const irg::i18n::Translator& translator,
                                 const irg::core::IrgCalculationBreakdown& breakdown);

    void displayFinalResult(const irg::i18n::Translator& translator,
                             const irg::core::IrgCalculationBreakdown& breakdown);

    irg::core::IrgCalculator calculator_;
};

}
