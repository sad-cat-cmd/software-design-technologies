#ifndef WORK_WITH_FILE_HPP
#define WORK_WITH_FILE_HPP

#include <fstream>
#include <filesystem>
#include <vector>

uint64_t lines_count(std::filesystem::path _path, uint64_t _size);

struct f_info
{
    std::filesystem::path _path;
    uint64_t _size_bytes;
    
    f_info (std::filesystem::path _PATH, uint64_t _SB);
    void update_info();
    void print_info();
};

std::string get_buffer_from_file(f_info * _info);

void write_buf_in_file(f_info * _info,
                       std::string_view buffer,
                       size_t target_size);

uint64_t lines_count(std::filesystem::path _path,
                     uint64_t _size);

f_info*_create_file_in_dir(std::filesystem::path _dir,
                           std::string_view _fname);

#endif
