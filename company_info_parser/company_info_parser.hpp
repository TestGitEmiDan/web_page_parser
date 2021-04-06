#ifndef COMPANY_INFO_PARSER_HPP
#define COMPANY_INFO_PARSER_HPP

#include <string>
#include <vector>

class company_info_parser
{
public:
  enum company_info_attr : int8_t
  {
    SHORT_NAME,
    FULL_NAME,
    INN,
    KPP,
    OKPO,
    OGRN
  };

  struct company_info
  {
    std::string short_name;
    std::string full_name;
    std::string inn;
    std::string kpp;
    std::string okpo;
    std::string ogrn;
  };

  static std::vector<std::pair<std::string, std::string>> parse_companies_url_list(const std::string& companies_html);
  static company_info parse_company_info(const std::string& company_html);

  static const std::vector<std::pair<company_info_attr, std::string>> company_info_attrs;

private:
  static std::pair<std::string, std::string> parse_company_name_and_url(const std::string& company_name_and_url_html);
};

#endif
