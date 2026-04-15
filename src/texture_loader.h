#pragma once
#include <d3d11.h>
#include <string>

ID3D11ShaderResourceView* LoadTextureFromFile(
    ID3D11Device* device,
    const std::string& filename
);