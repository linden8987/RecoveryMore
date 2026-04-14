#include <windows.h>
#include <commctrl.h>
#include <string>

// Global Variables
HINSTANCE hInst;
HCURSOR hCustomCursor = NULL;

// Function to update the cursor from a file path
void UpdateCursor(HWND hwnd, std::string path) {
    hCustomCursor = (HCURSOR)LoadImageA(NULL, path.c_str(), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
    if (hCustomCursor) {
        SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)hCustomCursor);
        SetCursor(hCustomCursor);
    }
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // Lead Architect Theme: Dark Green Gradient simulation
            HBRUSH brush = CreateSolidBrush(RGB(0, 30, 0));
            FillRect(hdc, &ps.rcPaint, brush);
            DeleteObject(brush);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_CREATE: {
            // Add a simple button to simulate "Open Browser"
            CreateWindow("BUTTON", "Launch Browser", WS_VISIBLE | WS_CHILD, 20, 20, 150, 40, hwnd, (HMENU)1, NULL, NULL);
            // Add a button for "File Explorer"
            CreateWindow("BUTTON", "File Explorer", WS_VISIBLE | WS_CHILD, 20, 80, 150, 40, hwnd, (HMENU)2, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wp) == 1) MessageBox(hwnd, "Browser Module Loading...", "RecoveryMore", MB_OK);
            if (LOWORD(wp) == 2) MessageBox(hwnd, "Explorer Module Loading...", "RecoveryMore", MB_OK);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE p, LPSTR lp, int n) {
    hInst = h;
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = h;
    wc.lpszClassName = "RM_GUI";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("RM_GUI", "RecoveryMore - Lead Architect Edition", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1024, 768, NULL, NULL, h, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
