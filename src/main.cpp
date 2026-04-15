#include <windows.h>
#include <filesystem>
#include "app.h"             
#include "browser_engine.h"  
#include "client_handler.h"  
#include "download_handler.h"

namespace fs = std::filesystem;

// Fixes the ghosting trail by forcing a repaint on movement
void CleanGhosting(HWND hWnd) {
    InvalidateRect(hWnd, NULL, TRUE);
    UpdateWindow(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_MOVE:
        case WM_SIZE:
            CleanGhosting(hWnd); 
            break;
        case WM_ERASEBKGND:
            return 1; 
        case WM_DESTROY:
            CefQuitMessageLoop();
            return 0;
    }
    return DefWindowProcW(hWnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    // 1. Mandatory Folder Creation
    try {
        fs::create_directories("userdata");
        fs::create_directories("downloads");
    } catch (...) {}

    // 2. Initialize CEF Application logic (from app.cpp)
    CefMainArgs main_args(hInst);
    CefRefPtr<App> app(new App());

    // Execute sub-processes (Mandatory for CEF multi-process architecture)
    int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
    if (exit_code >= 0) return exit_code;

    // 3. Detailed CEF Settings
    CefSettings settings;
    settings.no_sandbox = true;
    settings.windowless_rendering_enabled = false;
    
    std::wstring cache = fs::current_path().wstring() + L"/userdata";
    CefString(&settings.cache_path).FromWString(cache);

    // WINPE STABILITY: Disable GPU to prevent the "Frozen Frame" issue
    CefCommandLine::GetGlobalCommandLine()->AppendSwitch("disable-gpu");
    CefCommandLine::GetGlobalCommandLine()->AppendSwitch("disable-gpu-compositing");
    CefCommandLine::GetGlobalCommandLine()->AppendSwitch("enable-begin-frame-scheduling");

    CefInitialize(main_args, settings, app.get(), nullptr);

    // 4. Create Host Window
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(101)); 
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"RecoveryMoreMain";
    RegisterClassW(&wc);

    HWND hWnd = CreateWindowExW(0, L"RecoveryMoreMain", L"RecoveryMore", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInst, NULL);

    ShowWindow(hWnd, nShow);

    // 5. Integrate BrowserEngine & ClientHandler
    // This part ensures your client_handler.cpp is actually managing the browser
    CefWindowInfo window_info;
    window_info.SetAsChild(hWnd, {0, 0, 1280, 720});

    CefBrowserSettings browser_settings;
    
    // Create the browser using your custom ClientHandler (which links to DownloadHandler)
    CefRefPtr<ClientHandler> handler(new ClientHandler());
    CefBrowserHost::CreateBrowser(window_info, handler.get(), "https://www.google.com", browser_settings, nullptr, nullptr);

    // Link the engine to this main window
    BrowserEngine::GetInstance()->Init(hWnd, hInst);

    // Start the Chromium loop
    CefRunMessageLoop();
    CefShutdown();

    return 0;
}
