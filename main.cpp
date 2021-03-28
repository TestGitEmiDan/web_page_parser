#include <iostream>
#include <string>
#include <logger.hpp>

#include "downloder/web_downloader.hpp"
#include "parser/parser.hpp"

int main(int argc, const char** argv)
{   
    web_downloder web;
    parser parser;
    std::string buffer;

    web.init();

    web.download("https://www.b2b-center.ru/top-1000/", buffer);
    auto container = parser.get_companies_info(buffer);

    for (auto i = 5; i < 10; ++i)
    {
        LOG_INFO(parser.get_company_info(container[i].second));
    }

    return 0;
}