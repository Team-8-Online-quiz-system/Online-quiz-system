#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

bool contains(const std::string& text, const std::string& part) {
    return text.find(part) != std::string::npos;
}

std::string repeatBlock(std::size_t count) {
    std::string blocks;
    for (std::size_t i = 0; i < count; ++i) {
        blocks += "█";
    }
    return blocks;
}

const std::string kRendered ="What is 2 + 2?\n  A) 3\n  B) 4\n  C) 5\n  D) 22\n";

void testPlainOutputIsUnchanged() {
    oqs::ui::setStyledOutput(false);
    check(oqs::ui::paint("hello", oqs::ui::Tone::Accent) == "hello", "plain paint adds no codes");

    std::ostringstream question;
    oqs::ui::showQuestion(question, kRendered);
    check(question.str() == kRendered, "plain question is printed exactly as rendered");

    std::ostringstream progress;
    oqs::ui::showProgress(progress, oqs::startSession("Sarah", 5));
    check(progress.str() == "Question 1 of 5\n", "plain progress is the bare progress line");

    std::ostringstream feedback;
    oqs::ui::showFeedbackLine(feedback, "Correct!\n", true);
    check(feedback.str() == "Correct!\n", "plain feedback is printed as given");

    std::istringstream in("unread\n");
    std::ostringstream out;
    check(oqs::ui::waitForEnter(in, out, "continue") && out.str().empty(),
          "plain mode never waits for Enter");
    std::string rest;
    std::getline(in, rest);
    check(rest == "unread", "plain wait leaves input untouched");

    std::ostringstream panel;
    oqs::ui::showResultPanel(panel, {"Final score for Sarah: 4 / 5"});
    check(panel.str() == "Final score for Sarah: 4 / 5\n", "plain result panel prints its lines");

    std::ostringstream withPercentage;
    oqs::ui::showResultPanel(withPercentage, {"Final score for Sarah: 4 / 5"}, 80.0);
    check(withPercentage.str() == "Final score for Sarah: 4 / 5\n",
          "plain result panel adds no bar for the percentage");
}

void testStyledOutput() {
    oqs::ui::setStyledOutput(true);

    check(oqs::ui::paint("hi", oqs::ui::Tone::Success) == "\x1b[1;32mhi\x1b[0m",
          "styled paint wraps text in a colour and resets it");

    std::ostringstream clear;
    oqs::ui::clearScreen(clear);
    check(clear.str() == "\x1b[2J\x1b[H", "styled clear screen sends the ANSI clear sequence");

    oqs::SessionState session = oqs::startSession("Sarah", 4);
    session.currentQuestionIndex = 2;
    session.score = 1;
    std::ostringstream progress;
    oqs::ui::showProgress(progress, session);
    check(contains(progress.str(), "Question 3 of 4"), "styled progress keeps the progress text");
    check(contains(progress.str(), "Player: Sarah") && contains(progress.str(), "Score: 1"),
          "styled header shows the player and the running score");
    check(contains(progress.str(), "████████████") && contains(progress.str(), "░"),
          "styled progress draws a half-filled bar");

    std::ostringstream question;
    oqs::ui::showQuestion(question, kRendered);
    check(contains(question.str(), "[B]") && contains(question.str(), "4") &&
              !contains(question.str(), "B)"),
          "styled question relabels options as [A]..[D]");

    std::ostringstream wrong;
    oqs::ui::showFeedbackLine(wrong, "Incorrect. The correct answer was B.\n", false);
    check(contains(wrong.str(), "\x1b[1;31m") && contains(wrong.str(), "✘ Incorrect."),
          "styled incorrect feedback is red with a cross");

    std::istringstream in("\n");
    std::ostringstream out;
    check(oqs::ui::waitForEnter(in, out, "continue") && contains(out.str(), "Press Enter to continue"),
          "styled mode waits for Enter");
    std::istringstream ended("");
    check(!oqs::ui::waitForEnter(ended, out, "continue"), "styled wait reports end of input");

    std::ostringstream panel;
    oqs::ui::showResultPanel(panel, {"Final score for Sarah: 4 / 5"});
    check(contains(panel.str(), "YOUR RESULT") && contains(panel.str(), "╭") &&
              contains(panel.str(), "Final score for Sarah: 4 / 5"),
          "styled result panel draws a titled box");

    std::ostringstream high;
    oqs::ui::showResultPanel(high, {"Score: 4 / 5"}, 80.0);
    check(contains(high.str(), repeatBlock(24)) && contains(high.str(), "80.0%") &&
              contains(high.str(), "Excellent work!"),
          "styled result shows an 80% bar with a rating");

    std::ostringstream low;
    oqs::ui::showResultPanel(low, {"Score: 1 / 5"}, 20.0);
    check(contains(low.str(), "20.0%") && contains(low.str(), "Keep practising!") &&
              !contains(low.str(), repeatBlock(7)),
          "styled result shows a short bar and encouragement at 20%");

    oqs::ui::setStyledOutput(false);
}

}  // namespace

int main() {
    testPlainOutputIsUnchanged();
    testStyledOutput();

    std::cout << (failures == 0 ? "\nAll tests passed.\n" : "\nSome tests failed.\n");
    return failures == 0 ? 0 : 1;
}
