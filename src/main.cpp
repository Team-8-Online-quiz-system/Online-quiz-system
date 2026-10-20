#include <iostream>

#include "console_ui.hpp"

int main() {
    using namespace oqs;

    ui::showWelcome(std::cout);

    const auto choice = ui::promptStart(std::cin, std::cout);
    if (!choice || *choice == ui::StartChoice::Exit) {
        std::cout << "Goodbye!\n";
        return 0;
    }

    std::cout << "\nStarting quiz...\n";
    return 0;
}
