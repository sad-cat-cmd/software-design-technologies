// 10 вариант
#include <iostream>
#include <dlfcn.h>
#include "general_header.hpp"

typedef PARSING* (*create_parser_t)(f_info*, const char**, int, bool);
typedef void (*destroy_parser_t)(PARSING*);

const char* patterns[] = {
        "<h[1-6][^>]*>.*?</h[1-6]>",
        "<p[^>]*>.*?</p>", 
        "<b[^>]*>.*?</b>"
};
int patterns_count = 3;
bool print_all_flag = false;

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

    if (!create_parser || !destroy_parser) {
        std::cerr << "Error loading symbols: " << dlerror() << std::endl;
        dlclose(handle);
        return 1;
    }

    std::vector<std::string> vec_files_name;
    vec_files_name.reserve(3);
    vec_files_name.emplace_back(set_string_path("LITTLE.HTML"));
    vec_files_name.emplace_back(set_string_path("BIG_HTML"));
    vec_files_name.emplace_back(set_string_path("BUFFER_HTML"));
    
    f_info *little_html{NULL}, *big_html{NULL}, *buffer_html{NULL};
    try {
        little_html = _create_file_in_dir("dir_files_html", vec_files_name[0]);
        big_html = _create_file_in_dir("dir_files_html", vec_files_name[1]);
        buffer_html = _create_file_in_dir("dir_files_html", vec_files_name[2]);
    }
    catch(EXEP_work_file &ex){
        std::cout << "_create_file_in_dir #_" << ex.what() << "#_"<< ex.getMessage() << "#_"<< ex.getDataState() << std::endl;
        return 1;   
    }
    try{
        write_buf_in_file(little_html, get_buffer_from_file(buffer_html), 1024*1024);
        write_buf_in_file(big_html, get_buffer_from_file(buffer_html), 1024*1024*1024);
    }
    catch(EXEP_work_file &ex){
        std::cout << "Write_buf_in file #_" << ex.what() << "#_"<< ex.getMessage() << "#_"<< ex.getDataState() << std::endl;
        return 1;   
    }

    PARSING* parser = create_parser(little_html, patterns, patterns_count, print_all_flag);
    if (!parser){
        return 1;
    }
    destroy_parser(parser);
    PARSING* parser_1 = create_parser(big_html, patterns, patterns_count, print_all_flag);
    if (!parser_1){
        return 1;
    }
    destroy_parser(parser_1);
    
    dlclose(handle);
}