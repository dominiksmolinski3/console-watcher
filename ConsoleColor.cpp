#include "ConsoleColor.h"

#ifdef _WIN32
void ConsoleColor::setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void ConsoleColor::resetColor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7); // Reset to default color
}

void ConsoleColor::clearConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten)) {
        return;
    }

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten)) {
        return;
    }

    SetConsoleCursorPosition(hConsole, coordScreen);
}
#else
void ConsoleColor::setColor(int color) {
    switch(color) {
        case 10: // Green
            std::cout << "\033[32m";
            break;
        case 7:  // Default (light gray)
        default:
            std::cout << "\033[0m";
            break;
    }
}

void ConsoleColor::resetColor() {
    std::cout << "\033[0m";  // Reset color using ANSI codes
}

void ConsoleColor::clearConsole() {
    std::cout << "\033[2J\033[H";  // Clear screen and move cursor to home
}
#endif
