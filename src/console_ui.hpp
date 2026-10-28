#ifndef OQS_CONSOLE_UI_HPP
#define OQS_CONSOLE_UI_HPP

#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string>
#include <vector>

#include "session.hpp"

namespace oqs::ui {

enum class StartChoice { Start, Exit };
enum class ReplayChoice { Replay, Exit };

constexpr std::size_t kMaxNameLength = 30;

// Styled output (the TUI): colours, boxes, a progress bar and screen clearing.
// Off by default; main turns it on for interactive terminals. Plain output is
// unchanged text, so tests, CI and piped input see the same wording.
void setStyledOutput(bool enabled);
bool styledOutput();

enum class Tone { Title, Accent, Success, Error, Highlight, Muted };

// Wraps text in the colour for `tone` when styled output is on.
std::string paint(const std::string& text, Tone tone);

// Clears the screen when styled output is on; does nothing otherwise.
void clearScreen(std::ostream& out);

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

// "Question 3 of 10", with the question number starting at 1 (OQS-F-014).
std::string formatProgress(std::size_t questionIndex, std::size_t totalQuestions);

// Plain: "Question N of M". Styled: a header with the player, a progress bar
// and the running score.
void showProgress(std::ostream& out, const SessionState& session);

// Prints a question rendered by displayQuestion (text, then "  A) ..." lines);
// styled output highlights the text and the option labels.
void showQuestion(std::ostream& out, const std::string& rendered);

// Shows the answer prompt marker before the quiz engine's prompt (styled only).
void showAnswerCue(std::ostream& out);

// Prints a feedback line from showFeedback, in green or red when styled.
void showFeedbackLine(std::ostream& out, const std::string& feedback, bool correct);

// Styled: waits for Enter so the player can read the screen; returns false if
// input ends. Plain: returns true without reading anything.
bool waitForEnter(std::istream& in, std::ostream& out, const std::string& action);

// Prints result lines; styled output draws them in a titled box.
void showResultPanel(std::ostream& out, const std::vector<std::string>& lines);

// Shows the Play again / Exit menu after a result (OQS-F-019, OQS-F-020).
// Returns std::nullopt if input ends.
std::optional<ReplayChoice> promptReplay(std::istream& in, std::ostream& out);

// Prints the closing message; the name is left out when it is empty.
void showGoodbye(std::ostream& out, const std::string& playerName);

}  // namespace oqs::ui

#endif
