#include <iostream>
#include <list>

std::string get_string_path(std::string* temp_string, std::string masg){
    std::cout << masg <<": ";
    std::cin >> *temp_string;
    return *temp_string;
}

class PARSING_HTML{
    private:
    std::string name_file_in {NULL};
    std::string name_file_out {NULL};

    void open_file() {
        
    }
    public:
    PARSING_HTML(std::string _input, std::string _output){
        name_file_in = _input;
        name_file_out = _output;
    }
    


};