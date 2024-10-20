#include "types.hpp"

#if defined(__linux__) || defined(__APPLE__)
#include <termios.h>
using Termios = struct termios;
#endif

namespace ConsoleUtils {
static void clearScreen();

void waitForKeyPress();

static void clearInputBuffer();

static void printError(const str &message);

#if defined(_WIN32)
inline HANDLE hConsole;
inline DWORD mode;

void disableCanonicalMode();

void restoreTerminalSettings();

#else

inline Termios oldt, newt;

void disableCanonicalMode();

void restoreTerminalSettings();
#endif
} // namespace ConsoleUtils

namespace NumberCast {
Optional<f64> getInput(const str &prompt);

void checkBounds(f64 value);

template <typename T> T getMax(T a, T b) { return (a > b) ? a : b; }
}; // namespace NumberCast

class Application {
  public:
    void run();
};
