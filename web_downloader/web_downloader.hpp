#ifndef WEB_DOWNLOADER_HPP
#define WEB_DOWNLOADER_HPP

#include <curl/curl.h>
#include <string>

class web_downloder
{
public:
    web_downloder();
    ~web_downloder();
    
    bool init();
    bool download(const std::string& url, std::string& buffer);

private:
    CURL* curl;
    static uint32_t write_callback(void* content, uint32_t size, uint32_t content_amount, void* buffer);
};

#endif
