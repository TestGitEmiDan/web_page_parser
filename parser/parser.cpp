#include <sstream>
#include <logger.hpp>

#include "parser.hpp"

std::vector<std::pair<std::string, std::string>> parser::get_companies_url_list(const std::string& companies_html)
{
    if (companies_html.empty())
    {
        log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ": companies_html is empty");
    }

    company_info_.clear();

    const uint64_t pos_begin = companies_html.find("<div data-search=\"top1000-2021\">");

    if (pos_begin == std::string::npos)
    {
        log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ":", logger::red_bold_color, "invalid html format", logger::reset_color);
        return {};
    }

    const uint64_t pos_end = companies_html.find("<div data-search=\"top1000-2020\">", pos_begin);

    if (pos_end == std::string::npos)
    {
        log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ":", logger::red_bold_color, "invalid html format", logger::reset_color);
        return {};
    }

    std::stringstream ss(companies_html.substr(pos_begin, pos_end - pos_begin));

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
            const auto result{parse_company_name_and_url(cur_string)};

            if (!result.first.empty() && !result.second.empty())
            {
                company_info_.push_back(result);
            }
        }
    }

    log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ": found companies [", company_info_.size(), "]");

    return company_info_;
}

std::pair<std::string, std::string> parser::parse_company_name_and_url(const std::string& company_html)
{
    if (company_html.empty())
    {
        log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ": company_html is empty");
        return {};
    }

    std::string url;
    std::string name_company;

    const std::string start_url_attr = "href=\"";
    const std::string end_url_attr = "\" target=\"";
    const std::string start_name_company_attr = "_blank\">";
    const std::string end_name_company_attr = "</a>";

    const uint64_t pos_begin_url = company_html.find(start_url_attr) + start_url_attr.size();

    if (pos_begin_url == std::string::npos)
    {
        log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ":", logger::red_bold_color, "invalid html format:", logger::reset_color, company_html);
        return {};
    }

    const uint64_t pos_end_url = company_html.find(end_url_attr, pos_begin_url);

    if (pos_end_url == std::string::npos)
    {
        log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ":", logger::red_bold_color, "invalid html format:", logger::reset_color, company_html);
        return {};
    }

    url = company_html.substr(pos_begin_url, pos_end_url - pos_begin_url);

    const uint64_t pos_begin_name_company = company_html.find(start_name_company_attr, pos_end_url) + start_name_company_attr.size();

    if (pos_begin_name_company == std::string::npos)
    {
        log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ":", logger::red_bold_color, "invalid html format:", logger::reset_color, company_html);
        return {};
    }

    const uint64_t pos_end_name_company = company_html.find(end_name_company_attr, pos_begin_name_company);

    if (pos_end_name_company == std::string::npos)
    {
        log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ":", logger::red_bold_color, "invalid html format:", logger::reset_color, company_html);
        return {};
    }

    name_company = company_html.substr(pos_begin_name_company, pos_end_name_company - pos_begin_name_company);

    return {name_company, url};
}

std::vector<std::pair<std::string, std::string>> parser::get_company_info(const std::string company_info_html)
{
    if (company_info_html.empty())
    {
        log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ": company_info is empty");
        return {};
    }

    std::vector<std::pair<std::string, std::string>> company_info{{"Краткое наименование", ""},
    {"Полное наименование", ""},
    {"ИНН", ""},
    {"КПП", ""},
    {"ОКПО", ""},
    {"ОГРН", ""}};

    auto pos = 0;
    const std::string start_tag = "<td>";
    const std::string end_tag = "</td>";

    for (auto& info : company_info)
    {
        const auto begin_pos = company_info_html.find(info.first, pos) + info.first.size() + start_tag.size() + end_tag.size();

        if (begin_pos == std::string::npos)
        {
            log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ":", logger::red_bold_color, "invalid html format:", logger::reset_color, company_info_html);
            return {};
        }

        const auto end_pos = company_info_html.find(end_tag, begin_pos);

        if (end_pos == std::string::npos)
        {
            log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ":", logger::red_bold_color, "invalid html format:", logger::reset_color, company_info_html);
            return {};
        }

        info.second = company_info_html.substr(begin_pos, end_pos - begin_pos);
    }

    return company_info;
}   