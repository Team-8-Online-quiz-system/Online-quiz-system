#include "quiz_engine.hpp"

namespace oqs {

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

}  // namespace oqs
