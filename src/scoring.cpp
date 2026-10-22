#include "scoring.hpp"

#include <limits>
#include <ostream>
#include <stdexcept>

namespace oqs {

bool evaluateAnswer(int correctIndex, int selectedOption) {
    return correctIndex >= 0 && selectedOption >= 0 && correctIndex == selectedOption;
}

int updateScore(int currentScore, bool isCorrect) {
    if (isCorrect && currentScore == std::numeric_limits<int>::max()) {
        throw std::overflow_error("The quiz score cannot be increased further.");
    }
    return currentScore + (isCorrect ? 1 : 0);
}

void showFeedback(std::ostream& out, bool isCorrect, int correctIndex) {
    if (isCorrect) {
        out << "Correct!\n";
        return;
    }

    const char correctLetter = correctIndex >= 0 && correctIndex <= 25
                                   ? static_cast<char>('A' + correctIndex)
                                   : '?';
    out << "Incorrect. The correct answer was " << correctLetter << ".\n";
}

bool allQuestionsAnswered(const std::vector<int>& responses) {
    if (responses.empty()) {
        return false;
    }
    for (const int response : responses) {
        if (response == -1) {
            return false;
        }
    }
    return true;
}

}  // namespace oqs
