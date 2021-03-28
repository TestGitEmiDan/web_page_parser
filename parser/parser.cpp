#include <sstream>
#include <logger.hpp>

#include "parser.hpp"
#include "../downloder/web_downloader.hpp"

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

std::string parser::get_company_info(const std::string url)
{
    std::string main_url = "https://www.b2b-center.ru" + url;  
    std::string company_info;                                   

    web_downloder web;
    web.init();

    web.download(main_url, company_info);

    if (company_info.empty())
    {
        return {};
    }

    const std::string start_string = "<td class=\"small\">";
    const std::string end_string = "</td>";
    const std::string start_description = "<td>";

    std::string short_title;

    const uint64_t pos_begin_short_title = company_info.find(start_string) + start_string.size();

    if (pos_begin_short_title == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_short_title = company_info.find(end_string, pos_begin_short_title);

    if (pos_end_short_title == std::string::npos)
    {
        return {};
    }

    short_title = company_info.substr(pos_begin_short_title, pos_end_short_title - pos_begin_short_title);

    std::string short_title_name;

    const uint64_t pos_begin_short_title_name = company_info.find(start_description, pos_end_short_title) + start_description.size();

    if (pos_begin_short_title_name == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_short_title_name = company_info.find(end_string, pos_begin_short_title_name);

    if (pos_end_short_title_name == std::string::npos)
    {
        return {};
    }

    short_title_name = company_info.substr(pos_begin_short_title_name, pos_end_short_title_name - pos_begin_short_title_name);

    std::string full_name;

    const uint64_t pos_begin_full_name = company_info.find(start_string, pos_end_short_title_name) + start_string.size();

    if (pos_begin_full_name == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_full_name = company_info.find(end_string, pos_begin_full_name);

    if (pos_end_full_name == std::string::npos)
    {
        return {};
    }

    full_name = company_info.substr(pos_begin_full_name, pos_end_full_name - pos_begin_full_name);

    std::string full_name_description;

    const uint64_t pos_begin_full_name_description = company_info.find(start_description, pos_end_full_name) + start_description.size();

    if (pos_begin_full_name_description == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_full_name_description = company_info.find(end_string, pos_begin_full_name_description);

    if (pos_end_full_name_description == std::string::npos)
    {
        return {};
    }

    full_name_description = company_info.substr(pos_begin_full_name_description, pos_end_full_name_description - pos_begin_full_name_description);

    std::string inn;

    const uint64_t pos_begin_inn = company_info.find(start_string, pos_end_full_name_description) + start_string.size();

    if (pos_begin_inn == std::string::npos)
    {
        return {}; 
    }

    const uint64_t pos_end_inn = company_info.find(end_string, pos_begin_inn);

    if (pos_end_inn == std::string::npos)
    {
        return {};
    }

    inn = company_info.substr(pos_begin_inn, pos_end_inn - pos_begin_inn);


    std::string inn_description;

    const uint64_t pos_begin_inn_description = company_info.find(start_description, pos_end_inn) + start_description.size();

    if (pos_begin_inn_description == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_inn_description = company_info.find(end_string, pos_begin_inn_description);

    if (pos_end_inn_description == std::string::npos)
    {
        return {};
    }

    inn_description = company_info.substr(pos_begin_inn_description, pos_end_inn_description - pos_begin_inn_description);

    std::string kpp;

    const uint64_t pos_begin_kpp = company_info.find(start_string, pos_end_inn_description) + start_string.size();

    if (pos_begin_kpp == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_kpp = company_info.find(end_string, pos_begin_kpp);

    if (pos_end_kpp == std::string::npos)
    {
        return {};
    }

    kpp = company_info.substr(pos_begin_kpp, pos_end_kpp - pos_begin_kpp);

    std::string kpp_description;

    const uint64_t pos_begin_kpp_description = company_info.find(start_description, pos_end_kpp) + start_description.size();

    if (pos_begin_kpp_description == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_kpp_description = company_info.find(end_string, pos_begin_kpp_description);

    if (pos_end_kpp_description == std::string::npos)
    {
        return {};
    }

    kpp_description = company_info.substr(pos_begin_kpp_description, pos_end_kpp_description - pos_begin_kpp_description);

    std::string okpo;

    const uint64_t pos_begin_okpo = company_info.find(start_string, pos_end_kpp_description) + start_string.size();

    if (pos_begin_okpo == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_okpo = company_info.find(end_string, pos_begin_okpo);

    if (pos_end_okpo == std::string::npos)
    {
        return {};
    }

    okpo = company_info.substr(pos_begin_okpo, pos_end_okpo - pos_begin_okpo);


    std::string okpo_description;

    const uint64_t pos_begin_okpo_description = company_info.find(start_description, pos_end_okpo) + start_description.size();

    if (pos_begin_okpo_description == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_okpo_description = company_info.find(end_string, pos_begin_okpo_description);

    if (pos_end_okpo_description == std::string::npos)
    {
        return {};
    }

    okpo_description = company_info.substr(pos_begin_okpo_description, pos_end_okpo_description - pos_begin_okpo_description);

    std::string ogrn;

    const uint64_t pos_begin_ogrn = company_info.find(start_string, pos_end_okpo_description) + start_string.size();

    if (pos_begin_ogrn == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_ogrn = company_info.find(end_string, pos_begin_ogrn);

    if (pos_end_ogrn == std::string::npos)
    {
        return {};
    }

    ogrn = company_info.substr(pos_begin_ogrn, pos_end_ogrn - pos_begin_ogrn);


    std::string ogrn_description;

    const uint64_t pos_begin_ogrn_description = company_info.find(start_description, pos_end_ogrn) + start_description.size();

    if (pos_begin_ogrn_description == std::string::npos)
    {
        return {};
    }

    const uint64_t pos_end_ogrn_description = company_info.find(end_string, pos_begin_ogrn_description);

    if (pos_end_ogrn_description == std::string::npos)
    {
        return {};
    }

    ogrn_description = company_info.substr(pos_begin_ogrn_description, pos_end_ogrn_description - pos_begin_ogrn_description);

    std::string finish = short_title + " " + short_title_name + "\n" + full_name + " " + full_name_description + "\n" + inn + " " + inn_description + "\n"
    + kpp + " " + kpp_description + "\n" + okpo + " " + okpo_description + "\n" + ogrn + " " + ogrn_description + "\n";

    return finish;
}   