// 10 вариант
#include <iostream>
#include "header.hpp"

int main(){
    std::cout << "C++ standard: " << __cplusplus << "\n"<< std::endl;

    std::string file_path_in;

    file_path_in = set_string_path(&file_path_in, "INPUT.HTML");

    PARSING_HTML A (file_path_in);
    
    return 0;
}