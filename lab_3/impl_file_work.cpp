#include "general_header.hpp"
#include <iostream>

EXEP_work_file::EXEP_work_file(std::string _message, int _data_state){
    data_state = _data_state;
    message = _message;
}
int EXEP_work_file::getDataState() {return data_state;}
std::string EXEP_work_file::getMessage() {return message;}

void f_info::update_info() noexcept {
    //std::cout <<"____ UPDATES F_INFO :/" << _path << "____\n";
    //time_interval timer_f("f_info Updates: ");
    if (_size_bytes == std::filesystem::file_size(_path)) {
        // std::cout<<"___________________________\n";
        // print_info();
        return;
    }
    this->_size_bytes = std::filesystem::file_size(_path);
    // std::cout<<"___________________________\n";
    // print_info();
    return; 
}
std::string f_info::get_str_log_info(){
    return "INFORMATION " + _path.string() + "\n" + "Size file (bytes):" + std::to_string(_size_bytes) + "\n" + log_inforamation + "\n";
}
// void f_info::print_info() noexcept {
//     if (flag_write_info ==) {
//     }
//     std::cout << "___ PRINT_INFO "<< this->_path<< " ____\n";
//     std::cout << "Size file (bytes): " << this->_size_bytes <<"\n";
//     std::cout << log_inforamation << "\n";
//     std::cout<<"___________________________\n";
//     return;
// }
void f_info::write_info(std::string &msg) noexcept{
    if (flag_write_info == 0)
        return;
    log_inforamation += msg;
    if (flag_write_info == 2)
        write_log_info_in_file();
        log_inforamation = "";
    if (flag_write_info == 1)
        return;
}
void f_info::write_log_info_in_file(){
    if (std::filesystem::exists(path_log_file)) {
        std::ofstream _file (path_log_file, std::ios::app);
        if (_file.is_open()){
            _file << log_inforamation << std::endl;
        }
        _file.close();
    }
    return;
}

std::string get_buffer_from_file(f_info * _info, int flag_saves_info){
    std::string str_log_information;
    if (_info == NULL )
        throw EXEP_work_file("get_buffer_from_file() # file_ptr is NULL", 0);
    
    time_interval timer("get_buffer_from_file: ");
    std::ifstream file(_info->_path, std::ios::binary);

    if (!file.is_open())
        throw EXEP_work_file("get_buffer_from_file() # file is not open:" + _info->_path.string(), 1);
    std::string buffer;
    file.seekg(0, std::ios::end);
    buffer.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(buffer.data(), buffer.size());
    file.close();
    if (flag_saves_info)
        str_log_information += 
            "\n___________________________\n"
            "GET BUFFER FROM FILE: " + 
            _info->_path.string() + 
            "\n SIZE: " + 
            std::to_string(buffer.size()) + 
            "\n___________________________\n";
            _info->write_info(str_log_information);
    return buffer;
}

void write_buf_in_file(f_info * _info, std::string_view buffer, size_t target_size, int flag_saves_info){
    std::string str_log_information;
    if (_info == NULL) {
        throw EXEP_work_file("write_buf_in_file() # file_ptr is NULL", 0);
    }
    if (flag_saves_info)
        str_log_information += "___ WRITE IN FILE: " + _info->_path.string() +" ____\n";
    //std::cout<< "___ WRITE IN FILE: /"<< _info->_path<< " ____\n";
    time_interval timer("write buffer in file: ");
    if (buffer.empty()){
        throw EXEP_work_file("buffer is empty", 0);
    }
    size_t current_size = std::filesystem::file_size(_info->_path);
    if (current_size == target_size) {
        if (flag_saves_info)
            str_log_information += " file already has target size: " + " --- " +std::to_string(target_size) + " bytes\n";
        //std::cout << "write_buf_in_file() # file already has target size: " << target_size << " bytes\n";
        _info->update_info();
        _info->write_info(str_log_information);
        return;
    }
    std::ofstream file(_info->_path, std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        throw EXEP_work_file("write_buf_in_file() # file is not open:" + _info->_path.string(), 0);
    }
    size_t buffer_size = buffer.size();
    size_t total_written = 0;
    size_t needed_size = (current_size < target_size) ? target_size - current_size : 0;
    if (current_size > target_size) {
        file.close();
        std::filesystem::resize_file(_info->_path, target_size);
        if (flag_saves_info) 
            str_log_information += "File truncated to: " + std::to_string(target_size) + " bytes\n";
        //std::cout << "File truncated to: " << target_size << " bytes\n";
        _info->update_info();
        _info->write_info(str_log_information);
        
        return;
    }
    while(total_written < needed_size){
        size_t remaining = needed_size - total_written;
        size_t to_write = std::min(remaining, buffer_size);

        file.write(buffer.data(), to_write);
        total_written += to_write;

        if (!file) {
            file.close();
            throw EXEP_work_file("write_buf_in_file() # error write in file", 0);
        }
    }
    str_log_information += "Recorded: " + std::to_string(total_written) + " BYTES\n" + "Recording is over. UPDATES INFO\n";
    // std::cout << "Recorded: " << total_written << " BYTES\n";
    // std::cout << "Recording is over. UPDATES INFO\n";
    file.close();
    _info->update_info();
    _info->write_info(str_log_information);
    //std::cout<<"___________________________\n";
    return;
}

