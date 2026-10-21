#include "scoring.hpp"

#include <limits>
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

}  // namespace oqs
