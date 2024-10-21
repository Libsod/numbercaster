/* Including custom shortened types (Yeah, like in Rust) */
#include "types.hpp"

/* Platform-specific includes for terminal handling */
#if defined(__linux__) || defined(__APPLE__)
#include <termios.h>
using Termios = struct termios;
#else
/* Windows-specific macros to avoid conflict with std::max and std::min */
#undef max
#undef min
#include <Windows.h>
#endif

/* Namespace containing utility functions for console handling */
namespace ConsoleUtils {

/* Clears the terminal screen (implementation-specific) */
static void clearScreen();

/* Waits for any key press from the user */
void waitForKeyPress();

/* Clears the terminal input buffer to avoid unwanted input */
static void clearInputBuffer();

/* Prints an error message to the console */
static void printError(const str &message);

#if defined(_WIN32)
/* Windows-specific console handle */
inline HANDLE hConsole;

/* Stores the original console mode */
inline DWORD mode;

/* Disables canonical input mode in the terminal for Windows */
void disableCanonicalMode();

/* Restores terminal settings to their original state on Windows */
void restoreTerminalSettings();

#else
/* Unix-specific terminal settings (old and new modes) */
inline Termios oldt, newt;

/* Disables canonical input mode in the terminal for Unix-based systems */
void disableCanonicalMode();

/* Restores terminal settings to their original state on Unix-based systems */
void restoreTerminalSettings();
#endif
} // namespace ConsoleUtils

/* Namespace containing functions related to number casting and bounds
 * validation */
namespace NumberCast {

/* Gets user input from the console with the provided prompt and returns it as
 * an Optional double */
Optional<f64> getInput(const str &prompt);

/* Checks if the given floating-point value is within valid bounds and throws an
 * error if not */
void checkBounds(f64 value);

/* Template function that returns the maximum of two values (works with any
 * comparable type) */
template <typename T> T getMax(T a, T b) { return (a > b) ? a : b; }

}; // namespace NumberCast

/* Class representing the application logic */
class Application {
  public:
    /* Runs the main logic of the application */
    void run();
};
