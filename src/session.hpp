#ifndef OQS_SESSION_HPP
#define OQS_SESSION_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace oqs {

// Marks a question that has not been answered yet in acceptedResponses.
constexpr int kNoResponse = -1;

// State of one quiz attempt (SAD 4.1.1 SessionState).
struct SessionState {
    std::string playerName;
    std::size_t currentQuestionIndex = 0;
    int score = 0;
    std::size_t totalQuestions = 0;
    std::vector<int> acceptedResponses;
    bool completed = false;
};

// Creates a fresh attempt with score 0 and question index 0 (OQS-F-003).
SessionState startSession(const std::string& playerName, std::size_t totalQuestions);

// Clears score, answers and progress so the same player can replay.
void resetSession(SessionState& session);

// Moves to the next question and marks the attempt completed after the last one.
void advanceQuestion(SessionState& session);

// Closes the active attempt.
void endSession(SessionState& session);

}  // namespace oqs

#endif
