#include <sstream>

#include "parser.hpp"

std::vector<std::pair<std::string, std::string>> parser::get_companies_info(const std::string& buffer)
{
    container_.clear();

    const uint64_t pos_begin = buffer.find("<div data-search=\"top1000-2021\">");

    if (pos_begin == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end = buffer.find("<div data-search=\"top1000-2020\">", pos_begin);

    if (pos_end == std::string::npos)
    {
        return {};
    }

    std::stringstream ss(buffer.substr(pos_begin, pos_end - pos_begin));

    const std::string company_line = "<a class=\"name\" href=";
    std::string cur_string;

    while(std::getline(ss, cur_string))
    {
        const uint64_t pos = cur_string.find_first_not_of("\t");

        if (pos == std::string::npos)
        {
            continue;
        }

        if (!cur_string.compare(pos, company_line.size(), company_line))
        {
            container_.push_back(parse_company_name_url(cur_string));
        }
    }

    return container_;
}

std::pair<std::string, std::string> parser::parse_company_name_url(const std::string& buffer)
{
    std::string url;
    std::string name_company;

    const std::string start_url_attr = "href=\"";
    const std::string end_url_attr = "\" target=\"";
    const std::string start_name_company_attr = "_blank\">";
    const std::string end_name_company_attr = "</a>";

    const uint64_t pos_begin_url = buffer.find(start_url_attr) + start_url_attr.size();

    if (pos_begin_url == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_url = buffer.find(end_url_attr, pos_begin_url);

    if (pos_end_url == std::string::npos)
    {
        return {};
    }

    url = buffer.substr(pos_begin_url, pos_end_url - pos_begin_url);

    const uint64_t pos_begin_name_company = buffer.find(start_name_company_attr, pos_end_url) + start_name_company_attr.size();

    if (pos_begin_name_company == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_name_company = buffer.find(end_name_company_attr, pos_begin_name_company);

    if (pos_end_name_company == std::string::npos)
    {
        return {};
    }

    name_company = buffer.substr(pos_begin_name_company, pos_end_name_company - pos_begin_name_company);

    return {name_company, url};
}