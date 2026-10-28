#ifndef OQS_TERMINAL_HPP
#define OQS_TERMINAL_HPP

namespace oqs::terminal {

// Prepares the console for ANSI colours and UTF-8 box drawing, and reports
// whether styled output should be used: standard output is an interactive
// terminal, NO_COLOR is not set and TERM is not "dumb". Piped output (tests,
// CI) always gets plain text.
bool enableStyledOutput();

}  // namespace oqs::terminal

#endif
