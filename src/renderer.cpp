#include "renderer.h"
#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_win32.h"
#include "../ext/imgui/imgui_impl_dx11.h"

bool Renderer::Init(HWND hwnd) {
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    D3D_FEATURE_LEVEL level;
    const D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

    if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        levels, 2, D3D11_SDK_VERSION, &sd, &swapChain, &device, &level, &context) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void Renderer::CreateRenderTarget() {
    ID3D11Texture2D* bb = nullptr;
    swapChain->GetBuffer(0, IID_PPV_ARGS(&bb));
    device->CreateRenderTargetView(bb, nullptr, &rtv);
    bb->Release();
}

void Renderer::CleanupRenderTarget() {
    if (rtv) { rtv->Release(); rtv = nullptr; }
}

void Renderer::Resize(UINT width, UINT height) {
    if (!swapChain) return;
    CleanupRenderTarget();
    swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    CreateRenderTarget();
}

void Renderer::BeginFrame() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void Renderer::EndFrame() {
    ImGui::Render();
    float c[4] = { 0.08f, 0.08f, 0.12f, 1.0f };
    context->OMSetRenderTargets(1, &rtv, nullptr);
    context->ClearRenderTargetView(rtv, c);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    swapChain->Present(1, 0);
}

void Renderer::Cleanup() {
    CleanupRenderTarget();
    if (swapChain) swapChain->Release();
    if (context) context->Release();
    if (device) device->Release();
}

ID3D11Device* Renderer::GetDevice() { return device; }
ID3D11DeviceContext* Renderer::GetContext() { return context; }