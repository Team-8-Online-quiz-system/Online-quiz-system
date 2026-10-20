#ifndef OQS_QUIZ_ENGINE_HPP
#define OQS_QUIZ_ENGINE_HPP

#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

namespace oqs {

// 0 -> 'A', 1 -> 'B', ...
char optionLabel(std::size_t index);

// OQS-F-006/007: prints the question text, then one line per option.
void displayQuestion(std::ostream& out, const std::string& text,
                     const std::vector<std::string>& options);

}  // namespace oqs

#endif
