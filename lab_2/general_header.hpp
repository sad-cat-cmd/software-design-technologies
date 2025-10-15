#ifndef WORK_FILE_HPP
#define WORK_FILE_HPP

#include <vector>
#include <list>
#include <regex>
#include <fstream>
#include <chrono>
#include <filesystem>

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
    f_info* INFO;
    std::regex _combined_regex;
    std::vector<std::list<std::string>> pattern_lists;
    std::vector <std::string> _pattern_names;
/**
 * @brief функция вывода контейнера std::list<std::string> на экран
 * @param _list - копия объекта типа std::list<std::string>
 * @param msg - сообщение до ввода в поток вывода
 */

    public:
    void search_and_collect();
    
    void printAll();

    void printStats();
    PARSING(f_info * _INFO,
            std::vector<std::string> _patterns);
    ~PARSING (){
        
    }   
};

#endif