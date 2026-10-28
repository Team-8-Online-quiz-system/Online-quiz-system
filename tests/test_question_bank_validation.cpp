#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "question_bank.hpp"

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

oqs::Question sampleQuestion() {
    return {"Which language is standardized as C++17?",
            {"C++", "Java", "Python", "Ruby"},
            0};
}

void testQuestionError() {
    // Valid question
    check(oqs::questionError(sampleQuestion()).empty(), "valid question returns empty error string");

    // Empty and blank question text
    oqs::Question q = sampleQuestion();
    q.text.clear();
    check(oqs::questionError(q) == "question text is empty", "empty question text returns exact reason");

    q = sampleQuestion();
    q.text = "   \t\n ";
    check(oqs::questionError(q) == "question text is empty", "blank question text returns exact reason");

    // Question text over 512 characters
    q = sampleQuestion();
    q.text = std::string(513, 'a');
    check(oqs::questionError(q) == "question text is longer than 512 characters",
          "513-char question text returns exact reason");

    // 100 KB question text (TC-SEC-03)
    q = sampleQuestion();
    q.text = std::string(100 * 1024, 'x');
    check(oqs::questionError(q) == "question text is longer than 512 characters",
          "100 KB question text rejected safely without crash (TC-SEC-03)");

    // Option counts: expected 4
    q = sampleQuestion();
    q.options.pop_back();
    check(oqs::questionError(q) == "expected 4 options, found 3",
          "3 options returns expected 4 options, found 3");

    q = sampleQuestion();
    q.options.push_back("Go");
    check(oqs::questionError(q) == "expected 4 options, found 5",
          "5 options returns expected 4 options, found 5");

    // Option empty or blank
    q = sampleQuestion();
    q.options[1].clear();
    check(oqs::questionError(q) == "option 2 is empty", "empty option 2 returns exact reason");

    q = sampleQuestion();
    q.options[2] = "   ";
    check(oqs::questionError(q) == "option 3 is empty", "blank option 3 returns exact reason");

    // Option over 256 characters
    q = sampleQuestion();
    q.options[0] = std::string(257, 'c');
    check(oqs::questionError(q) == "option 1 is longer than 256 characters",
          "257-char option 1 returns exact reason");

    // Option duplicates
    q = sampleQuestion();
    q.options[1] = q.options[0];
    check(oqs::questionError(q) == "option 2 duplicates option 1",
          "duplicate option 2 duplicates option 1");

    q = sampleQuestion();
    q.options[3] = q.options[2];
    check(oqs::questionError(q) == "option 4 duplicates option 3",
          "duplicate option 4 duplicates option 3");

    // Correct answer references
    q = sampleQuestion();
    q.correctIndex = -1;
    check(oqs::questionError(q) == "correct answer is not one of the options",
          "correctIndex -1 returns correct answer is not one of the options");

    q = sampleQuestion();
    q.correctIndex = 4;
    check(oqs::questionError(q) == "correct answer is not one of the options",
          "correctIndex 4 returns correct answer is not one of the options");

    q = sampleQuestion();
    q.correctIndex = 100;
    check(oqs::questionError(q) == "correct answer is not one of the options",
          "correctIndex 100 returns correct answer is not one of the options");
}

void testFilterValidQuestions() {
    oqs::Question valid1 = {"Question One?", {"A", "B", "C", "D"}, 0};
    oqs::Question invalid1 = {"", {"A", "B", "C", "D"}, 0};
    oqs::Question valid2 = {"Question Two?", {"Alpha", "Beta", "Gamma", "Delta"}, 1};
    oqs::Question invalid2 = {"Question Three?", {"X", "Y", "Z"}, 0};

    // Mixed list
    std::vector<oqs::Question> mixed = {valid1, invalid1, valid2, invalid2};
    std::ostringstream errStream;
    std::vector<oqs::Question> filtered = oqs::filterValidQuestions(mixed, errStream);

    check(filtered.size() == 2, "filterValidQuestions retains exactly 2 valid questions from mixed list");
    check(filtered[0].text == "Question One?", "filterValidQuestions preserves valid question 1");
    check(filtered[1].text == "Question Two?", "filterValidQuestions preserves valid question 2");

    std::string expectedErr = "Rejected question 1: question text is empty\n"
                              "Rejected question 3: expected 4 options, found 3\n";
    check(errStream.str() == expectedErr, "filterValidQuestions outputs expected error lines");

    // All valid list writes nothing
    std::ostringstream validErr;
    std::vector<oqs::Question> allValid = {valid1, valid2};
    std::vector<oqs::Question> kept = oqs::filterValidQuestions(allValid, validErr);
    check(kept.size() == 2 && validErr.str().empty(), "filterValidQuestions on all-valid list writes nothing");

    // All invalid list returns empty vector
    std::ostringstream invalidErr;
    std::vector<oqs::Question> allInvalid = {invalid1, invalid2};
    std::vector<oqs::Question> emptyKept = oqs::filterValidQuestions(allInvalid, invalidErr);
    check(emptyKept.empty(), "filterValidQuestions on all-invalid list returns empty vector");
    check(!invalidErr.str().empty(), "filterValidQuestions on all-invalid list reports rejections");
}

