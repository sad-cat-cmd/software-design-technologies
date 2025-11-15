#include "general_header.hpp"
#include <dlfcn.h>
#include <iostream>
#include <limits>

typedef PARSING* (*create_parser_t)(f_info*, const char**, int, bool);
typedef void (*destroy_parser_t)(PARSING*);
create_parser_t create_parser;
destroy_parser_t destroy_parser;

void* open_lib_init_names();
void write_file_names(std::vector<std::string> &vec_str_name);
void verifying_correctness_names(std::vector<std::string> &vec_str_name) noexcept;
void init_vec_f_info(std::vector<f_info*> &vec_f_info);
void multy_buf_in_file();
void complex_init_global_value();

std::string based_dir_name = "dir_files_html";
std::string buffer_name = "test.html";
void* handle {NULL};
std::vector<std::string> vec_str_regex;
std::vector<std::string> vec_str_name;
std::vector<f_info*> vec_f_info;
size_t count_files = 5;
const char* patterns[] = {
        "<h[1-6][^>]*>.*?</h[1-6]>",
        "<p[^>]*>.*?</p>", 
        "<b[^>]*>.*?</b>"
};
int patterns_count = 3;
bool print_all_flag = false;
int main() {
    time_interval timer("FUNCTION MAIN: ");
        std::cout << "C++ standart: " << __cplusplus << "\n"<< std::endl;
    try {complex_init_global_value();}
    catch(EXEP_work_file &ex){
        std::cout <<"# "<< ex.getMessage() << "# "<< ex.getDataState() << "# "<< ex.what() << std::endl;
        return 1;   
    }
    time_interval timer("Multi_Parsing: ");
    for (size_t i = 0; i < count_files; i++)
    {
        PARSING* parser = create_parser(little_html, patterns, patterns_count, print_all_flag);
        if (!parser){
            return 1;
        }
        destroy_parser(parser);
    }
    return 0;
}

void* open_lib_init_names(){
    void* handle = dlopen("./lib_parsing.so", RTLD_LAZY);
    if (!handle)
        throw EXEP_work_file("open_lib_init_names() # error loading lib_parsing.so ", 0);

    create_parser_t create_parser = (create_parser_t)dlsym(handle, "create_parser");
    destroy_parser_t destroy_parser = (destroy_parser_t)dlsym(handle, "destroy_parser");

    if (!create_parser || !destroy_parser) {
        dlclose(handle);
        throw EXEP_work_file("open_lib_init_names() # error loading symbols ", 0); 
    }
    return handle;
}
void write_file_names(std::vector<std::string> &vec_str_name){
    bool flag_hand_write = true;
    std::string msg_entry;
    std::cout<< "Manual file entry:(1 -- yes) (0 -- no)\n"<<std::endl;
    std::cin >> msg_entry;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (msg_entry == "0")
        flag_hand_write = false;
    if (msg_entry == "1")
        flag_hand_write = true;

    vec_str_name.resize(count_files);
    if (flag_hand_write){
        for (size_t i = 0; i < count_files; i++){
            try {vec_str_name[i] = set_string_path("File " + std::to_string(i + 1));}
            catch(EXEP_work_file &ex) {throw EXEP_work_file("write_file_names() #" + ex.getMessage(), ex.getDataState());}
        }
    }
    else {
        for(size_t i = 0; i < count_files; i++){
            vec_str_name[i] = "";
        }
    }
}
void verifying_correctness_names(std::vector<std::string> &vec_str_name) noexcept {
    for (size_t i = 0; i < count_files; i++){
        if (vec_str_name[i].length() < 5 || !vec_str_name[i].ends_with(".html")){
            vec_str_name[i] = "File_" + std::to_string(i+1) + ".html";
        }
    }
}
void init_vec_f_info(std::vector<f_info*> &vec_f_info){
    vec_f_info.resize(count_files+1, NULL);
    try {vec_f_info[count_files] = _create_file_in_dir(based_dir_name, buffer_name);}
    catch(EXEP_work_file &ex) {throw EXEP_work_file("init_vec_f_info #" + ex.getMessage(), ex.getDataState());}
    for (size_t i = 0; i < count_files; i++){
        try {vec_f_info[i] = _create_file_in_dir(based_dir_name, vec_str_name[i]);}
        catch(EXEP_work_file &ex) {throw EXEP_work_file("init_vec_f_info #" + ex.getMessage(), ex.getDataState());}
    }
}
void multy_buf_in_file(){
    std::string buffer;
    try {buffer = get_buffer_from_file(vec_f_info[count_files]);}
    catch(EXEP_work_file &ex) {throw EXEP_work_file("multy_buf_in_file() # " + ex.getMessage(), ex.getDataState());}
    for (size_t i = 0; i< count_files; i++){
        try{write_buf_in_file(vec_f_info[i],buffer, 10*1024*1024);}
        catch(EXEP_work_file &ex) {throw EXEP_work_file("multy_buf_in_file() # " + ex.getMessage(), ex.getDataState());}
    }
}
void complex_init_global_value(){
    try {
        handle = open_lib_init_names();
        write_file_names(vec_str_name);
        verifying_correctness_names(vec_str_name);
        init_vec_f_info(vec_f_info);
        multy_buf_in_file();
    }
    catch (EXEP_work_file &ex) {throw EXEP_work_file("complex_init_global_value() # " + ex.getMessage(), 0);}
}