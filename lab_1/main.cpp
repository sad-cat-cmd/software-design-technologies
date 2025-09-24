// 10 вариант
#include <iostream>
#include "parsing.hpp"

// f_info *test_html = _create_file_in_dir("dir_files_html", "test.hmtl");
    // test_html->update_info();
    // test_html->print_info();

int main(){
    time_interval timer("FUNCTION MAIN: ");
    std::cout << "C++ standard: " << __cplusplus << "\n"<< std::endl;

    std::vector<std::string> vec_str_regex;
    vec_str_regex.reserve(3);
    vec_str_regex.emplace_back("<h[1-6][^>]*>.*?</h[1-6]>");
    vec_str_regex.emplace_back("<p[^>]*>.*?</p>");
    vec_str_regex.emplace_back("<b[^>]*>.*?</b>");

    std::vector<std::string> vec_files_name;
    vec_files_name.reserve(3);
    vec_files_name.emplace_back(set_string_path("LITTLE.HTML"));
    vec_files_name.emplace_back(set_string_path("BIG_HTML"));
    vec_files_name.emplace_back(set_string_path("BUFFER_HTML"));

    f_info *little_html = _create_file_in_dir("dir_files_html", vec_files_name[0]);
    f_info *big_html = _create_file_in_dir("dir_files_html", vec_files_name[1]);
    f_info *buffer_html = _create_file_in_dir("dir_files_html", vec_files_name[2]);

    write_buf_in_file(little_html, get_buffer_from_file(buffer_html), 1024*1024); // 1mb
    write_buf_in_file(big_html, get_buffer_from_file(buffer_html), 1024 * 1024 * 1024); // 17гб
    
    PARSING little_file(little_html, vec_str_regex);
    //PARSING big_file(big_html, vec_str_regex);
    // write_in_big_file(file_path_in_little, file_path_in_big);

    return 0;
}