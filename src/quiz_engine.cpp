#include "quiz_engine.hpp"

#include <cctype>
#include <string>

namespace oqs {
namespace {

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

}  // namespace oqs
