#include "irg/i18n/translator.hpp"

namespace irg::i18n {

namespace {

std::string translateArabic(TextKey key) {
  switch (key) {
  case TextKey::LanguageSelectPrompt:
    return "TODO_AR_LanguageSelectPrompt";
  case TextKey::InvalidLanguageChoice:
    return "TODO_AR_InvalidLanguageChoice";
  case TextKey::EnterAnnualIncomePrompt:
    return "TODO_AR_EnterAnnualIncomePrompt";
  case TextKey::InvalidNumberInput:
    return "TODO_AR_InvalidNumberInput";
  case TextKey::IrgResultLabel:
    return "TODO_AR_IrgResultLabel";
  case TextKey::NetIncomeAfterIrgLabel:
    return "TODO_AR_NetIncomeAfterIrgLabel";
  case TextKey::ExitMessage:
    return "TODO_AR_ExitMessage";
  }
  return "";
}

std::string translateEnglish(TextKey key) {
  switch (key) {
  case TextKey::LanguageSelectPrompt:
    return "Select a language: 1) Arabic 2) English 3) French";
  case TextKey::InvalidLanguageChoice:
    return "Invalid choice. Please enter 1, 2, or 3.";
  case TextKey::EnterAnnualIncomePrompt:
    return "Enter your monthly impossable net income (DZD):";
  case TextKey::InvalidNumberInput:
    return "Invalid input. Please enter a positive number.";
  case TextKey::IrgResultLabel:
    return "IRG due:";
  case TextKey::NetIncomeAfterIrgLabel:
    return "Net income after IRG:";
  case TextKey::ExitMessage:
    return "Thank you for using the IRG calculator.";
  }
  return "";
}

std::string translateFrench(TextKey key) {
  switch (key) {
  case TextKey::LanguageSelectPrompt:
    return "Choisissez une langue : 1) Arabe 2) Anglais 3) Francais";
  case TextKey::InvalidLanguageChoice:
    return "Choix invalide. Veuillez entrer 1, 2 ou 3.";
  case TextKey::EnterAnnualIncomePrompt:
    return "Entrez votre revenu net impossable mensuel (DZD) :";
  case TextKey::InvalidNumberInput:
    return "Entree invalide. Veuillez entrer un nombre positif.";
  case TextKey::IrgResultLabel:
    return "IRG du :";
  case TextKey::NetIncomeAfterIrgLabel:
    return "Revenu net apres IRG :";
  case TextKey::ExitMessage:
    return "Merci d'avoir utilise le calculateur IRG.";
  }
  return "";
}

} // namespace

Translator::Translator(Language language) : language_(language) {}

void Translator::setLanguage(Language language) { language_ = language; }

Language Translator::currentLanguage() const { return language_; }

std::string Translator::translate(TextKey key) const {
  switch (language_) {
  case Language::Arabic:
    return translateArabic(key);
  case Language::English:
    return translateEnglish(key);
  case Language::French:
    return translateFrench(key);
  }
  return "";
}

} // namespace irg::i18n
