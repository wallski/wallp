# Wallp

A lightweight Windows wallpaper manager built with C++, DirectX 11, and Dear ImGui.

## Overview

Wallp is a simple desktop application that allows you to browse, preview, and apply wallpapers from a selected folder. It provides a minimal UI with a focus on performance and simplicity.

## Features

- Browse wallpapers from a user-selected directory
- Preview wallpapers inside the application
- Apply wallpapers directly to the Windows desktop
- Persistent configuration stored in AppData
- Custom ImGui-based UI with DirectX 11 rendering
- Clean separation between rendering, UI, and settings logic

## Architecture

The project is structured into modular components:

- Renderer: DirectX 11 initialization and frame management
- App: Main application logic and UI layout
- WallpaperManager: Loads and manages wallpaper files
- SettingsManager: Handles persistent configuration storage
- Texture Loader: Converts image files into GPU textures for preview

## Configuration

Wallp stores configuration data in:
