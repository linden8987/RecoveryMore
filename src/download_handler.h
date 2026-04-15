#ifndef DOWNLOAD_HANDLER_H
#define DOWNLOAD_HANDLER_H

#include <string>
#include <curl/curl.h>

class DownloadManager {
public:
    DownloadManager();
    ~DownloadManager();

    bool downloadFile(const std::string &url, const std::string &destination);
    static size_t writeData(void *ptr, size_t size, size_t nmemb, FILE *stream);
};

#endif // DOWNLOAD_HANDLER_H
