#include <iostream>
#include <sstream>
#include <string>

#include "app.hpp"

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

struct Run {
    int exitCode;
    std::string output;
};

Run run(const std::string& input) {
    std::istringstream in(input);
    std::ostringstream out;
    std::ostringstream err;
    const int code = oqs::runApp(in, out, err);
    return {code, out.str()};
}

bool contains(const std::string& text, const std::string& part) {
    return text.find(part) != std::string::npos;
}

std::size_t count(const std::string& text, const std::string& part) {
    std::size_t n = 0;
    for (auto pos = text.find(part); pos != std::string::npos; pos = text.find(part, pos + 1)) {
        ++n;
    }
    return n;
}

// Built-in bank answers: B A B C B.
const std::string kAllCorrect = "B\nA\nB\nC\nB\n";
const std::string kAllA = "A\nA\nA\nA\nA\n";

void testReplayStartsFreshAttempt() {
    const Run r = run("1\nSarah\n" + kAllCorrect + "1\n" + kAllA + "2\n");
    check(r.exitCode == 0, "replay then exit returns 0");
    check(contains(r.output, "Final score for Sarah: 5 / 5"), "first attempt scores 5 / 5");
    check(contains(r.output, "Starting a new attempt. Your score is back to 0."),
          "replay announces the reset");
    check(contains(r.output, "Final score for Sarah: 1 / 5"),
          "second attempt is scored on its own answers");
    check(count(r.output, "Question 1 of 5") == 2, "replay asks every question again");
    check(contains(r.output, "Thanks for playing, Sarah! Goodbye."), "exit says goodbye");
}

void testExitPaths() {
    const Run atStart = run("2\n");
    check(atStart.exitCode == 0 && contains(atStart.output, "Goodbye!"),
          "exit from the start menu ends cleanly");

    const Run afterResult = run("1\nSarah\n" + kAllCorrect + "2\n");
    check(afterResult.exitCode == 0 && count(afterResult.output, "Final score") == 1,
          "exit after the result does not replay");
}

void testInputEndsAnywhere() {
    const char* inputs[] = {"", "1\n", "1\nSarah\n", "1\nSarah\nB\nA\n",
                            "1\nSarah\nB\nA\nB\nC\nB\n"};
    for (const char* input : inputs) {
        const Run r = run(input);
        check(r.exitCode == 0 && contains(r.output, "Goodbye"),
              "input ending after \"" + std::string(input).substr(0, 12) + "...\" exits cleanly");
    }
}

}  // namespace

int main() {
    testReplayStartsFreshAttempt();
    testExitPaths();
    testInputEndsAnywhere();

    std::cout << (failures == 0 ? "\nAll tests passed.\n" : "\nSome tests failed.\n");
    return failures == 0 ? 0 : 1;
}
