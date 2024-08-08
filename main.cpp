#include <iostream>
#include "ConsoleWatcher.h"
#include "Logger.h"
#include "ObservedString.h"
#include "MouseObserver.h"

int main() {
    try {
        auto logger = std::make_shared<Logger>("log.txt");
        ObservedString observedString;
        auto mouseObserver = std::make_shared<MouseObserver>(logger, observedString);
        auto consoleWatcher = std::make_shared<ConsoleWatcher>(logger, observedString);
        mouseObserver->startObserving();
        consoleWatcher->run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
