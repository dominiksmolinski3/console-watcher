#include "ConsoleWatcher.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "ConsoleColor.h"
#include <conio.h> // For _kbhit() and _getch()

ConsoleWatcher::ConsoleWatcher(std::shared_ptr<Logger> logger, ObservedString& observedString)
    : logger_(logger), observedString_(observedString), oldCoutBuffer_(std::cout.rdbuf()), buffer_() {
    observedString_.Subscribe(*this);

    // Create the TeeBuffer object to write to both the console and buffer_
    teeBuffer_ = std::make_unique<TeeBuffer>(oldCoutBuffer_, buffer_.rdbuf());
    std::cout.rdbuf(teeBuffer_.get());
}

void ConsoleWatcher::printMessage() {
    std::cout << "Hello World" << std::endl;
    std::cout << "Console Watcher is on!" << std::endl;
}

void ConsoleWatcher::waitForExit() {
    std::cout << "Press any key to exit..." << std::endl;
    while (!_kbhit()) { // Wait for key press
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    _getch(); // Consume the key press
}

void ConsoleWatcher::run() {
    printMessage();  // This should be called when the program starts
    waitForExit();   // Keep the application running until a key is pressed
}

void ConsoleWatcher::HandleEvent(const ObservedString& ref) {
    // Convert buffer content to a string before any operations
    std::string output = buffer_.str();

    if (ref.GetValue() == "Enter") {
        if (!isConsoleCleared_ || !isColorSet_) {
            ConsoleColor::clearConsole();  // Clear the console on entry
            ConsoleColor::setColor(10);    // Change color to green
            isConsoleCleared_ = true;
            isColorSet_ = true;

            // Print the buffer's content dynamically if it's not empty
            if (!output.empty()) {
                std::cout << output << std::endl;
            }
        }

        // Log the action
        logger_->log("Enter");

        // Reset the buffer with the new content
        buffer_.str(output);  // Put the output back into the buffer
        buffer_.clear();      // Clear any error flags

    } else if (ref.GetValue() == "Leave") {
        if (isConsoleCleared_) {
            ConsoleColor::clearConsole();  // Clear the console on exit
            ConsoleColor::setColor(7);     // Reset color to default (light gray)
            isConsoleCleared_ = false;
            isColorSet_ = false;

            // Reprint the buffer's content with default color if it's not empty
            if (!output.empty()) {
                std::cout << output << std::endl;
            }

            // Reset the buffer with the content after reprinting
            buffer_.str(output);  // Put the output back into the buffer
            buffer_.clear();      // Clear any error flags
        }

        // Log the action
        logger_->log("Leave");
    }

    // Restore the original std::cout buffer
    std::cout.rdbuf(oldCoutBuffer_);
}




