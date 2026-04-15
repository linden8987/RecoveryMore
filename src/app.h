// app.h - CEF Application Class Definition

#ifndef APP_H
#define APP_H

#include <include/cef_app.h>

class MyApp : public CefApp {
public:
    void OnBeforeCommandLineProcessing(const CefString& process_type, CefCommandLine& command_line) override;
    void OnContextInitialized() override;
    // other methods for handling client requests and rendering
};

#endif // APP_H