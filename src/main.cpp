#include <iostream>
#include <vector>

#include "console_ui.hpp"
#include "question_bank.hpp"
#include "quiz_runner.hpp"
#include "scoring.hpp"
#include "session.hpp"

int main() {
    using namespace oqs;

    ui::showWelcome(std::cout);

    const auto choice = ui::promptStart(std::cin, std::cout);
    if (!choice || *choice == ui::StartChoice::Exit) {
        std::cout << "Goodbye!\n";
        return 0;
    }

    const auto name = ui::promptPlayerName(std::cin, std::cout);
    if (!name) {
        std::cout << "Goodbye!\n";
        return 0;
    }

    const std::vector<Question> bank = loadQuestions();
    if (bank.empty()) {
        std::cerr << "No valid questions could be loaded. Exiting.\n";
        return 1;
    }

    SessionState session = startSession(*name, bank.size());
    std::cout << "\nWelcome, " << session.playerName << "! There are "
              << session.totalQuestions << " questions. Your score starts at "
              << session.score << ".\n";

    if (!runQuiz(std::cin, std::cout, session, bank)) {
        std::cout << "Quiz ended before the last question. Goodbye!\n";
        return 0;
    }

    const int finalScore = calculateFinalScore(session.acceptedResponses, answerKey(bank));
    std::cout << '\n';
    showFinalScore(std::cout, session.playerName, finalScore, session.totalQuestions);

    endSession(session);
    return 0;
}
