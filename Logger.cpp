#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>

Logger::Logger(const std::string& filename) {
    logFile_.open(filename, std::ios::out | std::ios::app);
    if (!logFile_.is_open()) {
        throw std::runtime_error("Unable to open log file.");
    }
}

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> guard(logMutex_);
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    logFile_ << std::ctime(&now) << " - " << message << std::endl;
}
