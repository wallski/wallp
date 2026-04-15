#pragma once
#include <d3d11.h>
#include <Windows.h>

class Renderer {
public:
    bool Init(HWND hwnd);
    void BeginFrame();
    void EndFrame();
    void Cleanup();
    void Resize(UINT width, UINT height);

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetContext();

private:
    void CreateRenderTarget();
    void CleanupRenderTarget();

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    IDXGISwapChain* swapChain = nullptr;
    ID3D11RenderTargetView* rtv = nullptr;
};