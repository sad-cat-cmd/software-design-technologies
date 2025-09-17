// 10 вариант
#include <iostream>
#include "header.hpp"

int main(){
    std::cout << "C++ standard: " << __cplusplus << "\n"<< std::endl;

    std::string file_path_in_little, file_path_in_big;

    file_path_in_little = set_string_path(&file_path_in_little, "INPUT.HTML");
    file_path_in_big = set_string_path(&file_path_in_big, "BIG_HTML");

    PARSING_HTML A (file_path_in_little);

    write_in_big_file(file_path_in_little, file_path_in_big);

    
    return 0;
}