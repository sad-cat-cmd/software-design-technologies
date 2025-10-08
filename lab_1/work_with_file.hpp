#include <fstream>
#include <filesystem>
#include <vector>

uint64_t lines_count(std::filesystem::path _path, uint64_t _size);

struct f_info
{
    std::filesystem::path _path;
    uint64_t _size_bytes;
    
    f_info (std::filesystem::path _PATH,
            uint64_t _SB): _path (_PATH), _size_bytes (_SB)
    {}
    void update_info(){
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
    void print_info(){
    std::cout<< "___ PRINT_INFO "<< this->_path<< " ____\n";
    std::cout << "Size file (bytes): " << this->_size_bytes <<"\n";
    std::cout<<"___________________________\n";
    return;
}
};

std::string get_buffer_from_file(f_info * _info){
    std::cout<< "___ GET BUFFER FROM FILE: "<< _info->_path<< " ____\n";
    time_interval timer("get_buffer_from_file: ");
    std::ifstream file(_info->_path, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "file is not open: "<< _info->_path.string() << "\n";
        std::cout<<"___________________________\n";
        return "";
    }
    std::string buffer;
    file.seekg(0, std::ios::end);
    buffer.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(buffer.data(), buffer.size());
    std::cout<<"___________________________\n";
    return buffer;
}

void write_buf_in_file(f_info * _info,
                       std::string_view buffer,
                       size_t target_size){
    std::cout<< "___ WRITE IN FILE: /"<< _info->_path<< " ____\n";
    time_interval timer("write buffer in file: ");
    std::ofstream file(_info->_path, std::ios::binary);
    if (buffer.empty()){
        std::cout << "buffer is empty \n";
        std::cout<<"___________________________\n";
        file.close();
        return;
    }
    if (!file.is_open()) {
        std::cout << "file is not open: " << _info->_path.string() << "\n";
        std::cout<<"___________________________\n";
        file.close();
        return;
    }

    if (_info->_size_bytes == target_size) {
        std::cout<< "File's size = target_size \n";
        std::cout<<"___________________________\n";
        return;
    }
    size_t buffer_size = buffer.size();
    size_t total_written = 0;
    while(total_written < target_size){
        size_t remaining = target_size - total_written;
        size_t to_write = std::min(remaining, buffer_size);

        file.write(buffer.data(), to_write);
        total_written += to_write;

        if (!file) {
            std::cout<< "Error write in file" << "\n";
            std::cout<<"___________________________\n";
            file.close();
            return;
        }
    }
    std::cout << "Recorded: " << total_written << "  BYTES\n";
    std::cout << "Recording is over. UPDATES INFO\n";
    _info->update_info();
    std::cout<<"___________________________\n";
    file.close();
    return;
}

uint64_t lines_count(std::filesystem::path _path,
                     uint64_t _size){
    uint64_t lines_count = 0;
    uint64_t total_read = 0;
    std::ifstream file(_path, std::ios::binary);
    const int size_buf = 1 * 1024 * 1024;
    std::vector <char> buffer(size_buf);
        
    while (file){
        file.read(buffer.data(), size_buf);
        size_t bytes_read = file.gcount();
        
        if (bytes_read == 0) break;
        
        const char* data = buffer.data();
        for (size_t i = 0; i < bytes_read; ++i) {
            if (data[i] == '\n') {
                lines_count++;
            }
        }
    }
    return lines_count;
}

f_info*_create_file_in_dir(std::filesystem::path _dir,
                           std::string_view _fname){
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
        if (!_file.is_open()) {
            std::cout<< "The file has not been created"<< _fpath << "\n";
            std::cout<<"___________________________\n";
            return nullptr;
        }
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


