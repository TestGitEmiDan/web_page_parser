#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>

class parser
{
public:
    std::vector<std::pair<std::string, std::string>> get_companies_url_list(const std::string& companies_html);
    std::string get_company_info(const std::string company_html);

private:
    static std::pair<std::string, std::string> parse_company_name_and_url(const std::string& company_html);
    std::vector<std::pair<std::string, std::string>> company_info_;
};

#endif
