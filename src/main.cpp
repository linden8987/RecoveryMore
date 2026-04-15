#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <shellapi.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <thread>
#include <wrl.h>
#include <WebView2.h>

namespace fs = std::filesystem;
using namespace Microsoft::WRL;

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "shell32.lib")

// --- 1. MAINTENANCE & PROTECTION ---
void PerformMaintenance() {
    // UPDATER: Swaps RecoveryMore_New.exe into place
    if (fs::exists("RecoveryMore_New.exe")) {
        std::ofstream batch("update.bat");
        batch << "@echo off\ntimeout /t 1 /nobreak > nul\ndel RecoveryMore.exe\n"
              << "move RecoveryMore_New.exe RecoveryMore.exe\nstart RecoveryMore.exe\ndel \"%~f0\"";
        batch.close();
        ShellExecuteW(NULL, L"open", L"update.bat", NULL, NULL, SW_HIDE);
        exit(0);
    }

    // CLEANER: Wipes everything EXCEPT assets, bin, and userdata
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    fs::path exePath(szPath);
    try {
        for (const auto& entry : fs::directory_iterator(exePath.parent_path())) {
            std::wstring name = entry.path().filename().wstring();
            if (entry.path() != exePath && name != L"bin" && name != L"userdata" && 
                name != L"assets" && name != L"update.bat") {
                fs::remove_all(entry.path());
            }
        }
    } catch (...) {}
}

// --- 2. THE IPAD BRIDGE (Server) ---
void StartIPadBridge() {
    std::thread([]() {
        WSADATA wsa; WSAStartup(MAKEWORD(2, 2), &wsa);
        SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr = { AF_INET, htons(8080), INADDR_ANY };
        bind(s, (sockaddr*)&addr, sizeof(addr));
        listen(s, 3);
        // This accepts file chunks from your iPad browser/app
    }).detach();
}

// --- 3. THE BROWSER ENGINE (WebView2) ---
void InitBrowser(HWND hWnd) {
    auto userData = fs::current_path() / L"userdata";
    CreateCoreWebView2EnvironmentWithOptions(nullptr, userData.c_str(), nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hWnd](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
                env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [hWnd](HRESULT res, ICoreWebView2Controller* ctrl) -> HRESULT {
                        if (ctrl) {
                            ICoreWebView2* webview;
                            ctrl->get_CoreWebView2(&webview);
                            RECT bounds; GetClientRect(hWnd, &bounds);
                            ctrl->put_Bounds(bounds);
                            // Point this to your local asset or a server URL
                            webview->Navigate(L"google.com"); 
                        }
                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());
}

// --- WINDOWS BOILERPLATE ---
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    PerformMaintenance();
    StartIPadBridge();

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"RecoveryMoreClass";
    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(0, L"RecoveryMoreClass", L"RecoveryMore", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInst, NULL);

    if (!hWnd) return 0;

    ShowWindow(hWnd, nShow);
    InitBrowser(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
