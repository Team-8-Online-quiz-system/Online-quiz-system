#include "quiz_runner.hpp"

#include <istream>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <utility>

#include "console_ui.hpp"
#include "quiz_engine.hpp"
#include "scoring.hpp"

namespace oqs {

namespace {

// Indents each new line written through it, so the quiz engine's prompts and
// error messages line up with the styled screen.
class IndentingBuffer : public std::streambuf {
public:
    IndentingBuffer(std::ostream& target, std::string indent)
        : target_(target), indent_(std::move(indent)) {}

protected:
    int overflow(int ch) override {
        if (ch == traits_type::eof()) {
            return traits_type::not_eof(ch);
        }
        if (atLineStart_ && ch != '\n') {
            target_ << indent_;
        }
        target_.put(static_cast<char>(ch));
        atLineStart_ = ch == '\n';
        return ch;
    }

    int sync() override {
        target_.flush();
        return 0;
    }

private:
    std::ostream& target_;
    std::string indent_;
    bool atLineStart_ = false;
};

}  // namespace

bool runQuiz(std::istream& in, std::ostream& out, SessionState& session,
             const std::vector<Question>& bank) {
    while (!session.completed) {
        const std::size_t index = session.currentQuestionIndex;
        const Question& question = getQuestion(bank, index);

        if (ui::styledOutput()) {
            ui::clearScreen(out);
        } else {
            out << '\n';
        }
        ui::showProgress(out, session);

        std::ostringstream rendered;
        displayQuestion(rendered, question.text, question.options);
        ui::showQuestion(out, rendered.str());

        ui::showAnswerCue(out);
        IndentingBuffer indented(out, "  ");
        std::ostream promptOut(&indented);
        const auto answer = promptAnswer(in, ui::styledOutput() ? promptOut : out,
                                         question.options.size());
        if (!answer) {
            out << '\n';
            return false;
        }

        recordResponse(session.acceptedResponses, index, *answer);
        const bool correct = evaluateAnswer(question.correctIndex, *answer);
        session.score = updateScore(session.score, correct);

        std::ostringstream feedback;
        showFeedback(feedback, correct, question.correctIndex);
        ui::showFeedbackLine(out, feedback.str(), correct);
        advanceQuestion(session);

        const bool last = session.completed;
        if (!ui::waitForEnter(in, out, last ? "see your result" : "continue")) {
            return false;
        }
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
