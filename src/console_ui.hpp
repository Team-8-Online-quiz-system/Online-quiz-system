#ifndef OQS_CONSOLE_UI_HPP
#define OQS_CONSOLE_UI_HPP

#include <iosfwd>
#include <optional>

namespace oqs::ui {

enum class StartChoice { Start, Exit };

// Prints the application title and welcome message (OQS-F-001).
void showWelcome(std::ostream& out);

// Shows the Start Quiz / Exit menu until a valid choice is entered.
// Returns std::nullopt if input ends.
std::optional<StartChoice> promptStart(std::istream& in, std::ostream& out);

}  // namespace oqs::ui

#endif
