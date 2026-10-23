#include "quiz_runner.hpp"

#include <istream>
#include <ostream>

#include "console_ui.hpp"
#include "quiz_engine.hpp"
#include "scoring.hpp"

namespace oqs {

bool runQuiz(std::istream& in, std::ostream& out, SessionState& session,
             const std::vector<Question>& bank) {
    while (!session.completed) {
        const std::size_t index = session.currentQuestionIndex;
        const Question& question = getQuestion(bank, index);

        out << '\n';
        ui::showProgress(out, session);
        displayQuestion(out, question.text, question.options);

        const auto answer = promptAnswer(in, out, question.options.size());
        if (!answer) {
            out << '\n';
            return false;
        }

        recordResponse(session.acceptedResponses, index, *answer);
        const bool correct = evaluateAnswer(question.correctIndex, *answer);
        session.score = updateScore(session.score, correct);
        showFeedback(out, correct, question.correctIndex);
        advanceQuestion(session);
    }
    return allQuestionsAnswered(session.acceptedResponses);
}

std::vector<int> answerKey(const std::vector<Question>& bank) {
    std::vector<int> key;
    key.reserve(bank.size());
    for (const Question& question : bank) {
        key.push_back(question.correctIndex);
    }
    return key;
}

}  // namespace oqs
