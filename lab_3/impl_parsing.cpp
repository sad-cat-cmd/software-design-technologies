#include "general_header.hpp"
#include <iostream>

#ifdef __GNUC__
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
    #define HIDDEN __attribute__((visibility("hidden")))
#else
    #define EXPORT
    #define IMPORT  
    #define HIDDEN
#endif

HIDDEN void PARSING::search_and_collect(){
    time_interval_get timer ("Time run: ");
    std::string str_log_information;

    if (!write_log_file) {
        std::cout<< "PARSING:  "<< INFO->_path.string() <<"\n";
    }
    if (write_log_file && INFO->flag_write_info == 2){
        str_log_information += "PARSING:  " + INFO->_path.string() + "\n";
    } 

    std::ifstream file(INFO->_path, std::ios::binary);
    
    const size_t CHUNK_SIZE = 64 * 1024 * 1024;
    std::vector<char> chunk_buffer(CHUNK_SIZE);
    size_t bytes_read = 0;
    size_t total_read = 0;
    size_t count_total_occurrences = 0;

    while (file){
        file.read(chunk_buffer.data(), CHUNK_SIZE);
        bytes_read = file.gcount(); 
        total_read += bytes_read;

        if (bytes_read == 0) break;

        std::string buffer(chunk_buffer.data(), bytes_read);

        auto begin = std::sregex_iterator(buffer.begin(), buffer.end(), _combined_regex);
        auto end = std::sregex_iterator();

        for (auto it = begin; it != end; it++){
            std::smatch match = *it;
            for (size_t i = 1; i < match.size(); i++){
                if (match[i].matched){
                    count_total_occurrences++;
                    pattern_lists[i-1].push_back(match[0].str());
                    break;
                } 
            }
        }
    }
    if (!write_log_file){
        std::cout << "number of bytes parsed:  "<< total_read <<"\n";
        std::cout << "total number of occurrences:" << count_total_occurrences << "\n";
        std::cout << timer.get_str_time_interval() << "\n";
    }

    if (write_log_file && INFO->flag_write_info == 2){
        str_log_information += "number of bytes parsed:  " + std::to_string(total_read) + "\n" + "number of occurrences: " + std::to_string(count_total_occurrences) + "\n";
        str_log_information += timer.get_str_time_interval();
        INFO->write_info(str_log_information);
    }
    file.close();
}
HIDDEN void PARSING::printAll() {
    for (auto pattern_it = pattern_lists.cbegin(); pattern_it != pattern_lists.cend(); ++pattern_it) {
        for (auto list_it = pattern_it->cbegin(); list_it != pattern_it->cend(); ++list_it) {
           std::cout << *list_it << std::endl;
        }
    }
}
HIDDEN void PARSING::printStats() {
    auto pattern_iter = pattern_lists.cbegin();
    auto name_iter = _pattern_names.cbegin();

    while (pattern_iter != pattern_lists.cend() && name_iter != _pattern_names.cend()) {
    std::cout << "PATTERN " << *name_iter << ": " 
                  << pattern_iter->size() << " occurrences" << std::endl;
    ++pattern_iter;
    ++name_iter;
    }
}
HIDDEN void PARSING::write_all_stats_file() {
    std::ofstream file(INFO->path_log_file, std::ios::app);
    if (file.is_open()){
        for (auto pattern_it = pattern_lists.cbegin(); pattern_it != pattern_lists.cend(); ++pattern_it) {
            for (auto list_it = pattern_it->cbegin(); list_it != pattern_it->cend(); ++list_it) {
                file << *list_it << std::endl;
            }
        }
        file.close();
    }
}
HIDDEN void PARSING::write_stats_file(){
    auto pattern_iter = pattern_lists.cbegin();
    auto name_iter = _pattern_names.cbegin();
    std::string str_to_write_file;

    while (pattern_iter != pattern_lists.cend() && name_iter != _pattern_names.cend()) {
        str_to_write_file += "PATTERN " + *name_iter +": " + std::to_string(pattern_iter->size()) +" occurrences" + "\n";
        ++pattern_iter;
        ++name_iter;
    }
    INFO->write_info(str_to_write_file);

}
HIDDEN PARSING::PARSING(f_info * _INFO,
        std::vector<std::string> _patterns,
        bool flag_print_all_info, 
        bool _write_log_file) 
{
    if (_INFO == NULL ) {
        return;
    }
    INFO = _INFO;
    std::string _combined_pattern;
    pattern_lists.resize(_patterns.size());
    _pattern_names = _patterns;
    write_log_file = _write_log_file;

    for (size_t i = 0; i < _patterns.size(); i++) {
        if (i > 0) _combined_pattern += "|";
        _combined_pattern += "(" + _patterns[i] + ")";
    }
    _combined_regex = std::regex(_combined_pattern, std::regex::optimize);
    search_and_collect();
    if (!write_log_file) {
        printStats();
        if (flag_print_all_info)
            printAll();
    }
    if (write_log_file && INFO->flag_write_info == 2) {
        write_stats_file();
        if (flag_print_all_info)
            write_all_stats_file();
    }
}  

extern "C" {

PARSING* create_parser(f_info* info, const char** patterns, int count, bool flag_all_info, bool write_info_file) {
    std::vector<std::string> pattern_vec;
    for (int i = 0; i < count; ++i) {
        if (patterns[i] != nullptr) {
            pattern_vec.push_back(patterns[i]);
        }
    }
    return new PARSING(info, pattern_vec, flag_all_info, write_info_file);
}

void destroy_parser(PARSING* parser) {
    if (parser) 
        delete parser;
}
}
