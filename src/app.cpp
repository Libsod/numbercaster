#include "app.hpp"
#include <format>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

void ConsoleUtils::clearScreen() {
#if defined(_WIN32)
  std::system("cls");
#elif defined(__linux__) || defined(__APPLE__)
  std::system("clear");
#endif
}

void ConsoleUtils::waitForKeyPress() {
  std::print("Press any key to continue...");
  disableCanonicalMode();
  getchar();
  restoreTerminalSettings();
}

void ConsoleUtils::clearInputBuffer() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ConsoleUtils::printError(const str &message) {
  std::cerr << std::format("Error: {}\n", message);
}

#if defined(_WIN32)
void ConsoleUtils::disableCanonicalMode() {
  hConsole = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(hConsole, &mode);
  SetConsoleMode(hConsole, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
}

void ConsoleUtils::restoreTerminalSettings() { SetConsoleMode(hConsole, mode); }
#else
void ConsoleUtils::disableCanonicalMode() {
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void ConsoleUtils::restoreTerminalSettings() {
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
#endif

Optional<f64> NumberCast::getInput(const str &prompt) {
  f64 value;
  std::print("{}", prompt);

  if (std::cin >> value) {
    return value;
  } else {
    std::cin.clear();
    ConsoleUtils::clearScreen();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return std::nullopt;
  }
}

void NumberCast::checkBounds(f64 value) {
  if (value < std::numeric_limits<i32>::min() ||
      value > std::numeric_limits<i32>::max()) {
    ConsoleUtils::clearScreen();
    throw std::out_of_range(std::format("{} is out of bounds for i32", value));
  }
}

void Application::run() {

  while (true) {
    ConsoleUtils::clearScreen();
    try {
      auto aOpt = NumberCast::getInput("Enter the first number: ");

      if (!aOpt) {
        ConsoleUtils::printError("Invalid input for the first number.");
        ConsoleUtils::waitForKeyPress();
        continue;
      }

      f64 a = *aOpt;

      ConsoleUtils::clearInputBuffer();
      NumberCast::checkBounds(a);

      auto bOpt = NumberCast::getInput("Enter the second number: ");

      if (!bOpt) {
        ConsoleUtils::printError("Invalid input for the second number.");
        ConsoleUtils::waitForKeyPress();
        continue;
      }

      f64 b = *bOpt;

      ConsoleUtils::clearInputBuffer();
      NumberCast::checkBounds(b);

      i32 aInt32 = static_cast<i32>(a);
      i32 bInt32 = static_cast<i32>(b);

      ConsoleUtils::clearScreen();

      std::println("Values of safely casted f64 to i32: {}, {}", aInt32,
                   bInt32);
      std::println("Bigger value: {}", NumberCast::getMax(aInt32, bInt32));

      ConsoleUtils::waitForKeyPress();

    } catch (const std::out_of_range &err) {
      ConsoleUtils::printError(err.what());
      std::cerr << std::format("Please enter a value between {} and {}.\n",
                               std::numeric_limits<i32>::min(),
                               std::numeric_limits<i32>::max());

      ConsoleUtils::waitForKeyPress();
    }
  }
}
