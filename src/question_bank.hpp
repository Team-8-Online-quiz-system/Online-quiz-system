#ifndef OQS_QUESTION_BANK_HPP
#define OQS_QUESTION_BANK_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace oqs {

constexpr std::size_t kOptionCount = 4;
constexpr std::size_t kMaxQuestionLength = 512;
constexpr std::size_t kMaxOptionLength = 256;

struct Question {
    std::string text;
    std::vector<std::string> options;
    int correctIndex = -1;
};

// OQS-SR-003: the reason a record is invalid, or "" if it is valid.
// Reasons, checked in this order (N and M are 1-based):
//   "question text is empty"
//   "question text is longer than 512 characters"
//   "expected 4 options, found <count>"
//   "option <N> is empty"
//   "option <N> is longer than 256 characters"
//   "option <N> duplicates option <M>"
//   "correct answer is not one of the options"
std::string questionError(const Question& question);

// OQS-F-005
bool validateQuestion(const Question& question);

// OQS-F-004
std::vector<Question> loadQuestions();

const Question& getQuestion(const std::vector<Question>& bank, std::size_t index);

// OQS-SR-002: true if index < bank.size().
bool isValidQuestionIndex(const std::vector<Question>& bank, std::size_t index);

// OQS-SR-002: true if 0 <= option < question.options.size().
bool isValidOptionIndex(const Question& question, int option);

}  // namespace oqs

#endif
