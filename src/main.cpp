#include <iostream>

#include "console_ui.hpp"
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

    // The question count comes from the question bank once feature/question-bank is merged.
    SessionState session = startSession(*name, 0);
    std::cout << "\nWelcome, " << session.playerName << "! Your score starts at "
              << session.score << ".\n";

    endSession(session);
    return 0;
}
