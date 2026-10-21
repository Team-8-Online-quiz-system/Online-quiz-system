#include "quiz_engine.hpp"

#include <cctype>
#include <string>

namespace oqs {
namespace {

constexpr std::size_t kMaxSupportedOptions = 4;

std::string answerChoices(std::size_t optionCount) {
    const std::size_t count = optionCount < kMaxSupportedOptions
                                  ? optionCount
                                  : kMaxSupportedOptions;
    if (count == 0) {
        return "no available options";
    }
    if (count == 1) {
        return std::string(1, optionLabel(0)) + " or 1";
    }
    return std::string("A-") + optionLabel(count - 1) + " or 1-" +
           static_cast<char>('0' + count);
}

std::string trimWhitespace(const std::string& input) {
    std::size_t first = 0;
    while (first < input.size() &&
           std::isspace(static_cast<unsigned char>(input[first])) != 0) {
        ++first;
    }

    std::size_t last = input.size();
    while (last > first &&
           std::isspace(static_cast<unsigned char>(input[last - 1])) != 0) {
        --last;
    }

    return input.substr(first, last - first);
}

}  // namespace

char optionLabel(std::size_t index) {
    if (index >= 26) {
        return '?';
    }
    return static_cast<char>('A' + static_cast<int>(index));
}

void displayQuestion(std::ostream& out, const std::string& text,
                     const std::vector<std::string>& options) {
    out << text << '\n';
    for (std::size_t index = 0; index < options.size(); ++index) {
        out << "  " << optionLabel(index) << ") " << options[index] << '\n';
    }
}

std::optional<int> parseAnswer(const std::string& input, std::size_t optionCount) {
    const std::string trimmed = trimWhitespace(input);
    if (trimmed.size() != 1) {
        return std::nullopt;
    }

    const unsigned char character = static_cast<unsigned char>(trimmed[0]);
    if (std::isalpha(character) != 0) {
        const char upper = static_cast<char>(std::toupper(character));
        if (upper < 'A' || upper > 'D') {
            return std::nullopt;
        }
        const int index = upper - 'A';
        if (static_cast<std::size_t>(index) >= optionCount) {
            return std::nullopt;
        }
        return index;
    }

    if (trimmed[0] >= '1' && trimmed[0] <= '4') {
        const int index = trimmed[0] - '1';
        if (static_cast<std::size_t>(index) >= optionCount) {
            return std::nullopt;
        }
        return index;
    }

    return std::nullopt;
}

std::optional<int> promptAnswer(std::istream& in, std::ostream& out,
                                std::size_t optionCount) {
    if (optionCount == 0) {
        return std::nullopt;
    }

    const std::string choices = answerChoices(optionCount);
    const std::string prompt = "Your answer (" + choices + "): ";
    const std::string error = "Invalid answer. Please enter " + choices + ".";

    std::string line;
    while (true) {
        out << prompt;
        if (!std::getline(in, line)) {
            return std::nullopt;
        }

        const std::optional<int> answer = parseAnswer(line, optionCount);
        if (answer.has_value()) {
            return answer;
        }
        out << error << '\n';
    }
}

}  // namespace oqs
