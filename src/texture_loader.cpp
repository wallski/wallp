#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb_image.h"
#include "texture_loader.h"

ID3D11ShaderResourceView* LoadTextureFromFile(
    ID3D11Device* device,
    const std::string& filename
) {
    int w, h, channels;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &channels, 4);

    if (!data) return nullptr;

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = w;
    desc.Height = h;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA sub = {};
    sub.pSysMem = data;
    sub.SysMemPitch = w * 4;

    ID3D11Texture2D* tex = nullptr;
    device->CreateTexture2D(&desc, &sub, &tex);

    ID3D11ShaderResourceView* srv = nullptr;
    device->CreateShaderResourceView(tex, nullptr, &srv);

    tex->Release();
    stbi_image_free(data);

    return srv;
}