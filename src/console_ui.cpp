#include "console_ui.hpp"

#include <algorithm>
#include <cctype>
#include <istream>
#include <ostream>
#include <sstream>

namespace oqs::ui {

namespace {

bool styled = false;

constexpr std::size_t kScreenWidth = 56;
constexpr std::size_t kBarWidth = 24;

std::string trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

std::string repeat(const std::string& piece, std::size_t count) {
    std::string result;
    for (std::size_t i = 0; i < count; ++i) {
        result += piece;
    }
    return result;
}

// Width calculations below assume ASCII text; player names are ASCII-only.
std::string padRight(const std::string& text, std::size_t width) {
    return text.size() >= width ? text : text + std::string(width - text.size(), ' ');
}

std::string centre(const std::string& text, std::size_t width) {
    if (text.size() >= width) {
        return text;
    }
    const std::size_t left = (width - text.size()) / 2;
    return std::string(left, ' ') + text + std::string(width - text.size() - left, ' ');
}

std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}

void boxTop(std::ostream& out, std::size_t inner, const std::string& title) {
    out << "  ╭";
    if (title.empty()) {
        out << repeat("─", inner);
    } else {
        out << "── " << paint(title, Tone::Accent) << ' '
            << repeat("─", inner - title.size() - 4);
    }
    out << "╮\n";
}

void boxRow(std::ostream& out, std::size_t inner, const std::string& text, Tone tone) {
    out << "  │  " << paint(padRight(text, inner - 4), tone) << "  │\n";
}

void boxBottom(std::ostream& out, std::size_t inner) {
    out << "  ╰" << repeat("─", inner) << "╯\n";
}

void showPrompt(std::ostream& out, const std::string& prompt) {
    if (styled) {
        out << "  " << paint("›", Tone::Accent) << ' ';
    }
    out << prompt;
}

void showError(std::ostream& out, const std::string& message) {
    if (styled) {
        out << "  " << paint("✘ " + message, Tone::Error) << '\n';
    } else {
        out << message << '\n';
    }
}

// Shows a two-option numbered menu until 1 or 2 is entered.
// Returns 1 or 2, or std::nullopt if input ends.
std::optional<int> promptTwoOptionMenu(std::istream& in, std::ostream& out,
                                       const std::string& first, const std::string& second) {
    std::string line;
    while (true) {
        if (styled) {
            out << "    " << paint("[1]", Tone::Accent) << "  " << first << '\n'
                << "    " << paint("[2]", Tone::Accent) << "  " << second << "\n\n";
        } else {
            out << "1. " << first << '\n'
                << "2. " << second << '\n';
        }
        showPrompt(out, "Choose an option (1-2): ");
        if (!std::getline(in, line)) {
            out << '\n';
            return std::nullopt;
        }
        const std::string choice = trim(line);
        if (choice == "1") {
            return 1;
        }
        if (choice == "2") {
            return 2;
        }
        showError(out, "Invalid choice. Please enter 1 or 2.");
        out << '\n';
    }
}

}  // namespace

void setStyledOutput(bool enabled) {
    styled = enabled;
}

bool styledOutput() {
    return styled;
}

std::string paint(const std::string& text, Tone tone) {
    if (!styled) {
        return text;
    }
    const char* code = "";
    switch (tone) {
        case Tone::Title: code = "\x1b[1;35m"; break;
        case Tone::Accent: code = "\x1b[1;36m"; break;
        case Tone::Success: code = "\x1b[1;32m"; break;
        case Tone::Error: code = "\x1b[1;31m"; break;
        case Tone::Highlight: code = "\x1b[1;97m"; break;
        case Tone::Muted: code = "\x1b[2m"; break;
    }
    return code + text + "\x1b[0m";
}

void clearScreen(std::ostream& out) {
    if (styled) {
        out << "\x1b[2J\x1b[H";
    }
}

void showWelcome(std::ostream& out) {
    if (!styled) {
        out << "========================================\n"
            << "          ONLINE QUIZ SYSTEM\n"
            << "========================================\n"
            << "Answer each multiple-choice question to\n"
            << "test your knowledge. Good luck!\n\n";
        return;
    }
    const std::size_t inner = kScreenWidth - 2;
    out << '\n';
    boxTop(out, inner, "");
    boxRow(out, inner, "", Tone::Highlight);
    out << "  │" << paint(centre("O N L I N E   Q U I Z   S Y S T E M", inner), Tone::Title) << "│\n";
    out << "  │" << centre("Test your knowledge, one question at a time.", inner) << "│\n";
    boxRow(out, inner, "", Tone::Highlight);
    boxBottom(out, inner);
    out << "  " << paint(centre("Team 8 · Software Engineering mini-project", inner + 1), Tone::Muted)
        << "\n\n";
}

std::optional<StartChoice> promptStart(std::istream& in, std::ostream& out) {
    const auto choice = promptTwoOptionMenu(in, out, "Start Quiz", "Exit");
    if (!choice) {
        return std::nullopt;
    }
    return *choice == 1 ? StartChoice::Start : StartChoice::Exit;
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
    const std::string accepted =
        "1-" + std::to_string(kMaxNameLength) + " letters, digits, spaces, _ or -";
    std::string line;
    if (styled) {
        out << '\n';
    }
    while (true) {
        showPrompt(out, "Enter your name (" + accepted + "): ");
        if (!std::getline(in, line)) {
            out << '\n';
            return std::nullopt;
        }
        const std::string name = trim(line);
        if (isValidPlayerName(name)) {
            return name;
        }
        showError(out, "Invalid name. Please enter " + accepted + ".");
    }
}

