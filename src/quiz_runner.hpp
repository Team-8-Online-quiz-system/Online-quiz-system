#ifndef OQS_QUIZ_RUNNER_HPP
#define OQS_QUIZ_RUNNER_HPP

#include <iosfwd>
#include <vector>

#include "question_bank.hpp"
#include "session.hpp"

namespace oqs {

// Asks every question once in order, recording answers and updating the score
// (OQS-F-006 to OQS-F-015). Returns false if input ends before the last question.
bool runQuiz(std::istream& in, std::ostream& out, SessionState& session,
             const std::vector<Question>& bank);

// The correct option index of each question, in bank order.
std::vector<int> answerKey(const std::vector<Question>& bank);

}  // namespace oqs

#endif
