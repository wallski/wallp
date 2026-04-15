#pragma once
#include <vector>
#include <string>

struct Wallpaper {
    std::string path;
};

class WallpaperManager {
public:
    void LoadFromFolder(const std::string& folder);
    const std::vector<Wallpaper>& GetWallpapers() const;

    bool SetWallpaper(const std::string& path);

private:
    std::vector<Wallpaper> wallpapers;
};