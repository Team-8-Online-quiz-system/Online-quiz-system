#include <iostream>
#include <string>

#include "app.hpp"
#include "console_ui.hpp"
#include "terminal.hpp"

int main(int argc, char* argv[]) {
    bool plain = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--plain") {
            plain = true;
        }
    }

    oqs::ui::setStyledOutput(!plain && oqs::terminal::enableStyledOutput());
    return oqs::runApp(std::cin, std::cout, std::cerr);
}
