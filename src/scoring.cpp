#include "scoring.hpp"

namespace oqs {

bool evaluateAnswer(int correctIndex, int selectedOption) {
    return correctIndex >= 0 && selectedOption >= 0 && correctIndex == selectedOption;
}

}  // namespace oqs
