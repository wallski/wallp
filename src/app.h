#pragma once
#include "renderer.h"
#include "wallpaper_manager.h"
#include "settings.h"
#include "settings_manager.h"
#include <vector>
#include <d3d11.h>

class App {
public:
    bool Init(HWND hwnd);
    void Update();
    void Cleanup();
    void OnResize(UINT w, UINT h);

private:
    void LoadTextures();

    Renderer renderer;
    WallpaperManager manager;
    Settings settings;
    SettingsManager settingsMgr;
    std::vector<ID3D11ShaderResourceView*> textures;
    int selected = -1;
};