#ifndef WEB_DOWNLOADER_HPP
#define WEB_DOWNLOADER_HPP

#include <string>

#include <curl/curl.h>

class web_downloder
{
public:
  web_downloder();
  ~web_downloder();

  //Метод инициализирует web_downloder, а именно библиотеку curl.
  //Если инициализация прошла неудачно, метод возвращает false.
  bool init();

  //Метод принимает в качестве первого параметра - ссылку(откуда нужно скачать),
  //второй - буффер(куда положить скачанную информацию).
  //Метод возвращает true - успешное скачивание, false - неудачное скачивание.
  bool download(const std::string& url, std::string& buffer);

private:
  CURL* curl;

  //callback для записи скачанной информации в буффер.
  static uint32_t write_callback(void* content, uint32_t size, uint32_t content_amount, void* buffer);
};

#endif
