#include <windows.h>
#include <shlobj.h>
#include <string>
#include <wrl.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

static ComPtr<ICoreWebView2> webviewWindow;
HCURSOR hCustomCursor = NULL;

void OpenCustomExplorer(HWND hwnd) {
    IFileOpenDialog* pFileOpen;
    if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen)))) {
        DWORD dwOptions;
        pFileOpen->GetOptions(&dwOptions);
        pFileOpen->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
        pFileOpen->SetTitle(L"RecoveryMore | Internal Explorer");
        pFileOpen->Show(hwnd);
        pFileOpen->Release();
    }
}

void ApplyThemeCursor(HWND hwnd) {
    char szFile[MAX_PATH] = {0};
    OPENFILENAMEA ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Cursor Files (*.cur;*.ani)\0*.cur;*.ani\0All Files\0*.*\0";
    if (GetOpenFileNameA(&ofn)) {
        hCustomCursor = (HCURSOR)LoadImageA(NULL, szFile, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
        if (hCustomCursor) {
            SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)hCustomCursor);
            SetCursor(hCustomCursor);
        }
    }
}

void StartChromium(HWND hWnd) {
    // Pointing to current directory for the bundled engine
    CreateCoreWebView2EnvironmentWithOptions(L".", nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hWnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (FAILED(result)) return result;
                env->CreateCoreWebView2Controller(hWnd, 
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [hWnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                        if (controller != nullptr) {
                            ComPtr<ICoreWebView2Controller> webviewController = controller;
                            webviewController->get_CoreWebView2(&webviewWindow);
                            RECT bounds;
                            GetClientRect(hWnd, &bounds);
                            bounds.top += 150; 
                            webviewController->put_Bounds(bounds);
                            webviewWindow->Navigate(L"https://github.com/linden8987/RecoveryMore");
                        }
                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            CreateWindow("BUTTON", "📂 Internal Explorer", WS_VISIBLE | WS_CHILD, 20, 20, 160, 40, hwnd, (HMENU)1, NULL, NULL);
            CreateWindow("BUTTON", "🖱️ Apply Cursor", WS_VISIBLE | WS_CHILD, 20, 70, 160, 40, hwnd, (HMENU)2, NULL, NULL);
            StartChromium(hwnd);
            break;
        case WM_COMMAND:
            if (LOWORD(wp) == 1) OpenCustomExplorer(hwnd);
            if (LOWORD(wp) == 2) ApplyThemeCursor(hwnd);
            break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HBRUSH brush = CreateSolidBrush(RGB(0, 40, 0)); 
            FillRect(hdc, &ps.rcPaint, brush);
            DeleteObject(brush);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY: PostQuitMessage(0); break;
        default: return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE p, LPSTR lp, int n) {
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = h;
    wc.lpszClassName = "RecoveryMoreGUI";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("RecoveryMoreGUI", "RecoveryMore", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1200, 800, NULL, NULL, h, NULL);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    CoUninitialize();
    return 0;
}
