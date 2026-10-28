#include "question_bank.hpp"
#include "quiz_engine.hpp"
#include "quiz_runner.hpp"
#include "scoring.hpp"
#include "session.hpp"

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

void testKnownPercentages() {
    check(oqs::formatPercentage(oqs::calculatePercentage(4, 5)) == "80.0%",
          "4 out of 5 is 80.0 percent");
    check(oqs::formatPercentage(oqs::calculatePercentage(5, 5)) == "100.0%",
          "5 out of 5 is 100.0 percent");
    check(oqs::formatPercentage(oqs::calculatePercentage(0, 5)) == "0.0%",
          "0 out of 5 is 0.0 percent");
    check(oqs::formatPercentage(oqs::calculatePercentage(1, 3)) == "33.3%",
          "1 out of 3 rounds to 33.3 percent");
    check(oqs::formatPercentage(oqs::calculatePercentage(2, 3)) == "66.7%",
          "2 out of 3 rounds to 66.7 percent");
    check(oqs::formatPercentage(oqs::calculatePercentage(0, 0)) == "0.0%",
          "0 out of 0 returns safely without division by zero");
}

void testResultSummary() {
    std::ostringstream out;
    oqs::showResultSummary(out, "Sarah", 4, 5);
    check(out.str() == "Player: Sarah\nScore: 4 / 5\nPercentage: 80.0%\n",
          "result summary prints the required three lines");

    out.str("");
    out.clear();
    bool negativeScoreThrew = false;
    try {
        oqs::showResultSummary(out, "Sarah", -1, 5);
    } catch (const std::out_of_range&) {
        negativeScoreThrew = true;
    }
    check(negativeScoreThrew && out.str().empty(),
          "negative score throws before printing any summary text");

    bool excessiveScoreThrew = false;
    try {
        oqs::showResultSummary(out, "Sarah", 6, 5);
    } catch (const std::out_of_range&) {
        excessiveScoreThrew = true;
    }
    check(excessiveScoreThrew && out.str().empty(),
          "score above total throws before printing any summary text");
}

void testScoreBounds() {
    check(oqs::isScoreInRange(0, 5), "zero is a valid score boundary");
    check(oqs::isScoreInRange(5, 5), "total questions is a valid score boundary");
    check(!oqs::isScoreInRange(-1, 5), "negative score is below the valid range");
    check(!oqs::isScoreInRange(6, 5), "score above total is outside the valid range");

    const std::vector<int> key = {1, 0, 2, 3, 0};
    const std::vector<int> answers = {1, 2, 2, 1, 0};
    int score = 0;
    for (std::size_t i = 0; i < answers.size(); ++i) {
        score = oqs::updateScore(score, oqs::evaluateAnswer(key[i], answers[i]));
        check(oqs::isScoreInRange(score, answers.size()),
              "score remains within bounds after answer " + std::to_string(i + 1));
    }
}

void testReplayReset() {
    const std::vector<int> key = oqs::answerKey(oqs::loadQuestions());
    oqs::SessionState session = oqs::startSession("Sarah", key.size());
    for (std::size_t i = 0; i < key.size(); ++i) {
        oqs::recordResponse(session.acceptedResponses, i, 0);
        session.score = oqs::updateScore(session.score, oqs::evaluateAnswer(key[i], 0));
        oqs::advanceQuestion(session);
    }
    check(session.score > 0 && session.completed,
          "first attempt records answers and reaches completion");

    oqs::resetSession(session);
    check(session.score == 0, "replay reset clears the score");
    check(session.currentQuestionIndex == 0, "replay reset returns to the first question");
    check(session.acceptedResponses == std::vector<int>(key.size(), -1),
          "replay reset marks every response unanswered");
    check(!session.completed, "replay reset opens a new attempt");

    const std::vector<int> replayAnswers = {1, 2, 1, 0, 1};
    for (std::size_t i = 0; i < replayAnswers.size(); ++i) {
        check(oqs::recordResponse(session.acceptedResponses, i, replayAnswers[i]),
              "replay accepts response " + std::to_string(i + 1));
        session.score = oqs::updateScore(
            session.score, oqs::evaluateAnswer(key[i], replayAnswers[i]));
        oqs::advanceQuestion(session);
    }
    const int replayScore = oqs::calculateFinalScore(session.acceptedResponses, key);
    check(session.completed && session.score == replayScore && replayScore == 3,
          "replay computes its own score of 3 out of 5");
    check(oqs::formatPercentage(oqs::calculatePercentage(replayScore, key.size())) == "60.0%",
          "replay percentage reflects only the second attempt");
}

void testRealQuestionBank() {
    const auto bank = oqs::loadQuestions();
    const auto key = oqs::answerKey(bank);
    const std::vector<int> allCorrect = {1, 0, 1, 2, 1};
    const std::vector<int> mostlyIncorrect = {0, 0, 0, 0, 0};

    const int perfectScore = oqs::calculateFinalScore(allCorrect, key);
    check(perfectScore == 5, "B,A,B,C,B scores 5 against the real question bank");
    check(oqs::formatPercentage(oqs::calculatePercentage(perfectScore, bank.size())) ==
              "100.0%",
          "B,A,B,C,B produces 100.0 percent");

    const int lowScore = oqs::calculateFinalScore(mostlyIncorrect, key);
    check(lowScore == 1, "A,A,A,A,A scores 1 against the real question bank");
    check(oqs::formatPercentage(oqs::calculatePercentage(lowScore, bank.size())) == "20.0%",
          "A,A,A,A,A produces 20.0 percent");
}

}  // namespace

int main() {
    testKnownPercentages();
    testResultSummary();
    testScoreBounds();
    testReplayReset();
    testRealQuestionBank();
    std::cout << (failures == 0 ? "\nAll result tests passed.\n"
                                : "\nSome result tests failed.\n");
    return failures == 0 ? 0 : 1;
}
