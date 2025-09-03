// 10 вариант
#include <iostream>

#include "header.hpp"

int main(){
    std::string file_path_in;
    std::string file_path_out;

    file_path_in = get_string_path(&file_path_in, "INPUT.HTML");
    file_path_out = get_string_path(&file_path_out, "OUTPUT_TXT");

    PARSING_HTML(file_path_in, file_path_out);
    

}