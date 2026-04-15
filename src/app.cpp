#include "app.h"
#include "texture_loader.h"
#include <filesystem>
#include <windows.h>
#include <shobjidl.h>
#include <combaseapi.h>
#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_win32.h"
#include "../ext/imgui/imgui_impl_dx11.h"
#include <ShlObj.h>

static std::string OpenFolderDialog(HWND hwnd) {
    std::string result;
    IFileDialog* dlg = nullptr;
    CoInitialize(nullptr);

    if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&dlg)))) {

        DWORD opt;
        dlg->GetOptions(&opt);
        dlg->SetOptions(opt | FOS_PICKFOLDERS);

        if (SUCCEEDED(dlg->Show(hwnd))) {
            IShellItem* item;
            if (SUCCEEDED(dlg->GetResult(&item))) {
                PWSTR path;
                if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path))) {
                    std::wstring ws(path);
                    result.assign(ws.begin(), ws.end());
                    CoTaskMemFree(path);
                }
                item->Release();
            }
        }

        dlg->Release();
    }

    CoUninitialize();
    return result;
}

bool App::Init(HWND hwnd) {
    if (!renderer.Init(hwnd)) return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    
    char path[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path);
    std::string appdata = std::string(path) + "\\Wallp";
    CreateDirectoryA(appdata.c_str(), NULL);


    CreateDirectoryA(appdata.c_str(), NULL);
    std::string ini = appdata + "\\imgui.ini";
    io.IniFilename = _strdup(ini.c_str());

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.FramePadding = ImVec2(12, 8);
    style.ItemSpacing = ImVec2(10, 10);
    style.WindowPadding = ImVec2(12, 12);
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;

    ImVec4* c = style.Colors;
    c[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.14f, 1.0f);
    c[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.16f, 1.0f);
    c[ImGuiCol_Button] = ImVec4(0.35f, 0.20f, 0.55f, 1.0f);
    c[ImGuiCol_ButtonHovered] = ImVec4(0.45f, 0.25f, 0.70f, 1.0f);
    c[ImGuiCol_ButtonActive] = ImVec4(0.55f, 0.30f, 0.85f, 1.0f);

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(renderer.GetDevice(), renderer.GetContext());

    settingsMgr.Load(settings);

    if (!settings.wallpaperFolder.empty()) {
        manager.LoadFromFolder(settings.wallpaperFolder);
        LoadTextures();
    }

    return true;
}

void App::LoadTextures() {
    textures.clear();
    for (auto& w : manager.GetWallpapers()) {
        textures.push_back(LoadTextureFromFile(renderer.GetDevice(), w.path));
    }
}

void App::Update() {
    renderer.BeginFrame();

    auto& wps = manager.GetWallpapers();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("main", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove);

    ImGui::BeginChild("left", ImVec2(260, 0), true);
    ImGui::Text("Wallpaper Manager");
    ImGui::Separator();
    ImGui::Text("Total: %d", (int)wps.size());

    if (ImGui::Button("Change Folder", ImVec2(-1, 40))) {
        std::string p = OpenFolderDialog(NULL);
        if (!p.empty()) {
            settings.wallpaperFolder = p;
            settingsMgr.Save(settings);
            manager.LoadFromFolder(p);
            LoadTextures();
            selected = -1;
        }
    }

    if (selected >= 0 && selected < wps.size()) {
        ImGui::Spacing();
        ImGui::TextWrapped("%s",
            std::filesystem::path(wps[selected].path).filename().string().c_str());

        if (ImGui::Button("Apply", ImVec2(-1, 40))) {
            manager.SetWallpaper(wps[selected].path);
        }
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("center", ImVec2(-300, 0), true);

    float size = 170.0f;
    int cols = (int)(ImGui::GetContentRegionAvail().x / (size + 16));
    if (cols < 1) cols = 1;

    ImGui::Columns(cols, 0, false);

    for (int i = 0; i < (int)wps.size(); i++) {
        if (i >= textures.size()) {
            ImGui::NextColumn();
            continue;
        }

        std::string id = "t_" + std::to_string(i);
        bool sel = selected == i;

        if (sel) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.40f, 0.20f, 0.65f, 1.0f));

        if (textures[i]) {
            if (ImGui::ImageButton(id.c_str(), (ImTextureID)textures[i], ImVec2(size, size * 0.6f))) {
                selected = i;
            }
        }

        if (sel) ImGui::PopStyleColor();

        ImGui::TextWrapped("%s",
            std::filesystem::path(wps[i].path).filename().string().c_str());

        ImGui::NextColumn();
    }

    ImGui::Columns(1);
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("right", ImVec2(300, 0), true);

    if (selected >= 0 && selected < wps.size() && selected < textures.size()) {
        ImGui::Text("Preview");
        if (textures[selected]) {
            ImGui::Image((ImTextureID)textures[selected], ImVec2(280, 160));
        }
    }

    ImGui::EndChild();

    ImGui::End();

    renderer.EndFrame();
}

void App::OnResize(UINT w, UINT h) {
    renderer.Resize(w, h);
}

void App::Cleanup() {
    for (auto t : textures) if (t) t->Release();
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    renderer.Cleanup();
}