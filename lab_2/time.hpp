#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>

class time_interval{
    private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
    std::string_view _message;
    
    public:
    time_interval(std::string_view message);
    ~time_interval();
};

#endif