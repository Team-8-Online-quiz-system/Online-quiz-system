#include "scoring.hpp"

#include <iomanip>
#include <limits>
#include <ostream>
#include <sstream>
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

int calculateFinalScore(const std::vector<int>& responses,
                        const std::vector<int>& correctIndexes) {
    if (responses.size() != correctIndexes.size()) {
        throw std::invalid_argument("Responses and answer key must have the same size.");
    }
    if (responses.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::length_error("The question count exceeds the supported score range.");
    }

    int score = 0;
    for (std::size_t i = 0; i < responses.size(); ++i) {
        if (responses[i] == correctIndexes[i]) {
            ++score;
        }
    }
    return score;
}

void showFinalScore(std::ostream& out, const std::string& playerName, int score,
                    std::size_t totalQuestions) {
    out << "Final score for " << playerName << ": " << score << " / "
        << totalQuestions << '\n';
}

double calculatePercentage(int score, std::size_t totalQuestions) {
    if (totalQuestions == 0) {
        return 0.0;
    }
    return (static_cast<double>(score) / static_cast<double>(totalQuestions)) * 100.0;
}

std::string formatPercentage(double percentage) {
    std::ostringstream formatted;
    formatted << std::fixed << std::setprecision(1) << percentage << '%';
    return formatted.str();
}

bool isScoreInRange(int score, std::size_t totalQuestions) {
    return score >= 0 && static_cast<std::size_t>(score) <= totalQuestions;
}

void showResultSummary(std::ostream& out, const std::string& playerName, int score,
                       std::size_t totalQuestions) {
    if (!isScoreInRange(score, totalQuestions)) {
        throw std::out_of_range("The score is outside the valid quiz range.");
    }

    out << "Player: " << playerName << '\n'
        << "Score: " << score << " / " << totalQuestions << '\n'
        << "Percentage: "
        << formatPercentage(calculatePercentage(score, totalQuestions)) << '\n';
}

}  // namespace oqs
