#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "question_bank.hpp"
#include "quiz_runner.hpp"
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

void testMixedInvalidInputs() {
    const std::vector<oqs::Question> bank = oqs::loadQuestions();
    const std::vector<std::string> invalid = {
        "E", "5", "0", "", "AB", "; rm -rf /", "$(reboot)",
        "| shutdown", "%n%s%x", "' OR 1=1 --", std::string(257, 'x')
    };
    const std::string validAnswers = "BABCB";

    std::string inputText;
    for (std::size_t question = 0; question < bank.size(); ++question) {
        for (const std::string& value : invalid) {
            inputText += value + "\n";
        }
        inputText += validAnswers[question];
        inputText += '\n';
    }

    std::istringstream input(inputText);
    std::ostringstream output;
    oqs::SessionState session = oqs::startSession("Robustness", bank.size());
    const bool completed = oqs::runQuiz(input, output, session, bank);

    check(completed, "quiz completes after mixed invalid inputs");
    check(session.acceptedResponses.size() == bank.size(),
          "one response slot exists per question");

    bool answersRecorded = session.acceptedResponses.size() == bank.size();
    for (std::size_t i = 0; answersRecorded && i < bank.size(); ++i) {
        answersRecorded = session.acceptedResponses[i] >= 0 &&
                          session.acceptedResponses[i] < 4;
    }
    check(answersRecorded, "invalid inputs do not prevent valid answers being recorded");
    check(session.score >= 0 &&
              session.score <= static_cast<int>(bank.size()),
          "score remains within valid bounds");
}
}  // namespace

int main() {
    testMixedInvalidInputs();
    std::cout << (failures == 0 ? "\nAll tests passed.\n" : "\nSome tests failed.\n");
    return failures == 0 ? 0 : 1;
}
