#ifndef WORK_FILE_HPP
#define WORK_FILE_HPP

#include <exception>
#include <vector>
#include <list>
#include <regex>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <cstring>

class EXEP_work_file : public std::exception {
    private:
    int data_state;
    std::string message;
    public:
    EXEP_work_file(std::string _message, int _data_state);
    int getDataState();
    std::string getMessage();
};

uint64_t lines_count(std::filesystem::path _path, uint64_t _size);

struct f_info
{
    std::filesystem::path _path;
    uint64_t _size_bytes;
    
    f_info (std::filesystem::path _PATH, uint64_t _SB);
    void update_info() noexcept;
    void print_info() noexcept;
};

std::string get_buffer_from_file(f_info * _info);

void write_buf_in_file(f_info * _info,
                       std::string_view buffer,
                       size_t target_size);

uint64_t lines_count(std::filesystem::path _path,
                     uint64_t _size);

f_info*_create_file_in_dir(std::filesystem::path _dir,
                           std::string_view _fname);

class time_interval{
    private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
    std::string_view _message;
    
    public:
    time_interval(std::string_view message);
    ~time_interval();
};

std::string set_string_path(std::string_view msg);


class PARSING{
    private:
    size_t count_pattern = 0;
    char** pattern_buffer;
    std::string _fpath;
    std::regex _combined_regex;
    std::vector <std::list<std::string>> pattern_lists;
    std::vector <std::string> _pattern_names;

    public:

    void search_and_collect();
    void printAll();
    void printStats();

    PARSING(const char* fpath,
            std::vector<std::string> _patterns,
            bool print_all_occurrences);
    ~PARSING();
};

#endif