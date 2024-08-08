#include "ConsoleWatcher.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "ConsoleColor.h"
#include <conio.h> // For _kbhit() and _getch()

ConsoleWatcher::ConsoleWatcher(std::shared_ptr<Logger> logger, ObservedString& observedString)
    : logger_(logger), observedString_(observedString) {
    observedString_.Subscribe(*this);
}

void ConsoleWatcher::printMessage() {
    std::cout << "Hello World" << std::endl;
}

void ConsoleWatcher::waitForExit() {
    std::cout << "Press any key to exit..." << std::endl;
    while (!_kbhit()) { // Wait for key press
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    _getch(); // Consume the key press
}

void ConsoleWatcher::run() {
    printMessage();
    waitForExit(); // Keep the application running until a key is pressed
}

void ConsoleWatcher::HandleEvent(const ObservedString& ref) {
    if (ref.GetValue() == "Enter") {
        ConsoleColor::clearConsole();
        ConsoleColor::setColor(10); // Change to green
        std::cout << "Hello World" << std::endl;
        logger_->log("Enter");
    } else if (ref.GetValue() == "Leave") {
        ConsoleColor::clearConsole();
        ConsoleColor::resetColor();
        std::cout << "Hello World" << std::endl;
        logger_->log("Leave");
    }
}
