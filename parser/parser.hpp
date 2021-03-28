#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>

class parser
{
public:
    std::vector<std::pair<std::string, std::string>> get_companies_info(const std::string& buffer);
    void print_companies_info();
    std::string get_company_info(const std::string url);

private:
    static std::pair<std::string, std::string> parse_company_name_url(const std::string& buffer);
    std::vector<std::pair<std::string, std::string>> container_;
};


#endif