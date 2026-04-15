// download_handler.cpp

#include <iostream>
#include <string>

class DownloadHandler {
public:
    void startDownload(const std::string& url) {
        std::cout << "Starting download from " << url << std::endl;
        // Implement download logic here
    }

    void cancelDownload() {
        std::cout << "Download cancelled" << std::endl;
        // Implement cancellation logic here
    }
};

int main() {
    DownloadHandler handler;
    handler.startDownload("http://example.com/file.zip");
    return 0;
}