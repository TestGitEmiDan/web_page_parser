#include <fstream>
#include <string>
#include <vector>

#include <logger.hpp>

#include "company_info_parser/company_info_parser.hpp"
#include "web_downloader/web_downloader.hpp"

//Функция создает файл csv, заполняет название столбцов
//(Краткое наименование, Полное наименование, ИНН, КПП, ОКПО, ОГРН).
void create_csv_file(const std::string& file_name,
                     const std::vector<std::pair<company_info_parser::company_info_attr, std::string>>& cols)
{
  std::ofstream file(file_name);

  for (const auto& col : cols)
  {
    file << col.second << ",";
  }

  file << std::endl;
}

//Функция получает в качестве параметров : структуру компании, название файла.
//Далее функция записывает в файл - информацию.
void write_to_csv_file(const company_info_parser::company_info& info, const std::string& file_name)
{
  std::ofstream file(file_name, std::ios_base::app);

  file << info.short_name << "," << info.full_name << "," << info.inn << "," << info.kpp << "," << info.okpo << ","
       << info.ogrn << std::endl;
}

int main(int argc, const char** argv)
{
  web_downloder downloader;

  //Проверяем инициализацию библиотеки
  if (!downloader.init())
  {
    log_error("web_downloder init() failed");
    return 1;
  }

  std::string html;

  //Скачиваем url страницу, в которой находится вся информация о компаниях.
  if (!downloader.download("https://www.b2b-center.ru/top-1000/", html))
  {
    log_error("web_downloder download() failed");
    return 1;
  }

  company_info_parser parser;

  //Получаем вектор пар (название компании + ссылка).
  const auto companies_info = parser.parse_companies_url_list(html);

  if (companies_info.empty())
  {
    log_error("company_info_parser parse_companies_url_list() failed");
    return 1;
  }

  //Создаем файл и записываем в него названия столбцов.
  const auto file_name = logger::current_time_str() + "_companies_info.csv";
  create_csv_file(file_name, company_info_parser::company_info_attrs);

  //Итерируем url компаний.
  for (const auto& company_info : companies_info)
  {
    if (!downloader.download("https://www.b2b-center.ru" + company_info.second, html))
    {
      log_error("web_downloder download() failed");
      return 1;
    }

    //Получаем заполненную структуру с информацией о конкретной компании.
    const auto info = parser.parse_company_info(html);

    //Проверяем структуру на валидность.
    if (!info.is_valid())
    {
      break;
    }

    log_info("writing", company_info.first, "data into file");

    //Записываем информацию в файл.
    write_to_csv_file(info, file_name);
  }

  return 0;
}
