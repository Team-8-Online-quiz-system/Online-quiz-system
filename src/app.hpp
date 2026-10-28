#ifndef OQS_APP_HPP
#define OQS_APP_HPP

#include <iosfwd>

namespace oqs {

// Runs the whole application: welcome, name entry, quiz, result, and the
// replay/exit loop (OQS-F-019, OQS-F-020). Returns the process exit code:
// 0 on a normal exit or when input ends, 1 if no valid questions load.
int runApp(std::istream& in, std::ostream& out, std::ostream& err);

}  // namespace oqs

#endif
