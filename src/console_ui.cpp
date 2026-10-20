#include "console_ui.hpp"

#include <istream>
#include <ostream>
#include <string>

namespace oqs::ui {

namespace {

std::string trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

}  // namespace

void showWelcome(std::ostream& out) {
    out << "========================================\n"
        << "          ONLINE QUIZ SYSTEM\n"
        << "========================================\n"
        << "Answer each multiple-choice question to\n"
        << "test your knowledge. Good luck!\n\n";
}

std::optional<StartChoice> promptStart(std::istream& in, std::ostream& out) {
    std::string line;
    while (true) {
        out << "1. Start Quiz\n"
            << "2. Exit\n"
            << "Enter 1 or 2: ";
        if (!std::getline(in, line)) {
            out << '\n';
            return std::nullopt;
        }
        const std::string choice = trim(line);
        if (choice == "1") {
            return StartChoice::Start;
        }
        if (choice == "2") {
            return StartChoice::Exit;
        }
        out << "Invalid choice. Please enter 1 or 2.\n\n";
    }
}

}  // namespace oqs::ui
