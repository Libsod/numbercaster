#include "app.hpp" // Include namespaces and their methods signatures
#include <fmt/core.h>
#include <iostream>  // Include std::cin, std::cerr, std::streamsize
#include <limits>    // Include std::numeric_limits
#include <stdexcept> // Include std::out_of_range

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h> // For Unix-specific system calls
#endif

/* Clears the screen depending on the platform */
void ConsoleUtils::clearScreen() {
#if defined(_WIN32)
  std::system("cls"); // Windows clear command

#elif defined(__linux__) || defined(__APPLE__)
  std::system("clear"); // Linux/Unix/macOS clear command
#endif
}

/* Prompts the user to press any key and waits for input, disabling canonical
 * mode temporarily */
void ConsoleUtils::waitForKeyPress() {
  fmt::print("Press any key to continue...");
  disableCanonicalMode(); // Disable canonical mode to capture input immediately
  getchar();              // Wait for key press
  restoreTerminalSettings(); // Restore original terminal settings
}

/* Clears the input buffer, discarding any excess input that might be left in
 * the buffer */
void ConsoleUtils::clearInputBuffer() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/* Prints an error message to the console using the provided string */
void ConsoleUtils::printError(const str &message) {
  fmt::print(stderr, "Error: {}\n", message);
}

#if defined(_WIN32)
/* Disables canonical mode on Windows by modifying the console's input mode
 * settings */
void ConsoleUtils::disableCanonicalMode() {
  hConsole = GetStdHandle(STD_INPUT_HANDLE); // Get handle to standard input
  GetConsoleMode(hConsole, &mode);           // Retrieve current console mode
  SetConsoleMode(hConsole,
                 mode &
                     ~(ENABLE_LINE_INPUT |
                       ENABLE_ECHO_INPUT)); // Disable line and echo input modes
}

/* Restores the console's input mode to its original state on Windows */
void ConsoleUtils::restoreTerminalSettings() { SetConsoleMode(hConsole, mode); }

#elif defined(__linux__) || defined(__APPLE__)
/* Disables canonical mode on Unix-based systems by modifying terminal
 * attributes */
void ConsoleUtils::disableCanonicalMode() {
  tcgetattr(STDIN_FILENO, &oldt);          // Get current terminal attributes
  newt = oldt;                             // Copy to new settings
  newt.c_lflag &= ~(ICANON | ECHO);        // Disable canonical mode and echo
  tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings
}

/* Restores the terminal attributes to their original state on Unix-based
 * systems */
void ConsoleUtils::restoreTerminalSettings() {
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
#endif

/* Prompts the user for input with a given prompt, returning an optional value
 * if valid input is entered */
Option<f64> NumberCast::getInput(const str &prompt) {
  f64 value;
  fmt::print("{}", prompt); // Print the prompt

  if (std::cin >> value) {
    return value; // Return the value if input is valid
  } else {
    std::cin.clear();                 // Clear input error state
    ConsoleUtils::clearScreen();      // Clear the screen
    ConsoleUtils::clearInputBuffer(); // Clear input buffer
    return std::nullopt;              // Return empty optional on invalid
  }
}

/* Checks if the given floating-point value is within the range of a 32-bit
 * integer, throws an exception if out of bounds */
void NumberCast::checkBounds(f64 value) {
  if (value < std::numeric_limits<i32>::min() ||
      value > std::numeric_limits<i32>::max()) {
    ConsoleUtils::clearScreen(); // Clear the screen if value is out of bounds
    throw std::out_of_range(fmt::format(
        "{} is out of bounds for i32",
        value)); // Throw an exception with a formatted error message
  }
}

/* Main function for the Application class, handling the core input-output loop
 * and logic */
void Application::run() {
  /* Using namespaces for convenience */
  using namespace NumberCast;
  using namespace ConsoleUtils;

  while (true) {
    clearScreen(); // Clear the screen at the start of each loop iteration
    try {
      /* Prompt for and get the first number */
      auto aOpt = getInput("Enter the first number: ");

      if (!aOpt) { // Check for invalid input
        printError("Invalid input for the first number.");
        waitForKeyPress(); // Wait for user input before continuing
        continue;
      }

      const f64 a = *aOpt; // Extract the value from the optional

      clearInputBuffer(); // Clear the input buffer after receiving the input
      checkBounds(a);     // Check if the value is within bounds

      /* Prompt for and get the second number */
      auto bOpt = getInput("Enter the second number: ");

      if (!bOpt) { // Check for invalid input
        printError("Invalid input for the second number.");
        waitForKeyPress(); // Wait for user input before continuing
        continue;
      }

      const f64 b = *bOpt; // Extract the value from the optional

      clearInputBuffer(); // Clear the input buffer after receiving the input
      checkBounds(b);     // Check if the value is within bounds

      /* Safely cast both numbers to 32-bit integers */
      i32 aInt32 = static_cast<i32>(a);
      i32 bInt32 = static_cast<i32>(b);

      clearScreen(); // Clear the screen before displaying the result

      /* Display the safely cast values and the larger of the two numbers */
      fmt::println("Values of safely casted f64 to i32: {}, {}", aInt32,
                   bInt32);
      fmt::println("Bigger value: {}", getMax(aInt32, bInt32));

      waitForKeyPress(); // Wait for user input before restarting the loop

    } catch (const std::out_of_range &err) {
      /* Handle out-of-range error, print error message, and prompt user to
       * enter valid input */
      printError(err.what());
      fmt::println(stderr, "Please enter a value between {} and {}.",
                   std::numeric_limits<i32>::min(),
                   std::numeric_limits<i32>::max());

      waitForKeyPress(); // Wait for user input before restarting the loop
    }
  }
}
