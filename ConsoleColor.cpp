#include "ConsoleColor.h"
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>

void ConsoleColor::readConsoleOutput(int color) {
    // handle to the console output buffer
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        std::cerr << "Error getting console handle" << std::endl;
        return;
    }

    // size of the buffer to read
    const int width = 80;
    const int height = 20;
    COORD bufferSize = {width, height};

    // rectangle to read from (top-left corner)
    SMALL_RECT readRegion = {0, 0, width - 1, height - 1};

    // buffer to hold the characters and attributes
    std::vector<CHAR_INFO> buffer(width * height);

    // buffer coordinates
    COORD bufferCoord = {0, 0};

    // console output into the buffer
    BOOL success = ReadConsoleOutput(
        hConsole,
        buffer.data(),
        bufferSize,
        bufferCoord,
        &readRegion
    );

    if (!success) {
        std::cerr << "Error reading console output" << std::endl;
        return;
    }
    ConsoleColor::clearConsole();
    SetConsoleTextAttribute(hConsole, color);
    // display contents of the buffer
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            CHAR_INFO charInfo = buffer[x + y * width];
            std::wcout << charInfo.Char.UnicodeChar;
        }
        std::wcout << std::endl;
    }
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

void ConsoleColor::clearConsole() {
    std::cout << "\033[2J\033[H";  // Clear screen and move cursor to home
}
#endif
