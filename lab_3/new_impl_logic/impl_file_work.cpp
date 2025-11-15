#include "general_header.hpp"
#include <iostream>

EXEP_work_file::EXEP_work_file(std::string _message, int _data_state){
    data_state = _data_state;
    message = _message;
}
int EXEP_work_file::getDataState() {return data_state;}
std::string EXEP_work_file::getMessage() {return message;}

void f_info::update_info() noexcept {
    std::cout <<"____ UPDATES F_INFO :/" << _path << "____\n";
    time_interval timer_f("f_info Updates: ");
    if (_size_bytes == std::filesystem::file_size(_path)) {
        std::cout<<"___________________________\n";
        print_info();
        return;
    }
    this->_size_bytes = std::filesystem::file_size(_path);
    std::cout<<"___________________________\n";
    print_info();
    return; 
}
void f_info::print_info() noexcept {
    std::cout<< "___ PRINT_INFO "<< this->_path<< " ____\n";
    std::cout << "Size file (bytes): " << this->_size_bytes <<"\n";
    std::cout<<"___________________________\n";
    return;
}

std::string get_buffer_from_file(f_info * _info){
    if (_info == NULL ){
        throw EXEP_work_file("file_ptr is NULL", 0);
    }
    std::cout<< "___ GET BUFFER FROM FILE: "<< _info->_path<< " ____\n";
    time_interval timer("get_buffer_from_file: ");
    std::ifstream file(_info->_path, std::ios::binary);
    if (!file.is_open()) {
        throw EXEP_work_file("file is not open:" + _info->_path.string(), 1);
    }
    std::string buffer;
    file.seekg(0, std::ios::end);
    buffer.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(buffer.data(), buffer.size());
    file.close();
    std::cout<<"___________________________\n";
    return buffer;
}

void write_buf_in_file(f_info * _info, std::string_view buffer, size_t target_size){
    if (_info == NULL) {
        throw EXEP_work_file("file_ptr is NULL", 0);
    }
    std::cout<< "___ WRITE IN FILE: /"<< _info->_path<< " ____\n";
    time_interval timer("write buffer in file: ");
    if (buffer.empty()){
        throw EXEP_work_file("buffer is empty", 0);
    }
    size_t current_size = std::filesystem::file_size(_info->_path);
    if (current_size == target_size) {
        std::cout << "File already has target size: " << target_size << " bytes\n";
        _info->update_info();
        return;
    }
    std::ofstream file(_info->_path, std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        throw EXEP_work_file("file is not open:" + _info->_path.string(), 0);
    }
    size_t buffer_size = buffer.size();
    size_t total_written = 0;
    size_t needed_size = (current_size < target_size) ? target_size - current_size : 0;
    if (current_size > target_size) {
        file.close();
        std::filesystem::resize_file(_info->_path, target_size);
        std::cout << "File truncated to: " << target_size << " bytes\n";
        _info->update_info();
        return;
    }
    while(total_written < needed_size){
        size_t remaining = needed_size - total_written;
        size_t to_write = std::min(remaining, buffer_size);

        file.write(buffer.data(), to_write);
        total_written += to_write;

        if (!file) {
            file.close();
            throw EXEP_work_file("Error write in file", 0);
        }
    }

    std::cout << "Recorded: " << total_written << " BYTES\n";
    std::cout << "Recording is over. UPDATES INFO\n";
    file.close();
    _info->update_info();
    std::cout<<"___________________________\n";
    return;
}

f_info::f_info(std::filesystem::path _PATH, uint64_t _SB){
    _path = _PATH;
    _size_bytes = _SB;
}
f_info*_create_file_in_dir(std::filesystem::path _dir,std::string_view _fname){
    time_interval _timer_f ("\t FILE and DIR CREATING/UPDATES: ");
    std::cout<<"____ Directory Creating: /"<< _dir << "____\n";
    if (!std::filesystem::exists(_dir)){
        std::filesystem::create_directory(_dir);
        std::cout<<"The directory has been created: /" << _dir <<"\n";
        std::cout<<"___________________________\n";
    }
    else {
        std::cout<<"The directory has already been created: /" << _dir << "\n";
        std::cout<<"___________________________\n";
    }
    std::filesystem::path _fpath = _dir / _fname;
    std::cout<<"____ FILE CREATING/UPDATES: /"<< _fpath<< "____\n";

    if (!std::filesystem::exists(_fpath)){
        std::ofstream _file (_fpath);
        if (!_file.is_open()) 
            throw EXEP_work_file("file is not open:" + _fpath.string(), 1);
        else {
            std::cout<< "The file has been created " << _fpath << "\n ";
            _file.close();
            std::cout<<"___________________________\n";
            f_info * _info = new f_info(_fpath, 0);
            _info->print_info();
            return _info;
        }
    }
    else {
        std::cout<< "The file has already been created: " << _fpath << " INFO UPDATES" << "\n";
        f_info* _info = new f_info(_fpath,
                                   std::filesystem::file_size(_fpath));
        _info->print_info();
        return _info;
    }
}

time_interval::time_interval(std::string_view message = "Execution time: ")
    : _start(std::chrono::high_resolution_clock::now()),
      _message(message){}
time_interval::~time_interval(){
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - _start);
    if (duration < std::chrono::nanoseconds(1)) {
    // < 1 ns
        std::cout << _message << "0ns\n";
    }
    else if (duration < std::chrono::microseconds(1)) {
        //ns
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
        std::cout << _message << ns.count() << "ns\n";
    }
    else if (duration < std::chrono::milliseconds(1)) {
        // mks
       auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
       std::cout << _message << us.count() << "μs\n";
    }
    else if (duration < std::chrono::seconds(1)) {
            // ms
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        std::cout << _message << ms.count() << "ms\n";
    }
    else if (duration < std::chrono::minutes(1)) {
        // s
        auto s = std::chrono::duration_cast<std::chrono::seconds>(duration);
        std::cout << _message << s.count() << "s\n";
    }
    else {
        // m + 
        auto min = std::chrono::duration_cast<std::chrono::minutes>(duration);
        std::cout << _message << min.count() << "min\n";
    }
    std::cout<< "\n \n";
}

std::string set_string_path(std::string_view msg){
    std::string temp_string;
    std::cout << msg <<": ";
    std::cin >> temp_string;
    return temp_string;
}
