#include "ConsoleWatcher.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "ConsoleColor.h"

// Cross-platform support for _kbhit() and _getch()
#ifdef _WIN32
#include <conio.h>  // Windows-specific includes
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Implement getch() and kbhit() for Unix-like systems
int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif

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
    while (!kbhit()) { // Use cross-platform kbhit()
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    getch(); // Use cross-platform getch() to consume the key press
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
