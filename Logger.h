#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <memory>
#include <mutex>

class Logger {
public:
    Logger(const std::string& filename);
    ~Logger();
    void log(const std::string& message);
private:
    std::ofstream logFile_;
    std::mutex logMutex_;
};

#endif // LOGGER_H
