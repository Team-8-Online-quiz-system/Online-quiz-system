#include <chrono>
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

void testExactlyOneHundredInputLines() {
    const std::vector<oqs::Question> bank = oqs::loadQuestions();
    const std::vector<std::string> invalid = {
        "E", "5", "0", "", "AB", "; rm -rf /", "$(reboot)",
        "| shutdown", "%n%s%x", "' OR 1=1 --", std::string(257, 'x')
    };
    const std::string validAnswers = "BABCB";

    std::string inputText;
    std::size_t lineCount = 0;
    for (std::size_t question = 0; question < 5; ++question) {
        for (std::size_t attempt = 0; attempt < 19; ++attempt) {
            inputText += invalid[(question * 19 + attempt) % invalid.size()];
            inputText += '\n';
            ++lineCount;
        }
        inputText += validAnswers[question];
        inputText += '\n';
        ++lineCount;
    }

    check(lineCount == 100, "prepared input contains exactly 100 lines");

    std::istringstream input(inputText);
    std::ostringstream output;
    oqs::SessionState session = oqs::startSession("Reliability", bank.size());
    const bool completed = oqs::runQuiz(input, output, session, bank);

    check(completed, "quiz completes with 100 input lines");
    check(session.completed, "session is marked completed");
    check(session.acceptedResponses.size() == 5,
          "all five question responses are recorded");
}

void testAverageResponseTime() {
    std::vector<oqs::Question> bank;
    bank.reserve(20);
    for (int i = 0; i < 20; ++i) {
        oqs::Question question;
        question.text = "Performance question " + std::to_string(i + 1);
        question.options = {"Correct", "Wrong 1", "Wrong 2", "Wrong 3"};
        question.correctIndex = 0;
        bank.push_back(question);
    }

    std::string answers;
    for (std::size_t i = 0; i < bank.size(); ++i) {
        answers += "A\n";
    }

    std::istringstream input(answers);
    std::ostringstream output;
    oqs::SessionState session = oqs::startSession("Performance", bank.size());

    const auto start = std::chrono::steady_clock::now();
    const bool completed = oqs::runQuiz(input, output, session, bank);
    const auto finish = std::chrono::steady_clock::now();

    const double totalSeconds =
        std::chrono::duration<double>(finish - start).count();
    const double averageSeconds = totalSeconds / static_cast<double>(bank.size());

    std::cout << "[INFO] 20 answers: " << totalSeconds
              << " seconds total; " << averageSeconds
              << " seconds average per answer.\n";

    check(completed, "20-question performance run completes");
    check(averageSeconds < 1.0,
          "average response time is below one second per answer");
}
}  // namespace

int main() {
    testExactlyOneHundredInputLines();
    testAverageResponseTime();
    std::cout << (failures == 0 ? "\nAll tests passed.\n" : "\nSome tests failed.\n");
    return failures == 0 ? 0 : 1;
}
