#ifndef CONSOLE_COLOR_H
#define CONSOLE_COLOR_H

#ifdef _WIN32
#include <windows.h>
#else
#include <iostream>
#endif

class ConsoleColor {
public:
    static void clearConsole();
    static void readConsoleOutput(int color);
};

#endif // CONSOLE_COLOR_H
