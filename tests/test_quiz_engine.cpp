#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "quiz_engine.hpp"

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

std::size_t countOccurrences(const std::string& text, const std::string& needle) {
    std::size_t count = 0;
    std::size_t position = 0;
    while ((position = text.find(needle, position)) != std::string::npos) {
        ++count;
        position += needle.size();
    }
    return count;
}

void testOptionLabel() {
    check(oqs::optionLabel(0) == 'A', "optionLabel maps 0 to A");
    check(oqs::optionLabel(1) == 'B', "optionLabel maps 1 to B");
    check(oqs::optionLabel(3) == 'D', "optionLabel maps 3 to D");
}

void testDisplayQuestion() {
    std::ostringstream out;
    oqs::displayQuestion(out, "Which language is this?", {"C++", "Java", "Python", "Rust"});
    const std::string displayed = out.str();

    check(displayed.find("Which language is this?") != std::string::npos,
          "displayQuestion prints the question text");
    check(displayed.find("A) C++") != std::string::npos,
          "displayQuestion labels option A");
    check(displayed.find("B) Java") != std::string::npos,
          "displayQuestion labels option B");
    check(displayed.find("C) Python") != std::string::npos,
          "displayQuestion labels option C");
    check(displayed.find("D) Rust") != std::string::npos,
          "displayQuestion labels option D");
}

void testParseAnswer() {
    check(oqs::parseAnswer("A", 4) == std::optional<int>(0),
          "parseAnswer accepts uppercase A");
    check(oqs::parseAnswer("d", 4) == std::optional<int>(3),
          "parseAnswer accepts lowercase d");
    check(oqs::parseAnswer(" 2 ", 4) == std::optional<int>(1),
          "parseAnswer trims whitespace around digits");
    check(oqs::parseAnswer("4", 4) == std::optional<int>(3),
          "parseAnswer accepts digit 4");

    check(!oqs::parseAnswer("E", 4).has_value(), "parseAnswer rejects E");
    check(!oqs::parseAnswer("5", 4).has_value(), "parseAnswer rejects 5");
    check(!oqs::parseAnswer("0", 4).has_value(), "parseAnswer rejects 0");
    check(!oqs::parseAnswer("", 4).has_value(), "parseAnswer rejects empty input");
    check(!oqs::parseAnswer("AB", 4).has_value(), "parseAnswer rejects multiple characters");
    check(!oqs::parseAnswer("z", 4).has_value(), "parseAnswer rejects z");
    check(!oqs::parseAnswer("-1", 4).has_value(), "parseAnswer rejects -1");
    check(!oqs::parseAnswer("C", 2).has_value(), "parseAnswer rejects letters beyond optionCount");
    check(!oqs::parseAnswer("3", 2).has_value(), "parseAnswer rejects digits beyond optionCount");
}

void testPromptAnswer() {
    std::istringstream in("x\n9\n\nb\n");
    std::ostringstream out;
    const std::optional<int> answer = oqs::promptAnswer(in, out, 4);
    const std::string displayed = out.str();

    check(answer == std::optional<int>(1), "promptAnswer retries invalid input and accepts b");
    check(countOccurrences(displayed, "Invalid answer. Please enter A-D or 1-4.") == 3,
          "promptAnswer displays exactly three invalid-answer messages");
    check(countOccurrences(displayed, "Your answer (A-D or 1-4): ") == 4,
          "promptAnswer displays the prompt for every attempt");

    std::istringstream empty;
    std::ostringstream emptyOutput;
    check(!oqs::promptAnswer(empty, emptyOutput, 4).has_value(),
          "promptAnswer returns nullopt when input ends");

    std::istringstream twoOptions("C\n2\n");
    std::ostringstream twoOptionsOutput;
    check(oqs::promptAnswer(twoOptions, twoOptionsOutput, 2) == std::optional<int>(1),
          "promptAnswer uses labels appropriate to optionCount");
    check(twoOptionsOutput.str().find("Your answer (A-B or 1-2): ") != std::string::npos,
          "promptAnswer formats choices based on optionCount");

    std::istringstream noOptions("A\n");
    std::ostringstream noOptionsOutput;
    check(!oqs::promptAnswer(noOptions, noOptionsOutput, 0).has_value(),
          "promptAnswer handles zero options without looping");
}

void testRecordResponse() {
    std::vector<int> responses(2, -1);
    check(oqs::recordResponse(responses, 0, 2), "recordResponse stores the first answer");
    check(responses[0] == 2, "recordResponse stores the selected option");

    check(!oqs::recordResponse(responses, 0, 1), "recordResponse refuses a second answer");
    check(responses[0] == 2, "recordResponse leaves an existing answer unchanged");
    check(!oqs::recordResponse(responses, 2, 0), "recordResponse rejects out-of-range question index");
    check(!oqs::recordResponse(responses, 1, -1), "recordResponse rejects a negative option");
    check(responses[1] == -1, "recordResponse leaves unanswered entry unchanged after invalid option");
}

}  // namespace

int main() {
    testOptionLabel();
    testDisplayQuestion();
    testParseAnswer();
    testPromptAnswer();
    testRecordResponse();

    std::cout << (failures == 0 ? "\nAll tests passed.\n" : "\nSome tests failed.\n");
    return failures == 0 ? 0 : 1;
}
