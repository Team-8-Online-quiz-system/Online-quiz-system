#include <iostream>
#include <sstream>
#include <string>

#include "console_ui.hpp"
#include "session.hpp"

namespace {

int failures = 0;

void check(bool condition, const std::string& description) {
    if (condition) {
        std::cout << "[PASS] " << description << '\n';
    } else {
        std::cout << "[FAIL] " << description << '\n';
        ++failures;
    }
}

void testStartSession() {
    const oqs::SessionState s = oqs::startSession("Sarah", 5);
    check(s.playerName == "Sarah", "startSession stores the player name");
    check(s.score == 0, "startSession sets score to 0");
    check(s.currentQuestionIndex == 0, "startSession sets question index to 0");
    check(s.acceptedResponses.size() == 5, "startSession has one response slot per question");
    check(!s.completed, "startSession is not completed");
}

void testAdvanceAndReset() {
    oqs::SessionState s = oqs::startSession("Sarah", 2);
    oqs::advanceQuestion(s);
    check(s.currentQuestionIndex == 1 && !s.completed, "advanceQuestion moves to question 2");
    oqs::advanceQuestion(s);
    check(s.completed, "advanceQuestion completes after the last question");
    oqs::advanceQuestion(s);
    check(s.currentQuestionIndex == 2, "advanceQuestion does not go past the end");

    s.score = 2;
    s.acceptedResponses[0] = 1;
    oqs::resetSession(s);
    check(s.score == 0 && s.currentQuestionIndex == 0 && !s.completed,
          "resetSession clears score and progress");
    check(s.acceptedResponses[0] == oqs::kNoResponse, "resetSession clears answers");
}

void testPlayerName() {
    check(oqs::ui::isValidPlayerName("Sarah"), "simple name is valid");
    check(oqs::ui::isValidPlayerName("K Madhu_2-x"), "spaces, digits, _ and - are valid");
    check(oqs::ui::isValidPlayerName(std::string(30, 'a')), "30-character name is valid");
    check(!oqs::ui::isValidPlayerName(std::string(31, 'a')), "31-character name is rejected");
    check(!oqs::ui::isValidPlayerName(""), "empty name is rejected");
    check(!oqs::ui::isValidPlayerName("rm -rf /;"), "special characters are rejected");

    std::istringstream in("\n   \nbad!name\n  Thanvitha  \n");
    std::ostringstream out;
    const auto name = oqs::ui::promptPlayerName(in, out);
    check(name && *name == "Thanvitha", "promptPlayerName re-prompts until a valid name");

    std::istringstream empty("");
    check(!oqs::ui::promptPlayerName(empty, out), "promptPlayerName handles end of input");
}

void testStartMenu() {
    std::ostringstream out;
    std::istringstream start("x\n9\n1\n");
    check(oqs::ui::promptStart(start, out) == oqs::ui::StartChoice::Start,
          "start menu re-prompts and accepts 1");
    std::istringstream quit("2\n");
    check(oqs::ui::promptStart(quit, out) == oqs::ui::StartChoice::Exit, "start menu accepts 2");
}

void testProgress() {
    check(oqs::ui::formatProgress(0, 10) == "Question 1 of 10", "progress starts at question 1");
    check(oqs::ui::formatProgress(9, 10) == "Question 10 of 10", "progress shows last question");
}

}  // namespace

int main() {
    testStartSession();
    testAdvanceAndReset();
    testPlayerName();
    testStartMenu();
    testProgress();

    std::cout << (failures == 0 ? "\nAll tests passed.\n" : "\nSome tests failed.\n");
    return failures == 0 ? 0 : 1;
}
