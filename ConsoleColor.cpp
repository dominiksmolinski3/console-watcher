#include "ConsoleColor.h"
#include <windows.h>
#include <iostream>

namespace ConsoleColor {
    void setColor(int color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole != INVALID_HANDLE_VALUE) {
            SetConsoleTextAttribute(hConsole, color);
        } else {
            std::cerr << "Failed to get console handle" << std::endl;
        }
    }

    void clearConsole() {
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
}