f_info::f_info (std::filesystem::path _PATH, uint64_t _SB, int flag_log_file) noexcept {
    size_t pos = _PATH.string().find_last_of('/');
    if (pos != std::string::npos) {
        path_log_file = _PATH.string().substr(0, pos);
        path_log_file = path_log_file / "logs";
        std::cout<<path_log_file.string() << "\n";
    }

    if (!std::filesystem::exists(path_log_file)){
        std::filesystem::create_directory(path_log_file);
        std::cout << "create_directory: " << path_log_file.string() << "\n"; 
    }
    if (pos != std::string::npos){
        std::filesystem::path file_name = _PATH.string().substr(pos+1);
        size_t pos_2 = file_name.string().find_last_of(".");
        path_log_file = path_log_file.string() + "/log_" + file_name.string().substr(0 ,pos_2) +".txt";
    }
    std::cout << path_log_file.string() << "\n";
    _path = _PATH;
    _size_bytes = _SB;
    flag_write_info = flag_log_file;
    if (flag_write_info == 2) {
        std::ofstream file(path_log_file);
        if (file.is_open()){
            file << get_str_log_info() << std::endl;
            file.close();
            std::cout<< path_log_file.string() << " create\n";
        } 
    }
}
f_info*_create_file_in_dir (std::filesystem::path _dir,std::string_view _fname, int flag_saves_info) {
    if (flag_saves_info != 1 && flag_saves_info != 2 && flag_saves_info != 0)
        throw EXEP_work_file("_create_file_in_dir() # invalid flag_saves_info: " + std::to_string(flag_saves_info) + " for " + (_dir/_fname).string(), 1);
    //time_interval _timer_fcreate ("\t FILE and DIR CREATING : " + _dir.string() + "/" + _fname);
    std::string str_log_information;
    if (!std::filesystem::exists(_dir)){
        std::filesystem::create_directory(_dir);
        if (flag_saves_info)
            str_log_information += _dir.string() + " has been created"+ "\n";
    }
    else {
        if (flag_saves_info)
            str_log_information += _dir.string() + " has already been created" + "\n";
    }
    std::filesystem::path _fpath = _dir / _fname;

    if (!std::filesystem::exists(_fpath)){
        std::ofstream _file (_fpath);
        if (!_file.is_open()) 
            throw EXEP_work_file("_create_file_in_dir() # file is not open: " + _fpath.string(), 1);
        else {
            if (flag_saves_info) 
                str_log_information += _fpath.string() + " has been created"+"\n";
            _file.close();
            f_info * _info = new f_info(_fpath, 0, flag_saves_info);
            _info->write_info(str_log_information);
            return _info;
        }
    }
    else {
        if (flag_saves_info) 
            str_log_information += _fpath.string() + " has been created"+"\n";
        std::cout<< "The file has already been created: " << _fpath << " INFO UPDATES" << "\n";
        f_info* _info = new f_info(_fpath,std::filesystem::file_size(_fpath), flag_saves_info);
        _info->write_info(str_log_information);
        //_info->print_info();
        return _info;
    }
}
std::string time_interval::calculating_diff_time() noexcept{
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - _start);
    if (duration < std::chrono::nanoseconds(1)) {
        return "0ns\n";
    }
    else if (duration < std::chrono::microseconds(1)) {
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
        return std::to_string(ns.count()) + " ns\n";
    }
    else if (duration < std::chrono::milliseconds(1)) {
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
        return std::to_string(us.count()) + " mks\n";
    }
    else if (duration < std::chrono::seconds(1)) {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        return std::to_string(ms.count()) + " ms\n";
    }
    else if (duration < std::chrono::minutes(1)) {
        auto s = std::chrono::duration_cast<std::chrono::seconds>(duration);
        return std::to_string(s.count()) + " s\n";
    }
    else {
        auto min = std::chrono::duration_cast<std::chrono::minutes>(duration);
        return std::to_string(min.count()) + " min\n";
    }
}
void time_interval::output_result(const std::string& value_time) noexcept{
    std::cout <<"\n"<< _message << " "<< value_time << "\n\n";
}

time_interval::time_interval(std::string message) noexcept {
        _start = std::chrono::high_resolution_clock::now();
        _message = message;
}
time_interval::~time_interval() noexcept {
    output_result(calculating_diff_time());
}

void time_interval_log_file::output_result(const std::string& value_time) noexcept{
    if (std::filesystem::exists(_path_log_file)) {
        std::ofstream _file (_path_log_file, std::ios::app);
        if (_file.is_open()){
            _file << "\n" <<_message <<" " << value_time << std::endl;
        }
        _file.close();
    }
}
time_interval_log_file::time_interval_log_file(
    std::string message, 
    std::filesystem::path path_log_file
) noexcept : time_interval(std::move(message)), _path_log_file(std::move(path_log_file)){}

std::string set_string_path(std::string_view msg){
    #ifdef _WIN32
        static const std::regex invalid_chars_regex("[<>:\"|?*]");
    #else
        static const std::regex invalid_chars_regex("[/\\0]");
    #endif
    std::string temp_string;
    std::cout << msg <<": ";
    std::getline(std::cin, temp_string);
    if (temp_string.empty()) {
        throw EXEP_work_file("set_string_path Path() # cannot be empty", 0);
    }
    if (std::regex_search(temp_string, invalid_chars_regex)) {
        throw EXEP_work_file("set_string_path Path() # Invalid characters in path", 0);
    }
    return temp_string;
}