void testBoundsChecking() {
    std::vector<oqs::Question> bank = {sampleQuestion(), sampleQuestion()};

    // isValidQuestionIndex
    check(oqs::isValidQuestionIndex(bank, 0), "isValidQuestionIndex returns true for index 0");
    check(oqs::isValidQuestionIndex(bank, 1), "isValidQuestionIndex returns true for index size-1");
    check(!oqs::isValidQuestionIndex(bank, 2), "isValidQuestionIndex returns false for index size");
    check(!oqs::isValidQuestionIndex(bank, 9999999), "isValidQuestionIndex returns false for huge index");

    std::vector<oqs::Question> emptyBank;
    check(!oqs::isValidQuestionIndex(emptyBank, 0), "isValidQuestionIndex returns false for empty bank");

    // isValidOptionIndex
    oqs::Question q = sampleQuestion();
    check(oqs::isValidOptionIndex(q, 0), "isValidOptionIndex returns true for option 0");
    check(oqs::isValidOptionIndex(q, 3), "isValidOptionIndex returns true for option 3");
    check(!oqs::isValidOptionIndex(q, -1), "isValidOptionIndex returns false for negative option");
    check(!oqs::isValidOptionIndex(q, 4), "isValidOptionIndex returns false for option 4");
}

void testGetQuestion() {
    std::vector<oqs::Question> bank = {sampleQuestion()};
    check(oqs::getQuestion(bank, 0).text == sampleQuestion().text,
          "getQuestion returns expected question at index 0");

    bool threw = false;
    try {
        static_cast<void>(oqs::getQuestion(bank, 1));
    } catch (const std::out_of_range&) {
        threw = true;
    }
    check(threw, "getQuestion throws std::out_of_range when index >= bank.size()");
}

void testLoadQuestions() {
    std::vector<oqs::Question> bank = oqs::loadQuestions();
    check(bank.size() == 5, "loadQuestions returns 5 built-in questions");

    bool allValid = true;
    for (const auto& q : bank) {
        if (!oqs::validateQuestion(q)) {
            allValid = false;
        }
    }
    check(allValid, "all questions returned by loadQuestions are valid");
}

void testStartupPerformance() {
    // TC-NF-PERF-01 / OQS-NF-001: Build 1,000 valid generated questions
    std::vector<oqs::Question> largeBank;
    largeBank.reserve(1000);
    for (int i = 0; i < 1000; ++i) {
        largeBank.push_back({
            "Generated question #" + std::to_string(i) + " for performance testing?",
            {"Choice Alpha " + std::to_string(i),
             "Choice Beta " + std::to_string(i),
             "Choice Gamma " + std::to_string(i),
             "Choice Delta " + std::to_string(i)},
            i % 4
        });
    }

    std::ostringstream discard;
    const auto startFilter = std::chrono::steady_clock::now();
    const std::vector<oqs::Question> filtered = oqs::filterValidQuestions(largeBank, discard);
    const auto endFilter = std::chrono::steady_clock::now();
    const auto filterDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(endFilter - startFilter).count();

    std::cout << "[INFO] filterValidQuestions(1,000 items) elapsed: " << filterDurationMs << " ms\n";
    check(filtered.size() == 1000, "1,000 valid questions are all accepted");
    check(filterDurationMs < 2000, "1,000 question validation startup time is under 2 seconds (TC-NF-PERF-01)");

    // Time loadQuestions() 10 times; each must be under 2 seconds
    bool allLoadsFast = true;
    long long maxLoadMs = 0;
    for (int run = 0; run < 10; ++run) {
        const auto startLoad = std::chrono::steady_clock::now();
        const std::vector<oqs::Question> loaded = oqs::loadQuestions();
        const auto endLoad = std::chrono::steady_clock::now();
        const auto loadDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(endLoad - startLoad).count();
        if (loadDurationMs > maxLoadMs) {
            maxLoadMs = loadDurationMs;
        }
        if (loadDurationMs >= 2000 || loaded.size() != 5) {
            allLoadsFast = false;
        }
    }

    std::cout << "[INFO] loadQuestions() 10 runs max elapsed: " << maxLoadMs << " ms\n";
    check(allLoadsFast, "loadQuestions() 10 iterations each complete in under 2 seconds (OQS-NF-001)");
}

}  // namespace

int main() {
    testQuestionError();
    testFilterValidQuestions();
    testBoundsChecking();
    testGetQuestion();
    testLoadQuestions();
    testStartupPerformance();

    std::cout << (failures == 0 ? "\nAll validation tests passed.\n" : "\nSome tests failed.\n");
    return failures == 0 ? 0 : 1;
}
