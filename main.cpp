#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h" // The Chromium Engine Header

using namespace Microsoft::WRL;

// Global Chromium Controller
static Pointer<ICoreWebView2Controller> webviewController;
static Pointer<ICoreWebView2> webview;

// Function to Initialize the Chromium Browser
void InitChromium(HWND hWnd) {
    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hWnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [hWnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                        if (controller != nullptr) {
                            webviewController = controller;
                            webviewController->get_CoreWebView2(&webview);
                        }
                        // Set the Chromium window size
                        RECT bounds;
                        GetClientRect(hWnd, &bounds);
                        bounds.top += 250; // Leave room for your Green Dashboard buttons
                        webviewController->put_Bounds(bounds);

                        // Navigate to your Wiki or Google
                        webview->Navigate(L"https://www.google.com");
                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());
}

// ... rest of your WndProc with button 3 calling InitChromium(hwnd) ...
