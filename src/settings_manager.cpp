#include "settings_manager.h"
#include <fstream>
#include <windows.h>
#include <shlobj.h>

static std::string GetConfigPath() {
    char path[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path);
    std::string dir = std::string(path) + "\\Wallp";
    CreateDirectoryA(dir.c_str(), NULL);
    return dir + "\\config.cfg";
}

bool SettingsManager::Load(Settings& out) {
    std::ifstream f(GetConfigPath());
    if (!f.is_open()) return false;
    std::getline(f, out.wallpaperFolder);
    return true;
}

bool SettingsManager::Save(const Settings& in) {
    std::ofstream f(GetConfigPath());
    if (!f.is_open()) return false;
    f << in.wallpaperFolder;
    return true;
}