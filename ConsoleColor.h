#ifndef CONSOLE_COLOR_H
#define CONSOLE_COLOR_H

#ifdef _WIN32
#include <windows.h>
#else
#include <iostream>
#endif

class ConsoleColor {
public:
    static void setColor(int color);
    static void resetColor();
    static void clearConsole();
};

#endif // CONSOLE_COLOR_H