std::string formatProgress(std::size_t questionIndex, std::size_t totalQuestions) {
    return "Question " + std::to_string(questionIndex + 1) + " of " +
           std::to_string(totalQuestions);
}

void showProgress(std::ostream& out, const SessionState& session) {
    const std::string progress =
        formatProgress(session.currentQuestionIndex, session.totalQuestions);
    if (!styled) {
        out << progress << '\n';
        return;
    }

    const std::string title = "ONLINE QUIZ SYSTEM";
    const std::string player = "Player: " + session.playerName;
    const std::size_t gap = kScreenWidth > title.size() + player.size() + 2
                                ? kScreenWidth - title.size() - player.size() - 2
                                : 1;
    out << '\n' << "  " << paint(title, Tone::Title) << std::string(gap, ' ')
        << paint(player, Tone::Muted) << '\n';

    const std::size_t total = std::max<std::size_t>(session.totalQuestions, 1);
    const std::size_t filled = session.currentQuestionIndex * kBarWidth / total;
    out << "  " << paint(padRight(progress, 16), Tone::Accent) << ' '
        << paint(repeat("█", filled), Tone::Accent) << paint(repeat("░", kBarWidth - filled), Tone::Muted)
        << "  " << paint("Score: " + std::to_string(session.score), Tone::Success) << '\n';
    out << "  " << paint(repeat("─", kScreenWidth - 2), Tone::Muted) << '\n';
}

void showQuestion(std::ostream& out, const std::string& rendered) {
    if (!styled) {
        out << rendered;
        return;
    }
    out << '\n';
    for (const std::string& line : splitLines(rendered)) {
        const bool isOption = line.size() >= 5 && line.compare(0, 2, "  ") == 0 &&
                              std::isupper(static_cast<unsigned char>(line[2])) && line[3] == ')';
        if (isOption) {
            out << "    " << paint(std::string("[") + line[2] + "]", Tone::Accent) << ' '
                << line.substr(4) << '\n';
        } else {
            out << "  " << paint(line, Tone::Highlight) << "\n\n";
        }
    }
}

void showAnswerCue(std::ostream& out) {
    if (styled) {
        out << "\n  " << paint("›", Tone::Accent) << ' ';
    }
}

void showFeedbackLine(std::ostream& out, const std::string& feedback, bool correct) {
    if (!styled) {
        out << feedback;
        return;
    }
    const std::string text = trim(feedback);
    out << "\n  " << paint((correct ? "✔ " : "✘ ") + text, correct ? Tone::Success : Tone::Error)
        << '\n';
}

bool waitForEnter(std::istream& in, std::ostream& out, const std::string& action) {
    if (!styled) {
        return true;
    }
    out << "\n  " << paint("Press Enter to " + action + "...", Tone::Muted);
    std::string line;
    if (!std::getline(in, line)) {
        out << '\n';
        return false;
    }
    return true;
}

namespace {

constexpr std::size_t kResultBarWidth = 30;

void drawResultPanel(std::ostream& out, const std::vector<std::string>& lines,
                     std::optional<double> percentage) {
    if (!styled) {
        for (const std::string& line : lines) {
            out << line << '\n';
        }
        return;
    }
    std::size_t longest = 0;
    for (const std::string& line : lines) {
        longest = std::max(longest, line.size());
    }
    const std::size_t inner = std::max(kScreenWidth - 2, longest + 4);
    out << '\n';
    boxTop(out, inner, "YOUR RESULT");
    boxRow(out, inner, "", Tone::Highlight);
    for (const std::string& line : lines) {
        boxRow(out, inner, line, Tone::Highlight);
    }

    if (percentage) {
        const double pct = std::clamp(*percentage, 0.0, 100.0);
        const auto filled = static_cast<std::size_t>(pct / 100.0 * kResultBarWidth + 0.5);
        std::ostringstream label;
        label.setf(std::ios::fixed);
        label.precision(1);
        label << pct << '%';
        const Tone tone = pct >= 80.0 ? Tone::Success : pct >= 50.0 ? Tone::Accent : Tone::Error;
        const std::string rating = pct >= 80.0   ? "Excellent work!"
                                   : pct >= 50.0 ? "Good effort!"
                                                 : "Keep practising!";

        boxRow(out, inner, "", Tone::Highlight);
        const std::size_t used = kResultBarWidth + 1 + label.str().size();
        out << "  │  " << paint(repeat("█", filled), tone)
            << paint(repeat("░", kResultBarWidth - filled), Tone::Muted) << ' '
            << paint(label.str(), tone) << std::string(inner - 4 - used, ' ') << "  │\n";
        boxRow(out, inner, rating, tone);
    }

    boxRow(out, inner, "", Tone::Highlight);
    boxBottom(out, inner);
}

}  // namespace

void showResultPanel(std::ostream& out, const std::vector<std::string>& lines) {
    drawResultPanel(out, lines, std::nullopt);
}

void showResultPanel(std::ostream& out, const std::vector<std::string>& lines,
                     double percentage) {
    drawResultPanel(out, lines, percentage);
}

std::optional<ReplayChoice> promptReplay(std::istream& in, std::ostream& out) {
    out << '\n';
    const auto choice = promptTwoOptionMenu(in, out, "Play again", "Exit");
    if (!choice) {
        return std::nullopt;
    }
    return *choice == 1 ? ReplayChoice::Replay : ReplayChoice::Exit;
}

void showGoodbye(std::ostream& out, const std::string& playerName) {
    const std::string message =
        playerName.empty() ? "Goodbye!" : "Thanks for playing, " + playerName + "! Goodbye.";
    if (styled) {
        out << "\n  " << paint(message, Tone::Accent) << "\n\n";
    } else {
        out << message << '\n';
    }
}

}  // namespace oqs::ui
