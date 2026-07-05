#pragma once

#include <string>

namespace irg::cli {

void printLine(const std::string &text);
void printLineWithDelay(const std::string &text, int delayMs = 2500);
void printSeparator();

int promptForMenuChoice(const std::string &promptText,
                        const std::string &errorText, int minChoice,
                        int maxChoice);

double promptForNonNegativeNumber(const std::string &promptText,
                                  const std::string &errorText);

std::string formatAmount(double amount);

} // namespace irg::cli
