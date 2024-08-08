#include "MouseObserver.h"
#include <windows.h>
#include "ConsoleColor.h"
#include <chrono>
#include <thread>
#include <iostream>

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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
