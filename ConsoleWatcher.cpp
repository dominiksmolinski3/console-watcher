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
    : logger_(logger), observedString_(observedString) {
    observedString_.Subscribe(*this);
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
    printMessage();
    waitForExit();   // Keep the application running until a key is pressed
}

void ConsoleWatcher::HandleEvent(const ObservedString& ref) {

    if (ref.GetValue() == "Enter") {
        ConsoleColor::readConsoleOutput(10);

        // Log the action
        logger_->log("Enter");
    } else if (ref.GetValue() == "Leave") {
        ConsoleColor::readConsoleOutput(7);

        // Log the action
        logger_->log("Leave");
    }
    
}
