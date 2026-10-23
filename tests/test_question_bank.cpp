#include <iostream>
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

void testValidation() {
    check(oqs::validateQuestion(sampleQuestion()), "valid question is accepted");

    oqs::Question question = sampleQuestion();
    question.text.clear();
    check(!oqs::validateQuestion(question), "empty question text is rejected");

    question = sampleQuestion();
    question.text = " \t \n";
    check(!oqs::validateQuestion(question), "blank question text is rejected");

    question = sampleQuestion();
    question.text = std::string(oqs::kMaxQuestionLength + 1, 'q');
    check(!oqs::validateQuestion(question), "question text over the limit is rejected");

    question = sampleQuestion();
    question.text = std::string(oqs::kMaxQuestionLength, 'q');
    question.options[0] = std::string(oqs::kMaxOptionLength, 'x');
    check(oqs::validateQuestion(question), "text and option at their maximum lengths are accepted");

    question = sampleQuestion();
    question.options.pop_back();
    check(!oqs::validateQuestion(question), "three options are rejected");

    question = sampleQuestion();
    question.options.push_back("Go");
    check(!oqs::validateQuestion(question), "five options are rejected");

    question = sampleQuestion();
    question.options[1].clear();
    check(!oqs::validateQuestion(question), "empty answer option is rejected");

    question = sampleQuestion();
    question.options[1] = "   ";
    check(!oqs::validateQuestion(question), "blank answer option is rejected");

    question = sampleQuestion();
    question.options[1] = question.options[0];
    check(!oqs::validateQuestion(question), "duplicate answer options are rejected");

    question = sampleQuestion();
    question.options[1] = std::string(oqs::kMaxOptionLength + 1, 'x');
    check(!oqs::validateQuestion(question), "answer option over the limit is rejected");

    question = sampleQuestion();
    question.correctIndex = -1;
    check(!oqs::validateQuestion(question), "negative correct answer index is rejected");

    question = sampleQuestion();
    question.correctIndex = 4;
    check(!oqs::validateQuestion(question), "correct answer index past the options is rejected");
}

void testQuestionBank() {
    const std::vector<oqs::Question> bank = oqs::loadQuestions();
    check(bank.size() >= 5, "built-in question bank has at least five questions");

    bool everyQuestionIsValid = true;
    for (const oqs::Question& question : bank) {
        if (!oqs::validateQuestion(question)) {
            everyQuestionIsValid = false;
        }
    }
    check(everyQuestionIsValid, "all loaded questions are valid");
}

void testGetQuestion() {
    std::vector<oqs::Question> bank = {sampleQuestion(), sampleQuestion()};
    bank[1].text = "What does RAM stand for?";

    check(oqs::getQuestion(bank, 1).text == bank[1].text,
          "getQuestion returns the requested question");

    bool threwOutOfRange = false;
    try {
        static_cast<void>(oqs::getQuestion(bank, bank.size()));
    } catch (const std::out_of_range&) {
        threwOutOfRange = true;
    }
    check(threwOutOfRange, "getQuestion throws for an out-of-range index");
}

}  // namespace

int main() {
    testValidation();
    testQuestionBank();
    testGetQuestion();

    std::cout << (failures == 0 ? "\nAll tests passed.\n" : "\nSome tests failed.\n");
    return failures == 0 ? 0 : 1;
}
