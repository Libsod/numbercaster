#include "types.hpp"

#if defined(__linux__) || defined(__APPLE__)
#include <termios.h>
using Termios = struct termios;
#endif

class ConsoleUtils {
  public:
    static void clearScreen();

    void waitForKeyPress();

    static void clearInputBuffer();

    static void printError(const str &message);

  private:
#if defined(_WIN32)
    HANDLE hConsole;
    DWORD mode;

    void disableCanonicalMode();

    void restoreTerminalSettings();
#else
    Termios oldt, newt;

    void disableCanonicalMode();

    void restoreTerminalSettings();
#endif
};

extern ConsoleUtils consoleUtils;

namespace NumberCast {
    Optional<f64> getInput(const str &prompt) ;

    void checkBounds(f64 value) ;

    template <typename T> T getMax(T a, T b)  { return (a > b) ? a : b; }
};

class Application {
  public:
    void run();
};
