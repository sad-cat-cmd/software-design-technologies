#include "general_header.hpp"
#include <dlfcn.h>
#include <iostream>
#include <limits>
#include <thread>

typedef PARSING* (*create_parser_t)(f_info*, const char**, int, bool, bool);
typedef void (*destroy_parser_t)(PARSING*);
create_parser_t create_parser;
destroy_parser_t destroy_parser;

void* open_lib_init_names();
void write_file_names(std::vector<std::string> &vec_str_name);
void verifying_correctness_names(std::vector<std::string> &vec_str_name) noexcept;
void init_vec_f_info(std::vector<f_info*> &vec_f_info);
void multy_buf_in_file();
void complex_init_global_value();

void* handle {NULL};
size_t count_files = 5;
std::vector<long long> vec_target_size {1024*1024, 1024*1024 * 50,  1024*1024 *10, 1024*1024* 11,  1024*1024* 20};
std::vector<std::thread> vec_thread (count_files);
std::string based_dir_name = "dir_files_html";
std::string buffer_name = "test.html";
std::vector<std::string> vec_str_name;
std::vector<f_info*> vec_f_info;
std::string buffer_str;
const char* patterns[] = {
        "<h[1-6][^>]*>.*?</h[1-6]>",
        "<p[^>]*>.*?</p>", 
        "<b[^>]*>.*?</b>"
};
int patterns_count = 3;
bool print_all_flag = false;
bool write_log_file_flag = true;

void work_file_thread(std::string buffer, size_t index_thread){
    std::cout << "=== Thread " << index_thread << " START ===" << std::endl;
    std::cout << "Creating file: " << vec_str_name[index_thread] << std::endl;
    
    try {
        vec_f_info[index_thread] = _create_file_in_dir(based_dir_name, vec_str_name[index_thread], 2);
        std::cout << "Thread " << index_thread << ": File created successfully" << std::endl;
    }
    catch(EXEP_work_file &ex) {
        std::cout << "Thread " << index_thread << ": ERROR creating file - " << ex.getMessage() << std::endl;
        throw EXEP_work_file("init_vec_f_info #" + ex.getMessage(), ex.getDataState());
    }
    
    std::cout << "Thread " << index_thread << ": Writing data, size: " << vec_target_size[index_thread % count_files] << std::endl;
    
    try {
        write_buf_in_file(vec_f_info[index_thread], buffer, vec_target_size[index_thread % count_files], 2);
        std::cout << "Thread " << index_thread << ": Write completed" << std::endl;
    }
    catch(EXEP_work_file &ex) {
        std::cout << "Thread " << index_thread << ": ERROR writing - " << ex.getMessage() << std::endl;
        throw EXEP_work_file("multy_buf_in_file() # " + ex.getMessage(), ex.getDataState());
    }
    
    std::cout << "=== Thread " << index_thread << " END ===" << std::endl;
}
void init_thread(){
    time_interval timer("time work files: ");
    vec_f_info.resize(count_files+1, NULL);
    try {vec_f_info[count_files] = _create_file_in_dir(based_dir_name, buffer_name, 2);}
    catch(EXEP_work_file &ex) {throw EXEP_work_file("init_vec_f_info #" + ex.getMessage(), ex.getDataState());}
    try {buffer_str = get_buffer_from_file(vec_f_info[count_files], 2);}
    catch(EXEP_work_file &ex) {throw EXEP_work_file("multy_buf_in_file() # " + ex.getMessage(), ex.getDataState());}
    for (size_t i = 0; i < count_files; i++){
        vec_thread[i] = std::thread{work_file_thread, buffer_str, i};
    }
    for (size_t i = 0; i < count_files; i++) {
    if (vec_thread[i].joinable()) {
        vec_thread[i].join();
    }
}
}
int main() {
    std::cout << "C++ standart: " << __cplusplus << "\n"<< std::endl;
    time_interval timer("FUNCTION MAIN: ");
    try {complex_init_global_value();}
    catch(EXEP_work_file &ex){
        std::cout <<"# "<< ex.getMessage() << "# "<< ex.getDataState() << "# "<< ex.what() << std::endl;
        return 1;   
    }
    time_interval timer_parsing("Multi_Parsing: ");
    for (size_t i = 0; i < count_files; i++)
    {
        PARSING* parser = create_parser(vec_f_info[i], patterns, patterns_count, print_all_flag, write_log_file_flag);
        if (!parser)
            return 1;
        destroy_parser(parser);
    }
    return 0;
}

void* open_lib_init_names(){
    void* handle = dlopen("./lib_parsing.so", RTLD_LAZY);
    if (!handle)
        throw EXEP_work_file("open_lib_init_names() # error loading lib_parsing.so ", 0);

    create_parser = (create_parser_t)dlsym(handle, "create_parser");
    destroy_parser = (destroy_parser_t)dlsym(handle, "destroy_parser");

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


// void init_vec_f_info(std::vector<f_info*> &vec_f_info){
//     vec_f_info.resize(count_files+1, NULL);
//     try {vec_f_info[count_files] = _create_file_in_dir(based_dir_name, buffer_name, 2);}
//     catch(EXEP_work_file &ex) {throw EXEP_work_file("init_vec_f_info #" + ex.getMessage(), ex.getDataState());}
//     for (size_t i = 0; i < count_files; i++){
//         try {vec_f_info[i] = _create_file_in_dir(based_dir_name, vec_str_name[i], 2);}
//         catch(EXEP_work_file &ex) {throw EXEP_work_file("init_vec_f_info #" + ex.getMessage(), ex.getDataState());}
//     }
// }
// void multy_buf_in_file(){
//     time_interval time_writing("TIME MULTY BUFFER WRITING: ");
//     std::string buffer;
//     try {buffer = get_buffer_from_file(vec_f_info[count_files], 2);}
//     catch(EXEP_work_file &ex) {throw EXEP_work_file("multy_buf_in_file() # " + ex.getMessage(), ex.getDataState());}
//     for (size_t i = 0; i< count_files; i++){
//         try{write_buf_in_file(vec_f_info[i],buffer, 10*1024*1024, 2);}
//         catch(EXEP_work_file &ex) {throw EXEP_work_file("multy_buf_in_file() # " + ex.getMessage(), ex.getDataState());}
//     }
// }
void complex_init_global_value(){
    try {
        handle = open_lib_init_names();
        write_file_names(vec_str_name);
        verifying_correctness_names(vec_str_name);
        std::cout << "verifying_correctness_names\n";
        // init_vec_f_info(vec_f_info);
        // multy_buf_in_file();
        init_thread();
    }
    catch (EXEP_work_file &ex) {throw EXEP_work_file("complex_init_global_value() # " + ex.getMessage(), 0);}
}