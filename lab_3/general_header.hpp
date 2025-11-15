#ifndef WORK_FILE_HPP
#define WORK_FILE_HPP

#include <exception>
#include <vector>
#include <list>
#include <regex>
#include <fstream>
#include <chrono>
#include <filesystem>

class EXEP_work_file : public std::exception {
    private:
    int data_state;
    std::string message;
    public:
    EXEP_work_file(std::string _message, int _data_state);
    int getDataState();
    std::string getMessage();
};

//uint64_t lines_count(std::filesystem::path _path, uint64_t _size);

struct f_info
{   
    private:
    int flag_write_info;
    std::filesystem::path path_log_file;
    void write_log_info_in_file();
    public:
    std::string log_inforamation;
    std::filesystem::path _path;
    uint64_t _size_bytes;
    
    f_info (std::filesystem::path _PATH, uint64_t _SB, int flag_log_file) noexcept;
    void update_info() noexcept;
    //void print_info() noexcept;
    std::string get_str_log_info();
    void write_info(std::string &msg) noexcept;
};

std::string get_buffer_from_file(f_info * _info, int flag_saves_info);

void write_buf_in_file(f_info * _info,
                       std::string_view buffer,
                       size_t target_size,
                       int flag_saves_info);

// uint64_t lines_count(std::filesystem::path _path,
//                      uint64_t _size);

f_info*_create_file_in_dir(std::filesystem::path _dir,
                           std::string_view _fname,
                           int flag_saves_info);

class time_interval{
protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
    std::string _message;
    virtual void output_result(const std::string& value_time) noexcept;
    std::string calculating_diff_time() noexcept;
public:
    time_interval(std::string message = "Execution time: ") noexcept;
    virtual ~time_interval() noexcept;
};

class time_interval_log_file : public time_interval{
private:
    std::filesystem::path _path_log_file;
    virtual void output_result(const std::string& value_time) noexcept override;
public:
    time_interval_log_file(std::string message, std::filesystem::path path_log_file) noexcept;
};

std::string set_string_path(std::string_view msg);


class PARSING{
    private:
    f_info* INFO;
    std::regex _combined_regex;
    std::vector<std::list<std::string>> pattern_lists;
    std::vector <std::string> _pattern_names;

    public:
    void search_and_collect();
    
    void printAll();

    void printStats();
    PARSING(f_info * _INFO,
            std::vector<std::string> _patterns,
            bool flag_print_all_info);
    ~PARSING (){
        
    }   
};

#endif