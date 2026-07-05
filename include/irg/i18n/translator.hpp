#pragma once

#include <string>

#include "irg/i18n/language.hpp"

namespace irg::i18n {

// Every piece of user-facing text in the app is a TextKey.
// Nothing in cli/ or core/ should ever contain a hardcoded string
// meant for display - it should ask the Translator for a TextKey instead.
enum class TextKey {
  LanguageSelectPrompt,
  InvalidLanguageChoice,
  EnterMonthlyIncomePrompt,
  InvalidNumberInput,

  NetImposableLabel,
  NetImposableFloorNote,
  FullyExemptMessage,

  BracketBreakdownHeader,
  BracketRangeOpenEndedLabel,
  TaxableAmountLabel,
  TaxForBracketLabel,

  GrossIrgLabel,
  FirstAbattementLabel,
  FirstAbattementCapAppliedLabel,
  IrgAfterFirstAbattementLabel,
  SecondAbattementZoneNotice,
  SecondAbattementFormulaLine,

  FinalIrgLabel,
  NetPayableLabel,
  CurrencyUnitLabel,

  ExitMessage
};

// Translator is a pure lookup service: given a language and a key,
// it returns the string to display. It has no knowledge of tax rules
// (core/) or how input/output happens (cli/).
class Translator {
public:
  explicit Translator(Language language);

  void setLanguage(Language language);
  Language currentLanguage() const;

  std::string translate(TextKey key) const;

private:
  Language language_;
};

} // namespace irg::i18n
