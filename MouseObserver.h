#ifndef MOUSEOBSERVER_H
#define MOUSEOBSERVER_H

#include <memory>
#include <atomic>
#include <thread>
#include "Logger.h"
#include "ObservedString.h"

class MouseObserver {
public:
    MouseObserver(std::shared_ptr<Logger> logger, ObservedString& observedString);
    ~MouseObserver();
    void startObserving();
private:
    std::shared_ptr<Logger> logger_;
    std::atomic<bool> running_;
    std::thread observerThread_;
    ObservedString& observedString_;
    void observe();
};

#endif // MOUSEOBSERVER_H
