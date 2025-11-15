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
    std::cout<< "___ PARSING: "<< INFO->_path<< " ____\n";
    time_interval timer("search_and_collect: ");

    std::ifstream file(INFO->_path, std::ios::binary);
        
    const size_t CHUNK_SIZE = 64 * 1024 * 1024;
    std::vector<char> chunk_buffer(CHUNK_SIZE);
    size_t bytes_read = 0;

    while (file){
        file.read(chunk_buffer.data(), CHUNK_SIZE);
        bytes_read = file.gcount(); 

        if (bytes_read == 0) break;

        std::string buffer(chunk_buffer.data(), bytes_read);

        auto begin = std::sregex_iterator(buffer.begin(), buffer.end(), _combined_regex);
        auto end = std::sregex_iterator();

        for (auto it = begin; it != end; it++){
            std::smatch match = *it;
            for (size_t i = 1; i < match.size(); i++){
                if (match[i].matched){
                    pattern_lists[i-1].push_back(match[0].str());
                    break;
                } 
            }
        }
        }
        file.close();
        std::cout<<"___________________________\n";
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
    std::cout << "PATTERN '" << *name_iter << "': " 
                  << pattern_iter->size() << " occurrences" << std::endl;
    
    ++pattern_iter;
    ++name_iter;
    }
}

HIDDEN PARSING::PARSING(f_info * _INFO,
        std::vector<std::string> _patterns,
        bool flag_print_all_info) 
{
    if (_INFO == NULL ) {
        return;
    }
    INFO = _INFO;
    std::string _combined_pattern;
    pattern_lists.resize(_patterns.size());
    _pattern_names = _patterns;

    for (size_t i = 0; i < _patterns.size(); i++) {
        if (i > 0) _combined_pattern += "|";
        _combined_pattern += "(" + _patterns[i] + ")";
    }
    _combined_regex = std::regex(_combined_pattern, std::regex::optimize);
    search_and_collect();
    printStats();
    if (flag_print_all_info)
        printAll();
}  

extern "C" {

PARSING* create_parser(f_info* info, const char** patterns, int count, bool flag) {
    std::vector<std::string> pattern_vec;
    for (int i = 0; i < count; ++i) {
        if (patterns[i] != nullptr) {
            pattern_vec.push_back(patterns[i]);
        }
    }
    return new PARSING(info, pattern_vec, flag);
}

void destroy_parser(PARSING* parser) {
    if (parser) {
        delete parser;
        std::cout << "Parser destroyed successfully" << std::endl;
    }
}

}
