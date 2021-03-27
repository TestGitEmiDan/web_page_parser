#include "web_downloader.hpp"

web_downloder::web_downloder() : curl(nullptr)
{

}

web_downloder::~web_downloder()
{
    if (curl)
    {
        curl_easy_cleanup(curl);
    }
}

bool web_downloder::init()
{
    if (curl)
    {
        return true;
    }

    curl = curl_easy_init();

    return curl != nullptr;
}

bool web_downloder::download(const std::string& website, std::string& buffer)
{
    CURLcode res;

    buffer.clear();
    
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, website.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);

        if (res == CURLE_OK)
        {
            return true;
        }

        else
        {
            return false;
        }
    }

    return false;
}

uint32_t web_downloder::write_callback(void* content, uint32_t size, uint32_t content_amount, void* buffer)
{
    reinterpret_cast<std::string*>(buffer)->append(reinterpret_cast<char*>(content), size * content_amount);
    return size * content_amount;
}