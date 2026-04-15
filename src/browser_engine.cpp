// browser_engine.cpp

#include <iostream>
#include <vector>

class Tab {
public:
    Tab(const std::string& url) : url(url) {}
    void navigate(const std::string& newUrl) { url = newUrl; }
    std::string getUrl() const { return url; }
private:
    std::string url;
};

class Download {
public:
    Download(const std::string& file) : filename(file) {}
    void start() { std::cout << "Downloading: " << filename << std::endl; }
private:
    std::string filename;
};

class BrowserEngine {
public:
    void openTab(const std::string& url) {
        tabs.push_back(Tab(url));
    }
    void downloadFile(const std::string& file) {
        Download download(file);
        download.start();
    }
private:
    std::vector<Tab> tabs;
};

int main() {
    BrowserEngine browser;
    browser.openTab("https://www.example.com");
    browser.downloadFile("example_file.txt");
    return 0;
}
