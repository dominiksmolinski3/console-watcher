#include "MouseObserver.h"
#include "ConsoleColor.h"
#include <chrono>
#include <thread>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <X11/Xlib.h> // Example for Unix-like systems
#include <unistd.h>
#endif

MouseObserver::MouseObserver(std::shared_ptr<Logger> logger, ObservedString& observedString)
    : logger_(logger), running_(false), observedString_(observedString) {}

MouseObserver::~MouseObserver() {
    if (running_) {
        running_ = false;
        if (observerThread_.joinable()) {
            observerThread_.join();
        }
    }
}

void MouseObserver::startObserving() {
    running_ = true;
    observerThread_ = std::thread(&MouseObserver::observe, this);
}

void MouseObserver::observe() {
    bool inside = false;
    while (running_) {
#ifdef _WIN32
        POINT pt;
        GetCursorPos(&pt);
        HWND hwnd = WindowFromPoint(pt);
        if (hwnd == GetConsoleWindow()) {
            if (!inside) {
                observedString_.SetValue("Enter");
                inside = true;
            }
        } else {
            if (inside) {
                observedString_.SetValue("Leave");
                inside = false;
            }
        }
#else
        Display* display = XOpenDisplay(nullptr);
        if (!display) {
            std::cerr << "Failed to open X display" << std::endl;
            return;
        }
        Window root = DefaultRootWindow(display);
        Window root_return, child_return;
        int root_x, root_y, win_x, win_y;
        unsigned int mask_return;
        XQueryPointer(display, root, &root_return, &child_return, &root_x, &root_y, &win_x, &win_y, &mask_return);
        
        // Check if the mouse is within the console window (you would need to get the window ID of your console)
        // Here we're just checking if it's within the screen bounds
        if (root_x >= 0 && root_y >= 0) {
            if (!inside) {
                observedString_.SetValue("Enter");
                inside = true;
            }
        } else {
            if (inside) {
                observedString_.SetValue("Leave");
                inside = false;
            }
        }
        XCloseDisplay(display);
#endif
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
