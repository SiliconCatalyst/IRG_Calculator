#include "irg/cli/console_io.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <thread>

namespace irg::cli {

void printLine(const std::string &text) { std::cout << text << std::endl; }

void printLineWithDelay(const std::string &text, int delayMs) {
  std::cout << text << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
}

void printSeparator() { printLine("----------------------------------------"); }

namespace {

void clearBadInputState() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

} // namespace

int promptForMenuChoice(const std::string &promptText,
                        const std::string &errorText, int minChoice,
                        int maxChoice) {
  while (true) {
    std::cout << promptText << " ";
    int choice = 0;
    std::cin >> choice;

    if (std::cin.fail() || choice < minChoice || choice > maxChoice) {
      clearBadInputState();
      printLine(errorText);
      continue;
    }

    clearBadInputState();
    return choice;
  }
}

double promptForNonNegativeNumber(const std::string &promptText,
                                  const std::string &errorText) {
  while (true) {
    std::cout << promptText << " ";
    double value = 0.0;
    std::cin >> value;

    if (std::cin.fail() || value < 0.0) {
      clearBadInputState();
      printLine(errorText);
      continue;
    }

    clearBadInputState();
    return value;
  }
}

std::string formatAmount(double amount) {
  std::ostringstream stream;
  stream << std::fixed << std::setprecision(2) << amount;
  return stream.str();
}

} // namespace irg::cli
