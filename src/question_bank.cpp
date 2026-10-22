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

bool validateQuestion(const Question& question) {
    if (isBlank(question.text) || question.text.size() > kMaxQuestionLength ||
        question.options.size() != kOptionCount || question.correctIndex < 0 ||
        static_cast<std::size_t>(question.correctIndex) >= kOptionCount) {
        return false;
    }

    for (std::size_t i = 0; i < question.options.size(); ++i) {
        const std::string& option = question.options[i];
        if (isBlank(option) || option.size() > kMaxOptionLength) {
            return false;
        }

        for (std::size_t j = 0; j < i; ++j) {
            if (option == question.options[j]) {
                return false;
            }
        }
    }

    return true;
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
    if (index >= bank.size()) {
        throw std::out_of_range("question index is out of range");
    }
    return bank[index];
}

}  // namespace oqs
