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

// OQS-F-005
bool validateQuestion(const Question& question);

// OQS-F-004
std::vector<Question> loadQuestions();

const Question& getQuestion(const std::vector<Question>& bank, std::size_t index);

}  // namespace oqs

#endif
