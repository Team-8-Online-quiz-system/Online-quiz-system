#include "question_bank.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace oqs {
namespace {

bool isBlank(const std::string& value) {
    return std::all_of(value.begin(), value.end(), [](unsigned char character) {
        return std::isspace(character) != 0;
    });
}

}  // namespace

// OQS-SR-003
std::string questionError(const Question& question) {
    if (isBlank(question.text)) {
        return "question text is empty";
    }
    if (question.text.size() > kMaxQuestionLength) {
        return "question text is longer than 512 characters";
    }
    if (question.options.size() != kOptionCount) {
        return "expected 4 options, found " + std::to_string(question.options.size());
    }
    for (std::size_t i = 0; i < question.options.size(); ++i) {
        if (isBlank(question.options[i])) {
            return "option " + std::to_string(i + 1) + " is empty";
        }
    }
    for (std::size_t i = 0; i < question.options.size(); ++i) {
        if (question.options[i].size() > kMaxOptionLength) {
            return "option " + std::to_string(i + 1) + " is longer than 256 characters";
        }
    }
    for (std::size_t i = 0; i < question.options.size(); ++i) {
        for (std::size_t j = 0; j < i; ++j) {
            if (question.options[i] == question.options[j]) {
                return "option " + std::to_string(i + 1) + " duplicates option " + std::to_string(j + 1);
            }
        }
    }
    if (!isValidOptionIndex(question, question.correctIndex)) {
        return "correct answer is not one of the options";
    }
    return "";
}

// OQS-F-005
bool validateQuestion(const Question& question) {
    return questionError(question).empty();
}

std::vector<Question> loadQuestions() {
    const std::vector<Question> builtInQuestions = {
        {"What is the primary role of a CPU?",
         {"Store files permanently", "Execute program instructions", "Display web pages", "Connect to Wi-Fi"},
         1},
        {"What is the smallest unit of digital information?",
         {"Bit", "Byte", "Kilobyte", "Word"},
         0},
        {"What does HTML stand for?",
         {"High-level Text Management Language", "HyperText Markup Language", "Hyperlink and Transfer Machine Language", "Home Tool Markup Language"},
         1},
        {"Which type of memory loses its contents when power is turned off?",
         {"ROM", "SSD", "RAM", "Blu-ray disc"},
         2},
        {"Which data structure follows the last-in, first-out rule?",
         {"Queue", "Stack", "Tree", "Graph"},
         1},
    };

    std::vector<Question> validQuestions;
    validQuestions.reserve(builtInQuestions.size());

    for (std::size_t i = 0; i < builtInQuestions.size(); ++i) {
        if (validateQuestion(builtInQuestions[i])) {
            validQuestions.push_back(builtInQuestions[i]);
        } else {
            std::cerr << "Skipping invalid built-in question at index " << i << '\n';
        }
    }

    return validQuestions;
}

const Question& getQuestion(const std::vector<Question>& bank, std::size_t index) {
    if (!isValidQuestionIndex(bank, index)) {
        throw std::out_of_range("question index is out of range");
    }
    return bank[index];
}

// OQS-SR-002
bool isValidQuestionIndex(const std::vector<Question>& bank, std::size_t index) {
    return index < bank.size();
}

// OQS-SR-002
bool isValidOptionIndex(const Question& question, int option) {
    return option >= 0 && static_cast<std::size_t>(option) < question.options.size();
}

}  // namespace oqs
