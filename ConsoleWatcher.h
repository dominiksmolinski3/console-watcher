#ifndef CONSOLEWATCHER_H
#define CONSOLEWATCHER_H

#include <memory>
#include "Logger.h"
#include "MouseObserver.h"
#include "IObserver.h"
#include "ObservedString.h"
#include <sstream>

class TeeBuffer : public std::streambuf {
public:
    TeeBuffer(std::streambuf* sb1, std::streambuf* sb2)
        : sb1_(sb1), sb2_(sb2) {}

protected:
    virtual int overflow(int c) override {
        if (c == EOF) {
            return !EOF;
        } else {
            int const r1 = sb1_->sputc(c);
            int const r2 = sb2_->sputc(c);
            return r1 == EOF || r2 == EOF ? EOF : c;
        }
    }

    virtual int sync() override {
        int const r1 = sb1_->pubsync();
        int const r2 = sb2_->pubsync();
        return r1 == 0 && r2 == 0 ? 0 : -1;
    }

private:
    std::streambuf* sb1_;
    std::streambuf* sb2_;
};

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
    std::streambuf* oldCoutBuffer_;
    std::stringstream buffer_;
    std::unique_ptr<TeeBuffer> teeBuffer_; 
    bool isConsoleCleared_;
    bool isColorSet_;  
};

#endif // CONSOLEWATCHER_H
