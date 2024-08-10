#ifndef CONSOLEWATCHER_H
#define CONSOLEWATCHER_H

#include <memory>
#include "Logger.h"
#include "MouseObserver.h"
#include "IObserver.h"
#include "ObservedString.h"
#include <sstream>

class ConsoleWatcher : public IObserver {
public:
    ConsoleWatcher(std::shared_ptr<Logger> logger, ObservedString& observedString);
    void run();
    void HandleEvent(const ObservedString& ref) override;
private:
    std::shared_ptr<Logger> logger_;
    ObservedString& observedString_;
    void printMessage();
    void waitForExit();
};

#endif // CONSOLEWATCHER_H
