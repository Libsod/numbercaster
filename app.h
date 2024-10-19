#pragma once

#include "types.hpp"
#include <format>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>

#if defined(_WIN32)
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
using Termios = struct termios;
#endif

class ConsoleUtils {
  public:
    static void clearScreen() {
#if defined(_WIN32)
      std::system("cls");
#elif defined(__linux__) || defined(__APPLE__)
      std::system("clear");
#endif
    }

    void waitForKeyPress() {
      std::print("Press any key to continue...");
      disableCanonicalMode();
      getchar();
      restoreTerminalSettings();
    }

    static void clearInputBuffer() {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    static void printError(const std::string &message) {
      std::cerr << std::format("Error: {}\n", message);
    }

  private:
#if defined(_WIN32)
    HANDLE hConsole;
    DWORD mode;

    void disableCanonicalMode() {
      hConsole = GetStdHandle(STD_INPUT_HANDLE);
      GetConsoleMode(hConsole, &mode);
      SetConsoleMode(hConsole, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
    }

    void restoreTerminalSettings() { SetConsoleMode(hConsole, mode); }
#else
    Termios oldt, newt;

    void disableCanonicalMode() {
      tcgetattr(STDIN_FILENO, &oldt);
      newt = oldt;
      newt.c_lflag &= ~(ICANON | ECHO);
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }

    void restoreTerminalSettings() { tcsetattr(STDIN_FILENO, TCSANOW, &oldt); }
#endif
};

class NumberCast {
  public:
    Optional<f64> getInput(const str &prompt) const {
      f64 value;
      std::print("{}", prompt);

      if (std::cin >> value) {
        return value;
      } else {
        std::cin.clear();
        consoleUtils.clearScreen();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::nullopt;
      }
    }

    void checkBounds(f64 value) const {
      if (value < std::numeric_limits<i32>::min() ||
          value > std::numeric_limits<i32>::max()) {
        consoleUtils.clearScreen();
        throw std::out_of_range(
            std::format("{} is out of bounds for i32", value));
      }
    }

    template <typename T> T getMax(T a, T b) const { return (a > b) ? a : b; }

  private:
    static ConsoleUtils consoleUtils;
};

class Application {
  public:
    void run() {
      ConsoleUtils consoleUtils;

      while (true) {
        consoleUtils.clearScreen();
        try {
          auto aOpt = numberCast.getInput("Enter the first number: ");

          if (!aOpt) {
            consoleUtils.printError("Invalid input for the first number.");
            consoleUtils.waitForKeyPress();
            continue;
          }

          f64 a = *aOpt;

          consoleUtils.clearInputBuffer();
          numberCast.checkBounds(a);

          auto bOpt = numberCast.getInput("Enter the second number: ");

          if (!bOpt) {
            consoleUtils.printError("Invalid input for the second number.");
            consoleUtils.waitForKeyPress();
            continue;
          }

          f64 b = *bOpt;

          consoleUtils.clearInputBuffer();
          numberCast.checkBounds(b);

          i32 aInt32 = static_cast<i32>(a);
          i32 bInt32 = static_cast<i32>(b);

          consoleUtils.clearScreen();

          std::println("Values of safely casted f64 to i32: {}, {}", aInt32,
                       bInt32);
          std::println("Bigger value: {}", numberCast.getMax(aInt32, bInt32));

          consoleUtils.waitForKeyPress();

        } catch (const std::out_of_range &err) {
          consoleUtils.printError(err.what());
          std::cerr << std::format("Please enter a value between {} and {}.\n",
                                   std::numeric_limits<i32>::min(),
                                   std::numeric_limits<i32>::max());

          consoleUtils.waitForKeyPress();
        }
      }
    }

  private:
    NumberCast numberCast;
};
