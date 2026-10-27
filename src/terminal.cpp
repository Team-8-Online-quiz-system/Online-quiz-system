#include "terminal.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

// Platform headers: ISO C++ has no portable way to detect a terminal or to
// switch the Windows console into ANSI mode (SRS 2.5).
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#include <unistd.h>
#endif

namespace oqs::terminal {

namespace {

bool stdoutIsTerminal() {
#ifdef _WIN32
    return _isatty(_fileno(stdout)) != 0;
#else
    return isatty(STDOUT_FILENO) != 0;
#endif
}

bool environmentAllowsColour() {
    if (std::getenv("NO_COLOR") != nullptr) {
        return false;
    }
    const char* term = std::getenv("TERM");
    return term == nullptr || std::strcmp(term, "dumb") != 0;
}

#ifdef _WIN32
bool enableWindowsAnsi() {
    SetConsoleOutputCP(CP_UTF8);
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (console == INVALID_HANDLE_VALUE || !GetConsoleMode(console, &mode)) {
        return false;
    }
    return SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
}
#endif

}  // namespace

bool enableStyledOutput() {
    if (!stdoutIsTerminal() || !environmentAllowsColour()) {
        return false;
    }
#ifdef _WIN32
    return enableWindowsAnsi();
#else
    return true;
#endif
}

}  // namespace oqs::terminal
