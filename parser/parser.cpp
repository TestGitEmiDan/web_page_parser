#include <iostream>
#include <logger.hpp>
#include <string.h>

#include "parser.hpp"
#include "save_to_file.hpp"

void Parser::parse_name_company(std::string& buffer)
{
    std::string temp_buffer;
    std::string cur_string;

    size_t pos_begin = buffer.find("<div data-search=\"top1000-2021\">");
    size_t pos_end = buffer.find("<div data-search=\"top1000-2020\">");

    // for (auto i = pos_begin; i < pos_begin + 32; ++i)
    // {
    //     std::cout << buffer[i];
    // }

    // std::cout << "\n";

    // for (auto i = pos_end; i < pos_end + 32; ++i)
    // {
    //     std::cout << buffer[i];
    // }

    // std::cout << "\n";

    // LOG_INFO(pos_begin);
    // LOG_INFO(pos_end);
    
    temp_buffer = buffer.substr(pos_begin, pos_end - pos_begin);

    std::stringstream ss(temp_buffer);

    const char* company_line = "<a class=\"name\" href=";

    int32_t count = 0;

    while(std::getline(ss, cur_string))
    {
        size_t pos = cur_string.find_first_not_of("\t");

        if (pos == std::string::npos)
        {
            continue;
        }

        if (!cur_string.compare(pos, strlen(company_line), company_line))
        {
            save_to_file(cur_string);
            save_to_file("\n");
        }
    }
}