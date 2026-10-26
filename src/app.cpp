#include "app.hpp"

#include <istream>
#include <ostream>
#include <vector>

#include "console_ui.hpp"
#include "question_bank.hpp"
#include "quiz_runner.hpp"
#include "scoring.hpp"
#include "session.hpp"

namespace oqs {

int runApp(std::istream& in, std::ostream& out, std::ostream& err) {
    ui::showWelcome(out);

    const auto choice = ui::promptStart(in, out);
    if (!choice || *choice == ui::StartChoice::Exit) {
        ui::showGoodbye(out, "");
        return 0;
    }

    const auto name = ui::promptPlayerName(in, out);
    if (!name) {
        ui::showGoodbye(out, "");
        return 0;
    }

    const std::vector<Question> bank = loadQuestions();
    if (bank.empty()) {
        err << "No valid questions could be loaded. Exiting.\n";
        return 1;
    }

    SessionState session = startSession(*name, bank.size());
    out << "\nWelcome, " << session.playerName << "! There are "
        << session.totalQuestions << " questions. Your score starts at "
        << session.score << ".\n";

    while (true) {
        if (!runQuiz(in, out, session, bank)) {
            endSession(session);
            ui::showGoodbye(out, session.playerName);
            return 0;
        }

        const int finalScore = calculateFinalScore(session.acceptedResponses, answerKey(bank));
        out << '\n';
        showFinalScore(out, session.playerName, finalScore, session.totalQuestions);

        const auto again = ui::promptReplay(in, out);
        if (!again || *again == ui::ReplayChoice::Exit) {
            endSession(session);
            ui::showGoodbye(out, session.playerName);
            return 0;
        }

        // OQS-F-019: a replay is a fresh attempt for the same player.
        resetSession(session);
        out << "\nStarting a new attempt. Your score is back to " << session.score << ".\n";
    }
}

}  // namespace oqs
