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

    bool is_valid() const;
  };

  //Метод принимает строчку, в которой лежит главный html, вся информация о компаниях.
  //Метод производит парсинг информации и отдает вектор пар(название компании + ее ссылка).
  //Если html не валидный, то отдает пустой вектор.
  static std::vector<std::pair<std::string, std::string>> parse_companies_url_list(const std::string& companies_html);

  //Метод принимает строчку(html компании) и производит парсинг нужной информации.
  //Метод отдает заполненую структуру, в которой содержится вся информации о компании
  //(Краткое наименование, Полное наименование, ИНН, КПП, ОКПО, ОГРН).
  //Если html не валидный или не удалось спарсить нужную информацию о компании, метод отдает пустую структуру.
  static company_info parse_company_info(const std::string& company_html);

  //Вектор пар, по которому производится поиск нужных полей компании.
  //(Краткое наименование, Полное наименование, ИНН, КПП, ОКПО, ОГРН).
  //Используется для создании столбцов в .csv файле.
  static const std::vector<std::pair<company_info_attr, std::string>> company_info_attrs;

private:
  //Вспомогательный метод, который вызывается в методе parse_companies_url_list.
  //Метод производит парсинг названия компании и ее ссылке из входящей строчки.
  //Если входящая строка невалидная или пустая, то метод отдает пустую пару.
  static std::pair<std::string, std::string> parse_company_name_and_url(const std::string& company_name_and_url_html);
};

#endif
