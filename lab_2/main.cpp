// 10 вариант
#include <iostream>
#include <dlfcn.h>
#include "general_header.hpp"

typedef PARSING* (*create_parser_t)(f_info*, const char**, int);
typedef void (*destroy_parser_t)(PARSING*);
typedef void (*parser_search_t)(PARSING*);
typedef void (*parser_print_all_t)(PARSING*);

int main(){
    time_interval timer("FUNCTION MAIN: ");
    void* handle = dlopen("./lib_parsing.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error loading lib_parsing.so: " << dlerror() << std::endl;
        return 1;
    }
    std::cout << "C++ standard: " << __cplusplus << "\n"<< std::endl;

    create_parser_t create_parser = (create_parser_t)dlsym(handle, "create_parser");
    destroy_parser_t destroy_parser = (destroy_parser_t)dlsym(handle, "destroy_parser");
    parser_search_t parser_search = (parser_search_t)dlsym(handle, "parser_search");
    parser_print_all_t parser_print_all = (parser_print_all_t)dlsym(handle, "parser_print_all");

    if (!create_parser || !destroy_parser || !parser_search || !parser_print_all) {
        std::cerr << "Error loading symbols: " << dlerror() << std::endl;
        dlclose(handle);
        return 1;
    }
    
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

    try{
        write_buf_in_file(little_html, get_buffer_from_file(buffer_html), 1024*1024);
    }
    catch(EXEP_work_file &ex){
        std::cout << "Write_buf_in file #_" << ex.what() << "#_"<< ex.getMessage() << "#_"<< ex.getDataState() << std::endl;
        return 1;   
    }

    try{
        write_buf_in_file(big_html, get_buffer_from_file(buffer_html), 1024 * 1024 * 200);
    }
    catch (EXEP_work_file &ex) {
        std::cout << "Write_buf_in file #_" << ex.what() << "#_"<< ex.getMessage() << "#_"<< ex.getDataState() << std::endl;
        return 1;
    }
    

    // PARSING little_file(little_html, vec_str_regex);
    // PARSING big_file(big_html, vec_str_regex);
    // write_in_big_file(file_path_in_little, file_path_in_big);
    const char* patterns[] = {
        "<h[1-6][^>]*>.*?</h[1-6]>",
        "<p[^>]*>.*?</p>", 
        "<b[^>]*>.*?</b>"
    };
    int patterns_count = 3;

    PARSING* parser = create_parser(little_html, patterns, patterns_count);
    if (parser) {
        parser_search(parser);      
        //parser_print_all(parser);   
        destroy_parser(parser);
    }

    PARSING* parser_1 = create_parser(big_html, patterns, patterns_count);
    if (parser_1){
        parser_search(parser_1);
        destroy_parser(parser_1);
    }
    return 0;
    
    dlclose(handle);
}