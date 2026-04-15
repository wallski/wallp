#include <Windows.h>
#include "app.h"
#include "../ext/imgui/imgui_impl_win32.h"

static App app;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, w, l))
        return true;

    if (msg == WM_SIZE && w != SIZE_MINIMIZED)
        app.OnResize(LOWORD(l), HIWORD(l));

    if (msg == WM_DESTROY)
        PostQuitMessage(0);

    return DefWindowProc(hWnd, msg, w, l);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0,0,hInst,0,0,0,0,L"wp",0 };
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, L"Wallpaper Manager",
        WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, 0, 0, wc.hInstance, 0);

    if (!app.Init(hwnd)) return 1;

    ShowWindow(hwnd, SW_SHOWDEFAULT);

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }
        app.Update();
    }

    app.Cleanup();
    DestroyWindow(hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}