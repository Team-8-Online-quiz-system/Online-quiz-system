#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "question_bank.hpp"
#include "quiz_runner.hpp"
#include "scoring.hpp"
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

std::vector<oqs::Question> sampleBank() {
    return {
        {"First?", {"a", "b", "c", "d"}, 1},
        {"Second?", {"a", "b", "c", "d"}, 0},
        {"Third?", {"a", "b", "c", "d"}, 3},
    };
}

void testAllCorrect() {
    const auto bank = sampleBank();
    oqs::SessionState session = oqs::startSession("Sarah", bank.size());
    std::istringstream in("B\nA\nD\n");
    std::ostringstream out;

    check(oqs::runQuiz(in, out, session, bank), "all questions answered");
    check(session.score == 3, "score is 3 when all answers are correct");
    check(session.completed, "session is completed after the last question");
    check(oqs::calculateFinalScore(session.acceptedResponses, oqs::answerKey(bank)) == 3,
          "final score matches the running score");
    check(out.str().find("Question 1 of 3") != std::string::npos &&
              out.str().find("Question 3 of 3") != std::string::npos,
          "progress is shown for every question");
}

void testMixedAnswersAndInvalidInput() {
    const auto bank = sampleBank();
    oqs::SessionState session = oqs::startSession("Sarah", bank.size());
    std::istringstream in("z\n9\nb\n3\n\nd\n");
    std::ostringstream out;

    check(oqs::runQuiz(in, out, session, bank), "quiz completes after invalid input");
    check(session.score == 2, "score counts only correct answers");
    check(session.acceptedResponses == std::vector<int>({1, 2, 3}),
          "one response recorded per question");
    check(out.str().find("Incorrect. The correct answer was A.") != std::string::npos,
          "incorrect answer gets feedback");
    check(out.str().find("Invalid answer.") != std::string::npos,
          "invalid input is rejected with a message");
}

void testInputEndsEarly() {
    const auto bank = sampleBank();
    oqs::SessionState session = oqs::startSession("Sarah", bank.size());
    std::istringstream in("B\n");
    std::ostringstream out;

    check(!oqs::runQuiz(in, out, session, bank), "quiz stops when input ends");
    check(session.score == 1 && session.currentQuestionIndex == 1,
          "state is kept up to the last answered question");
}

void testBuiltInBank() {
    const auto bank = oqs::loadQuestions();
    oqs::SessionState session = oqs::startSession("Team8", bank.size());
    std::string answers;
    for (const int correct : oqs::answerKey(bank)) {
        answers += static_cast<char>('A' + correct);
        answers += '\n';
    }
    std::istringstream in(answers);
    std::ostringstream out;

    check(oqs::runQuiz(in, out, session, bank) &&
              session.score == static_cast<int>(bank.size()),
          "built-in bank can be played to a full score");
}

}  // namespace

int main() {
    testAllCorrect();
    testMixedAnswersAndInvalidInput();
    testInputEndsEarly();
    testBuiltInBank();

    std::cout << (failures == 0 ? "\nAll tests passed.\n" : "\nSome tests failed.\n");
    return failures == 0 ? 0 : 1;
}
