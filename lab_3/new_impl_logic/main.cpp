// 10 вариант
#include <iostream>
#include <dlfcn.h>
#include "general_header.hpp"

typedef PARSING* (*PARSING_Constructor_t)(const char*, std::vector<std::string>, bool);
typedef void (*PARSING_Destructor_t)(PARSING*);

int main(){
    std::cout << "C++ standard: " << __cplusplus << "\n"<< std::endl;
    time_interval timer("FUNCTION MAIN: ");
    void* handle = dlopen("./lib_new_parsing.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error loading lib_parsing.so: " << dlerror() << std::endl;
        return 1;
    }
    PARSING_Constructor_t constructor = (PARSING_Constructor_t)dlsym(handle,
         "_ZN7PARSINGC1EPKcSt6vectorINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESaIS8_EEb");
    PARSING_Destructor_t destructor = (PARSING_Destructor_t)dlsym(handle, "_ZN7PARSINGD1Ev");
    
    if (!constructor || !destructor) {
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

    f_info *little_html{NULL}, *big_html{NULL}, *buffer_html{NULL};
    try {
        little_html = _create_file_in_dir("dir_files_html", vec_files_name[0]);
        big_html = _create_file_in_dir("dir_files_html", vec_files_name[1]);
        buffer_html = _create_file_in_dir("dir_files_html", vec_files_name[2]);
    }
    catch (EXEP_work_file &ex){
        std::cout << "_create_file_in_dir #_" << ex.what() << "#_"<< ex.getMessage() << "#_"<< ex.getDataState() << std::endl;
        return 1;  
    }
    try{
        write_buf_in_file(little_html, get_buffer_from_file(buffer_html), 1024*1024);
        write_buf_in_file(big_html, get_buffer_from_file(buffer_html), 1024 * 1024 * 200);
    }
    catch(EXEP_work_file &ex){
        std::cout << "Write_buf_in file #_" << ex.what() << "#_"<< ex.getMessage() << "#_"<< ex.getDataState() << std::endl;
        return 1;   
    }

    little_html->print_info();
    big_html->print_info();
    buffer_html->print_info();

    
    PARSING* parser_1 = constructor(little_html->_path.string().c_str(), vec_str_regex, false);
    PARSING* parser_2 = constructor(big_html->_path.string().c_str(), vec_str_regex, false);
    std::cout<<"create constuctor\n";

    destructor(parser_1);
    destructor(parser_2);
    
    dlclose(handle);
    return 0;
}