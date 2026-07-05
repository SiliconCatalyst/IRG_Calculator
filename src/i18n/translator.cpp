#include "irg/i18n/translator.hpp"

namespace irg::i18n {

namespace {

std::string translateEnglish(TextKey key) {
  switch (key) {
  case TextKey::LanguageSelectPrompt:
    return "Select a language: 1) English 1) French";
  case TextKey::InvalidLanguageChoice:
    return "Invalid choice. Please enter 1 or 2";
  case TextKey::EnterMonthlyIncomePrompt:
    return "Enter your monthly net imposable income (DA):";
  case TextKey::InvalidNumberInput:
    return "Invalid input. Please enter a positive number.";
  case TextKey::NetImposableLabel:
    return "Net imposable income:";
  case TextKey::NetImposableFloorNote:
    return "Note: decimal part is dropped before applying brackets.";
  case TextKey::FullyExemptMessage:
    return "Income is at or below 30,000 DA: fully exempt from IRG.";
  case TextKey::BracketBreakdownHeader:
    return "Bracket-by-bracket breakdown:";
  case TextKey::BracketRangeOpenEndedLabel:
    return "and above";
  case TextKey::TaxableAmountLabel:
    return "taxable amount";
  case TextKey::TaxForBracketLabel:
    return "tax";
  case TextKey::GrossIrgLabel:
    return "Gross IRG (sum of all brackets):";
  case TextKey::FirstAbattementLabel:
    return "First abattement (40%, capped between 1,000 and 1,500 DA):";
  case TextKey::FirstAbattementCapAppliedLabel:
    return "applying cap of";
  case TextKey::IrgAfterFirstAbattementLabel:
    return "IRG after first abattement:";
  case TextKey::SecondAbattementZoneNotice:
    return "Income is in the 30,001-35,000 DA smoothing zone:";
  case TextKey::SecondAbattementFormulaLine:
    return "Second abattement formula: IRG =";
  case TextKey::FinalIrgLabel:
    return "Final IRG:";
  case TextKey::NetPayableLabel:
    return "Net payable salary";
  case TextKey::CurrencyUnitLabel:
    return "DA";
  case TextKey::ExitMessage:
    return "Thank you for using the IRG calculator.";
  }
  return "";
}

std::string translateFrench(TextKey key) {
  switch (key) {
  case TextKey::LanguageSelectPrompt:
    return "Sélectionnez une langue : 1) Arabe 2) Anglais 3) Français";
  case TextKey::InvalidLanguageChoice:
    return "Choix invalide. Veuillez saisir 1, 2 ou 3.";
  case TextKey::EnterMonthlyIncomePrompt:
    return "Entrez votre revenu mensuel net imposable (DA) :";
  case TextKey::InvalidNumberInput:
    return "Entrée invalide. Veuillez saisir un nombre positif.";
  case TextKey::NetImposableLabel:
    return "Revenu net imposable :";
  case TextKey::NetImposableFloorNote:
    return "Note : la partie décimale est arrondie à l'inférieur avant "
           "l'application du barème.";
  case TextKey::FullyExemptMessage:
    return "Le revenu est inférieur ou égal à 30 000 DA : totalement exonéré "
           "de l'IRG.";
  case TextKey::BracketBreakdownHeader:
    return "Détail par tranche :";
  case TextKey::BracketRangeOpenEndedLabel:
    return "et plus";
  case TextKey::TaxableAmountLabel:
    return "montant imposable";
  case TextKey::TaxForBracketLabel:
    return "taxe";
  case TextKey::GrossIrgLabel:
    return "IRG Brut (somme de toutes les tranches) :";
  case TextKey::FirstAbattementLabel:
    return "Premier abattement (40%, plafonné entre 1 000 et 1 500 DA) :";
  case TextKey::FirstAbattementCapAppliedLabel:
    return "application du plafond de";
  case TextKey::IrgAfterFirstAbattementLabel:
    return "IRG après le premier abattement :";
  case TextKey::SecondAbattementZoneNotice:
    return "Revenu dans la zone d'abattement 30 001-35 000 DA :";
  case TextKey::SecondAbattementFormulaLine:
    return "Deuxieme abattement (formule) : IRG =";
  case TextKey::FinalIrgLabel:
    return "IRG Final :";
  case TextKey::NetPayableLabel:
    return "Salaire net à payer";
  case TextKey::CurrencyUnitLabel:
    return "DA";
  case TextKey::ExitMessage:
    return "Merci d'avoir utilisé le calculateur d'IRG.";
  }
  return "";
}

} // namespace

Translator::Translator(Language language) : language_(language) {}

void Translator::setLanguage(Language language) { language_ = language; }

Language Translator::currentLanguage() const { return language_; }

std::string Translator::translate(TextKey key) const {
  switch (language_) {
  case Language::English:
    return translateEnglish(key);
  case Language::French:
    return translateFrench(key);
  }
  return "";
}

} // namespace irg::i18n
