#include "scoring.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

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

void testEvaluateAnswer() {
    check(oqs::evaluateAnswer(1, 1), "matching non-negative option is correct");
    check(!oqs::evaluateAnswer(1, 2), "different option is incorrect");
    check(!oqs::evaluateAnswer(-1, -1), "negative answer-key index is invalid");
    check(!oqs::evaluateAnswer(1, -1), "negative selected option is invalid");
}

void testUpdateScore() {
    int score = oqs::updateScore(0, true);
    check(score == 1, "correct answer adds one point");
    score = oqs::updateScore(score, false);
    check(score == 1, "incorrect answer leaves the score unchanged");
}

void testFeedback() {
    std::ostringstream out;
    oqs::showFeedback(out, true, 1);
    check(out.str() == "Correct!\n", "correct feedback matches the required text");

    out.str("");
    out.clear();
    oqs::showFeedback(out, false, 1);
    check(out.str() == "Incorrect. The correct answer was B.\n",
          "incorrect feedback includes the correct option letter");
}

void testAllQuestionsAnswered() {
    check(oqs::allQuestionsAnswered({0, 1, 2}), "all recorded responses are complete");
    check(!oqs::allQuestionsAnswered({0, -1, 2}), "an unanswered response is incomplete");
    check(!oqs::allQuestionsAnswered({}), "an empty quiz has no answered questions");
}

void testCalculateFinalScore() {
    check(oqs::calculateFinalScore({0, 2, 1, 0, 2}, {0, 2, 1, 0, 1}) == 4,
          "four correct answers out of five score four points");
    check(oqs::calculateFinalScore({0, 0, 0}, {1, 1, 1}) == 0,
          "no correct answers score zero points");

    bool threw = false;
    try {
        static_cast<void>(oqs::calculateFinalScore({0}, {0, 1}));
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    check(threw, "different response and answer-key sizes are rejected");
}

void testShowFinalScore() {
    std::ostringstream out;
    oqs::showFinalScore(out, "Jordan", 4, 5);
    check(out.str() == "Final score for Jordan: 4 / 5\n",
          "final score display matches the required text");
}

}  // namespace

int main() {
    testEvaluateAnswer();
    testUpdateScore();
    testFeedback();
    testAllQuestionsAnswered();
    testCalculateFinalScore();
    testShowFinalScore();

    std::cout << (failures == 0 ? "\nAll tests passed.\n" : "\nSome tests failed.\n");
    return failures == 0 ? 0 : 1;
}
