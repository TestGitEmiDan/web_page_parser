#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>

class Parser// parser
{
public:
    void get_company_info(std::string& buffer);//vector pair <string(name_company), url(чистый url)>

private:
    //pair<name, url> parse_company_name_url(buffer)
    std::vector<std::string> container_;
};


#endif