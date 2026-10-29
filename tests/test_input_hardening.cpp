#include <iostream>
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

void testSafeInput() {
    check(oqs::isSafeInput("A"), "accepts ordinary input");
    check(oqs::isSafeInput(" b "), "accepts surrounding spaces");
    check(oqs::isSafeInput("A\tB"), "accepts tab");
    check(oqs::isSafeInput("A\r"), "accepts trailing carriage return");
    check(oqs::isSafeInput(std::string(256, 'x')), "accepts exactly 256 characters");
    check(!oqs::isSafeInput(std::string(257, 'x')), "rejects 257 characters");
    check(!oqs::isSafeInput(std::string(1024 * 1024, 'x')), "rejects 1 MB input");

    std::string escapeInput("A");
    escapeInput.push_back('\x1b');
    escapeInput += "[2J";
    check(!oqs::isSafeInput(escapeInput), "rejects ESC control character");

    check(!oqs::isSafeInput(std::string(1, '\0')), "rejects NUL");
    check(!oqs::isSafeInput(std::string(1, '\x07')), "rejects BEL");
    check(!oqs::isSafeInput(std::string(1, '\x7f')), "rejects DEL");
}

void testParseAnswerRejectsUnsafeInput() {
    check(oqs::parseAnswer("A", 4) == 0, "parses A");
    check(oqs::parseAnswer("4", 4) == 3, "parses 4");
    check(!oqs::parseAnswer("E", 4), "rejects E");
    check(!oqs::parseAnswer("5", 4), "rejects 5");
    check(!oqs::parseAnswer("0", 4), "rejects 0");
    check(!oqs::parseAnswer("AB", 4), "rejects multiple characters");
    check(!oqs::parseAnswer(std::string(257, 'x'), 4), "rejects overlong answer");
    check(!oqs::parseAnswer(std::string(1, '\0'), 4), "rejects NUL answer");

    std::string escapeInput("A");
    escapeInput.push_back('\x1b');
    check(!oqs::parseAnswer(escapeInput, 4), "rejects ESC answer");
}

void testPromptAnswerDoesNotEchoPayloads() {
    const std::vector<std::string> payloads = {
        "; rm -rf /", "$(reboot)", "| shutdown", "%n%s%x", "' OR 1=1 --"
    };
    std::string inputText;
    for (const std::string& payload : payloads) {
        inputText += payload + "\n";
    }
    inputText += "B\n";

    std::istringstream input(inputText);
    std::ostringstream output;
    const auto answer = oqs::promptAnswer(input, output, 4);
    const std::string displayed = output.str();

    check(answer == 1, "re-prompts and accepts B after command-like input");
    check(countOccurrences(displayed, "Invalid answer. Please enter A-D or 1-4.") ==
              payloads.size(),
          "prints one invalid-answer message per rejected payload");

    bool echoed = false;
    for (const std::string& payload : payloads) {
        if (displayed.find(payload) != std::string::npos) {
            echoed = true;
        }
    }
    check(!echoed, "does not echo submitted payloads");
}
}  // namespace

int main() {
    testSafeInput();
    testParseAnswerRejectsUnsafeInput();
    testPromptAnswerDoesNotEchoPayloads();
    std::cout << (failures == 0 ? "\nAll tests passed.\n" : "\nSome tests failed.\n");
    return failures == 0 ? 0 : 1;
}
