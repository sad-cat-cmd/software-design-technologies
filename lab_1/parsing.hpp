#include "time.hpp"
#include "work_with_file.hpp"

#include <vector>
#include <iostream>
#include <list>
#include <regex>
#include <fstream>

/**
 * @brief функция предназначенная для ввода текста в строку через поток ввода
 * @param temp_string - строка, в которую осуществляется ввод текста
 * @param msg - сообщение до ввода в поток вывода
 * @return - передаваемая строка
 */
std::string set_string_path(std::string_view msg){
    std::string temp_string;
    std::cout << msg <<": ";
    std::cin >> temp_string;
    return temp_string;
}

/**
 * @brief класс для парсинга
 * @details private: имя файла,regex для каждого типа выражений, списки для хранения строк с вхождением выражений
 *                   функция вывода, функция парсинга
 *          public: конструктор с параметрами, 
 *      
 */
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
    void search_and_collect(){
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
    
    void printAll() const {
        for (auto pattern_it = pattern_lists.cbegin(); pattern_it != pattern_lists.cend(); ++pattern_it) {
            for (auto list_it = pattern_it->cbegin(); list_it != pattern_it->cend(); ++list_it) {
               std::cout << *list_it << std::endl;
            }
        }
    }

    void printStats() {
        auto pattern_iter = pattern_lists.cbegin();
        auto name_iter = _pattern_names.cbegin();

        while (pattern_iter != pattern_lists.cend() && name_iter != _pattern_names.cend()) {
        std::cout << "PATTERN '" << *name_iter << "': " 
                  << pattern_iter->size() << " occurrences" << std::endl;
    
        ++pattern_iter;
        ++name_iter;
        }
    }

    public:
    PARSING(f_info * _INFO,
            std::vector<std::string> _patterns) 
    {
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
        //printAll();
    }
    ~PARSING (){
        
    }    
};