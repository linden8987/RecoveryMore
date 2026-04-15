#include <windows.h>
#include <filesystem>
#include "app.h"
#include "browser_engine.h"
#include "client_handler.h"
#include "download_handler.h"

namespace fs = std::filesystem;

// Fixes the "Solitaire" ghosting trail in WinPE
void ClearGhosting(HWND hWnd) {
    InvalidateRect(hWnd, NULL, TRUE);
    UpdateWindow(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_MOVE:
        case WM_SIZE:
            ClearGhosting(hWnd); 
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
    // 1. Setup local folders on the F: drive
    try {
        fs::create_directories("userdata");
        fs::create_directories("downloads");
    } catch (...) {}

    // 2. Initialize CEF Application logic (from your app.cpp)
    CefMainArgs main_args(hInst);
    CefRefPtr<App> app(new App());

    int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
    if (exit_code >= 0) return exit_code;

    // 3. Configure CEF Settings
    CefSettings settings;
    settings.no_sandbox = true;
    std::wstring cache = fs::current_path().wstring() + L"/userdata";
    CefString(&settings.cache_path).FromWString(cache);

    // WINPE STABILITY: Disable GPU to prevent the blue frame glitch
    CefCommandLine::GetGlobalCommandLine()->AppendSwitch("disable-gpu");
    CefCommandLine::GetGlobalCommandLine()->AppendSwitch("disable-gpu-compositing");

    CefInitialize(main_args, settings, app.get(), nullptr);

    // 4. Create Main Window with the Gold Icon (ID 101)
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(101)); // Uses your icon.ico
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"RecoveryMoreMain";
    RegisterClassW(&wc);

    HWND hWnd = CreateWindowExW(0, L"RecoveryMoreMain", L"RecoveryMore Dashboard", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInst, NULL);

    ShowWindow(hWnd, nShow);

    // 5. Use all your classes: Engine, Client, and Download handlers
    BrowserEngine::GetInstance()->Init(hWnd, hInst);
    
    // Explicitly using ClientHandler to bridge with DownloadHandler
    CefWindowInfo window_info;
    window_info.SetAsChild(hWnd, {0, 0, 1280, 720});
    CefBrowserSettings browser_settings;
    CefRefPtr<ClientHandler> handler(new ClientHandler());
    
    CefBrowserHost::CreateBrowser(window_info, handler.get(), "https://www.google.com", browser_settings, nullptr, nullptr);

    CefRunMessageLoop();
    CefShutdown();

    return 0;
}
