#ifndef OQS_QUIZ_ENGINE_HPP
#define OQS_QUIZ_ENGINE_HPP

#include <cstddef>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace oqs {

// 0 -> 'A', 1 -> 'B', ...
char optionLabel(std::size_t index);

// OQS-F-006/007: prints the question text, then one line per option.
void displayQuestion(std::ostream& out, const std::string& text,
                     const std::vector<std::string>& options);

// OQS-F-008: accepts a valid letter or digit and returns a zero-based index.
std::optional<int> parseAnswer(const std::string& input, std::size_t optionCount);

}  // namespace oqs

#endif
