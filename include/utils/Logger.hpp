#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <filesystem>

class Logger {
public:
    Logger(const std::string& filename);
    ~Logger();
    void log(const std::string& message);

private:
    void writer_thread_func();

    std::ofstream log_file;
    std::queue<std::string> log_queue;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::thread writer_thread;
    bool exit_flag;
};

#endif // LOGGER_HPP