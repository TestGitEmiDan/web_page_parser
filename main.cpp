#include <iostream>
#include <string>
#include <logger.hpp>

#include "web_downloader.hpp"
#include "parser.hpp"

int main(int argc, const char** argv)
{   
    web_downloder web;
    Parser parser;
    std::string buffer;

    web.init();

    web.download("https://www.b2b-center.ru/top-1000/", buffer);

    parser.parse_name_company(buffer);

    return 0;
}