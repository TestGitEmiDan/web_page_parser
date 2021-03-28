#include "web_downloader.hpp"

#include <cstring>
#include <logger.hpp>

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

bool web_downloder::download(const std::string& url, std::string& buffer)
{
    buffer.clear();
    
    if(curl)
    {
        std::cerr << logger::yellow_bold_color << "[web_downloder]" << logger::reset_color << ": download data from: <" << url << ">" <<" ...";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, CURL_MAX_READ_SIZE);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        const auto result{curl_easy_perform(curl) == CURLE_OK ? true : false};
        
        if (result)
        {
            std::cerr << logger::green_bold_color << "  SUCCESS\n" << logger::reset_color;
        }
        else
        {
            std::cerr << logger::red_bold_color << "  ERROR\n" << logger::reset_color;
        }

        return result;
    }

    return false;
}

uint32_t web_downloder::write_callback(void* content, uint32_t size, uint32_t content_amount, void* buffer)
{
    reinterpret_cast<std::string*>(buffer)->append(reinterpret_cast<char*>(content), size * content_amount);
    return size * content_amount;
}
