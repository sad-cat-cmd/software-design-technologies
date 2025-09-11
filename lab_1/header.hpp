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
std::string set_string_path(std::string* temp_string, std::string msg){
    std::cout << msg <<": ";
    std::cin >> *temp_string;
    return *temp_string;
}

/**
 * @brief класс для парсинга
 * @details private: имя файла,rexex для каждого типа выражений, списки для хранения строк с вхождением выражений
 *                   функция вывода, функция парсинга
 *          public: конструктор с параметрами, 
 *      
 */
class PARSING_HTML{
    private:
    std::string name_file_in;

    std::regex regular_header {"<h[1-6].*>.*</h[1-6]>", std::regex::icase};
    std::regex regular_paragraph {"<p.*>.*</p>", std::regex::icase};
    std::regex regular_bold {"<b.*>.*</b>", std::regex::icase};

    std::list<std::string> list_headers;
    std::list<std::string> list_paragraphs;
    std::list<std::string> list_bolds;
/**
 * @brief функция вывода контейнера std::list<std::string> на экран
 * @param _list - копия объекта типа std::list<std::string>
 * @param msg - сообщение до ввода в поток вывода
 */
    void _print(std::list<std::string> _list, std::string msg)
    {
        std::list<std::string>::const_iterator _it {_list.cbegin()};

        std::cout <<msg<< std::endl;
        while (_it != _list.cend()){
            std::cout<<*_it<<std::endl;
            _it++;
        }
        std::cout<<"-----------------------"<<std::endl;
        
    }
/**
* @brief функция парсинга
* @details input - файловый поток, передаем в контструктор имя файла
           line - строка, в будущем содержащая строку из файла
           цикл while: пока не достиг конца файла, тело: добавление на каждой итерации строки в контейнеры, при вхождении в нее regex
           вызов функции _print для вывода каждого контейнера        
*/
    void _parsing(){
        std::ifstream input(name_file_in);
        std::string line;
        if (!input.is_open()){return;}

        while (getline(input, line))
        {
            if (std::regex_search(line, regular_header)){list_headers.push_back(line);}
            if (std::regex_search(line, regular_paragraph)){list_paragraphs.push_back(line);}
            if (std::regex_search(line, regular_bold)){list_bolds.push_back(line);}
        }
        input.close();

        _print(list_headers, "lines with the occurrence of HEADERS_tag:");
        _print(list_paragraphs, "lines with the occurrence of PARAGRAPH_tag:");
        _print(list_bolds, "lines with the occurrence of BOLD_tag");

    }
    
    public:
    PARSING_HTML(std::string _input){
        name_file_in = _input;
        _parsing();
    }
};