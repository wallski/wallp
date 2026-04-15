#include "wallpaper_manager.h"
#include <filesystem>
#include <Windows.h>

void WallpaperManager::LoadFromFolder(const std::string& folder) {
    wallpapers.clear();

    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        auto ext = entry.path().extension().string();

        if (ext == ".jpg" || ext == ".png" || ext == ".jpeg") {
            wallpapers.push_back({ entry.path().string() });
        }
    }
}

const std::vector<Wallpaper>& WallpaperManager::GetWallpapers() const {
    return wallpapers;
}

bool WallpaperManager::SetWallpaper(const std::string& path) {
    return SystemParametersInfoA(
        SPI_SETDESKWALLPAPER,
        0,
        (PVOID)path.c_str(),
        SPIF_UPDATEINIFILE | SPIF_SENDCHANGE
    );
}