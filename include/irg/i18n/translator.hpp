#pragma once

#include <string>

#include "irg/i18n/language.hpp"

namespace irg::i18n {

enum class TextKey {
  LanguageSelectPrompt,
  InvalidLanguageChoice,
  EnterAnnualIncomePrompt,
  InvalidNumberInput,
  IrgResultLabel,
  NetIncomeAfterIrgLabel,
  ExitMessage
};

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
