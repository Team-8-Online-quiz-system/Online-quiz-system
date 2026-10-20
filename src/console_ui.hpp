#ifndef OQS_CONSOLE_UI_HPP
#define OQS_CONSOLE_UI_HPP

#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string>

namespace oqs::ui {

enum class StartChoice { Start, Exit };

constexpr std::size_t kMaxNameLength = 30;

// Prints the application title and welcome message (OQS-F-001).
void showWelcome(std::ostream& out);

// Shows the Start Quiz / Exit menu until a valid choice is entered.
// Returns std::nullopt if input ends.
std::optional<StartChoice> promptStart(std::istream& in, std::ostream& out);

// 1-30 characters of letters, digits, spaces, underscores or hyphens.
bool isValidPlayerName(const std::string& name);

// Prompts until a valid player name is entered (OQS-F-002).
// Returns the trimmed name, or std::nullopt if input ends.
std::optional<std::string> promptPlayerName(std::istream& in, std::ostream& out);

}  // namespace oqs::ui

#endif
