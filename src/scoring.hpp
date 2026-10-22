#ifndef OQS_SCORING_HPP
#define OQS_SCORING_HPP

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

namespace oqs {

// OQS-F-011
bool evaluateAnswer(int correctIndex, int selectedOption);

// OQS-F-012
int updateScore(int currentScore, bool isCorrect);

// OQS-F-013
void showFeedback(std::ostream& out, bool isCorrect, int correctIndex);

// OQS-F-015
bool allQuestionsAnswered(const std::vector<int>& responses);

// OQS-F-016
int calculateFinalScore(const std::vector<int>& responses,
                        const std::vector<int>& correctIndexes);

// OQS-F-016
void showFinalScore(std::ostream& out, const std::string& playerName, int score,
                    std::size_t totalQuestions);

}  // namespace oqs

#endif  // OQS_SCORING_HPP
