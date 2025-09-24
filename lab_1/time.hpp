#include <chrono>

// struct run_time
// {
//     start_time;
//     start_end;
// };

class time_interval{
    private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
    std::string_view _message;
    
    public:
    explicit time_interval(std::string_view message = "Execution time: ")
        : _start(std::chrono::high_resolution_clock::now()),
          _message(message){}
    ~time_interval(){
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - _start);
        if (duration < std::chrono::nanoseconds(1)) {
        // < 1 ns
            std::cout << _message << "0ns\n";
        }
        else if (duration < std::chrono::microseconds(1)) {
            //ns
            auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
            std::cout << _message << ns.count() << "ns\n";
        }
        else if (duration < std::chrono::milliseconds(1)) {
            // mks
           auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
           std::cout << _message << us.count() << "μs\n";
        }
        else if (duration < std::chrono::seconds(1)) {
            // ms
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
            std::cout << _message << ms.count() << "ms\n";
        }
        else if (duration < std::chrono::minutes(1)) {
            // s
            auto s = std::chrono::duration_cast<std::chrono::seconds>(duration);
            std::cout << _message << s.count() << "s\n";
        }
        else {
            // m + 
            auto min = std::chrono::duration_cast<std::chrono::minutes>(duration);
            std::cout << _message << min.count() << "min\n";
        }
        std::cout<< "\n \n";
    }
};