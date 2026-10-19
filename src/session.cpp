#include "session.hpp"

namespace oqs {

SessionState startSession(const std::string& playerName, std::size_t totalQuestions) {
    SessionState session;
    session.playerName = playerName;
    session.totalQuestions = totalQuestions;
    resetSession(session);
    return session;
}

void resetSession(SessionState& session) {
    session.currentQuestionIndex = 0;
    session.score = 0;
    session.acceptedResponses.assign(session.totalQuestions, kNoResponse);
    session.completed = session.totalQuestions == 0;
}

void advanceQuestion(SessionState& session) {
    if (session.completed) {
        return;
    }
    ++session.currentQuestionIndex;
    if (session.currentQuestionIndex >= session.totalQuestions) {
        session.currentQuestionIndex = session.totalQuestions;
        session.completed = true;
    }
}

void endSession(SessionState& session) {
    session.completed = true;
}

}  // namespace oqs
