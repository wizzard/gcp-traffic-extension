#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::string read_text_file(std::string &filename)
{
    std::stringstream buffer;
    buffer << std::ifstream(filename).rdbuf();
    return buffer.str();
}