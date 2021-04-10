#include "company_info_parser.hpp"

#include <sstream>

#include <logger.hpp>

const std::vector<std::pair<company_info_parser::company_info_attr, std::string>>
    company_info_parser::company_info_attrs{{SHORT_NAME, "Краткое наименование"},
                                            {FULL_NAME, "Полное наименование"},
                                            {INN, "ИНН"},
                                            {KPP, "КПП"},
                                            {OKPO, "ОКПО"},
                                            {OGRN, "ОГРН"}};


bool company_info_parser::company_info::is_valid() const
{
  return !short_name.empty() || !full_name.empty() || !inn.empty() || !kpp.empty() || !okpo.empty() || !ogrn.empty();
}

std::vector<std::pair<std::string, std::string>> company_info_parser::parse_companies_url_list(
    const std::string& companies_html)
{
  if (companies_html.empty())
  {
    log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ": companies_html is empty");
  }

  std::vector<std::pair<std::string, std::string>> companies_info;

  const uint64_t pos_begin = companies_html.find("<div data-search=\"top1000-2021\">");

  if (pos_begin == std::string::npos)
  {
    log_info(logger::yellow_bold_color,
             "[parser]",
             logger::reset_color,
             ":",
             logger::red_bold_color,
             "invalid html format",
             logger::reset_color);
    return {};
  }

  const uint64_t pos_end = companies_html.find("<div data-search=\"top1000-2020\">", pos_begin);

  if (pos_end == std::string::npos)
  {
    log_info(logger::yellow_bold_color,
             "[parser]",
             logger::reset_color,
             ":",
             logger::red_bold_color,
             "invalid html format",
             logger::reset_color);
    return {};
  }

  std::stringstream ss(companies_html.substr(pos_begin, pos_end - pos_begin));

  const std::string company_line = "<a class=\"name\" href=";
  std::string cur_string;

  while (std::getline(ss, cur_string))
  {
    const uint64_t pos = cur_string.find_first_not_of("\t");

    if (pos == std::string::npos)
    {
      continue;
    }

    if (cur_string.compare(pos, company_line.size(), company_line) == 0)
    {
      const auto result = parse_company_name_and_url(cur_string);

      if (!result.first.empty() && !result.second.empty())
      {
        companies_info.push_back(result);
      }
    }
  }

  log_info(
      logger::yellow_bold_color, "[parser]", logger::reset_color, ": found companies [", companies_info.size(), "]");

  return companies_info;
}

std::pair<std::string, std::string> company_info_parser::parse_company_name_and_url(
    const std::string& company_name_and_url_html)
{
  if (company_name_and_url_html.empty())
  {
    log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ": company_name_and_url_html is empty");
    return {};
  }

  std::string url;
  std::string name_company;

  const std::string start_url_attr = "href=\"";
  const std::string end_url_attr = "\" target=\"";
  const std::string start_name_company_attr = "_blank\">";
  const std::string end_name_company_attr = "</a>";

  const uint64_t pos_begin_url = company_name_and_url_html.find(start_url_attr) + start_url_attr.size();

  if (pos_begin_url == std::string::npos)
  {
    log_info(logger::yellow_bold_color,
             "[parser]",
             logger::reset_color,
             ":",
             logger::red_bold_color,
             "invalid html format:",
             logger::reset_color,
             company_name_and_url_html);
    return {};
  }

  const uint64_t pos_end_url = company_name_and_url_html.find(end_url_attr, pos_begin_url);

  if (pos_end_url == std::string::npos)
  {
    log_info(logger::yellow_bold_color,
             "[parser]",
             logger::reset_color,
             ":",
             logger::red_bold_color,
             "invalid html format:",
             logger::reset_color,
             company_name_and_url_html);
    return {};
  }

  url = company_name_and_url_html.substr(pos_begin_url, pos_end_url - pos_begin_url);

  const uint64_t pos_begin_name_company =
      company_name_and_url_html.find(start_name_company_attr, pos_end_url) + start_name_company_attr.size();

  if (pos_begin_name_company == std::string::npos)
  {
    log_info(logger::yellow_bold_color,
             "[parser]",
             logger::reset_color,
             ":",
             logger::red_bold_color,
             "invalid html format:",
             logger::reset_color,
             company_name_and_url_html);
    return {};
  }

  const uint64_t pos_end_name_company = company_name_and_url_html.find(end_name_company_attr, pos_begin_name_company);

  if (pos_end_name_company == std::string::npos)
  {
    log_info(logger::yellow_bold_color,
             "[parser]",
             logger::reset_color,
             ":",
             logger::red_bold_color,
             "invalid html format:",
             logger::reset_color,
             company_name_and_url_html);
    return {};
  }

  name_company =
      company_name_and_url_html.substr(pos_begin_name_company, pos_end_name_company - pos_begin_name_company);

  return {name_company, url};
}

company_info_parser::company_info company_info_parser::parse_company_info(const std::string& company_info_html)
{
  if (company_info_html.empty())
  {
    log_info(logger::yellow_bold_color, "[parser]", logger::reset_color, ": company_info_html is empty");
    return {};
  }

  company_info cur_company_info;

  auto start_pos = 0;
  const std::string start_tag = "<td>";
  const std::string end_tag = "</td>";

  for (auto& attr : company_info_attrs)
  {
    auto begin_pos = company_info_html.find(attr.second, start_pos);

    if (begin_pos == std::string::npos)
    {
      continue;
    }

    begin_pos += attr.second.size() + start_tag.size() + end_tag.size();

    const auto end_pos = company_info_html.find(end_tag, begin_pos);

    if (end_pos == std::string::npos)
    {
      log_info(logger::yellow_bold_color,
               "[parser]",
               logger::reset_color,
               ":",
               logger::red_bold_color,
               "invalid html format:",
               logger::reset_color,
               company_info_html);
      return {};
    }

    const auto value = company_info_html.substr(begin_pos, end_pos - begin_pos);

    switch (attr.first)
    {
      case SHORT_NAME:
        cur_company_info.short_name = value;
        break;
      case FULL_NAME:
        cur_company_info.full_name = value;
        break;
      case INN:
        cur_company_info.inn = value;
        break;
      case KPP:
        cur_company_info.kpp = value;
        break;
      case OKPO:
        cur_company_info.okpo = value;
        break;
      case OGRN:
        cur_company_info.ogrn = value;
        break;
      default:
        break;
    }

    start_pos = end_pos;
  }

  return cur_company_info;
}
