#ifndef PARSING_HPP
#define PARSING_HPP

#include "time.hpp"
#include "work_with_file.hpp"

#include <vector>
#include <iostream>
#include <list>
#include <regex>
#include <fstream>

std::string set_string_path(std::string_view msg);

class PARSING{
    private:
    f_info* INFO;
    std::regex _combined_regex;
    std::vector<std::list<std::string>> pattern_lists;
    std::vector <std::string> _pattern_names;
/**
 * @brief функция вывода контейнера std::list<std::string> на экран
 * @param _list - копия объекта типа std::list<std::string>
 * @param msg - сообщение до ввода в поток вывода
 */
    void search_and_collect();
    
    void printAll();

    void printStats();

    public:
    PARSING(f_info * _INFO,
            std::vector<std::string> _patterns);
    ~PARSING (){
        
    }   
};

#endif
