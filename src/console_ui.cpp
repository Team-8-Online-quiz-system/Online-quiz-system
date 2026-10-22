#include "console_ui.hpp"

#include <cctype>
#include <istream>
#include <ostream>

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

bool isValidPlayerName(const std::string& name) {
    if (name.empty() || name.size() > kMaxNameLength) {
        return false;
    }
    for (const char c : name) {
        const auto uc = static_cast<unsigned char>(c);
        if (!std::isalnum(uc) && c != ' ' && c != '_' && c != '-') {
            return false;
        }
    }
    return true;
}

std::optional<std::string> promptPlayerName(std::istream& in, std::ostream& out) {
    std::string line;
    while (true) {
        out << "Enter your name (1-" << kMaxNameLength
            << " letters, digits, spaces, _ or -): ";
        if (!std::getline(in, line)) {
            out << '\n';
            return std::nullopt;
        }
        const std::string name = trim(line);
        if (isValidPlayerName(name)) {
            return name;
        }
        if (name.empty()) {
            out << "Name cannot be empty. Please try again.\n";
        } else {
            out << "Invalid name. Please try again.\n";
        }
    }
}

std::string formatProgress(std::size_t questionIndex, std::size_t totalQuestions) {
    return "Question " + std::to_string(questionIndex + 1) + " of " +
           std::to_string(totalQuestions);
}

void showProgress(std::ostream& out, const SessionState& session) {
    out << formatProgress(session.currentQuestionIndex, session.totalQuestions) << '\n';
}

}  // namespace oqs::ui
