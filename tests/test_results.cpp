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

}  // namespace

int main() {
    testKnownPercentages();
    testResultSummary();
    testScoreBounds();
    std::cout << (failures == 0 ? "\nAll result tests passed.\n"
                                : "\nSome result tests failed.\n");
    return failures == 0 ? 0 : 1;
}
