#include <iostream>
#include <string>
#include <filesystem>
#include <Windows.h>
#include <vector>
#include <fstream>

bool running = true;

int main() {

	std::string wallpaperPath;

	if (std::filesystem::exists("config.cfg")) {
		std::ifstream file("config.cfg");
		std::getline(file, wallpaperPath);
		file.close();
		std::cout << "Loaded: " << wallpaperPath << '\n';
	}
	else {
		std::cout << "Welcome! Set your wallpaper folder:\n";

		std::getline(std::cin, wallpaperPath);


		while (!std::filesystem::exists(wallpaperPath)) {
			std::cout << "That folder doesn't exist! Try again:\n";
			std::getline(std::cin, wallpaperPath);
		}

		std::ofstream file("config.cfg");
		file << wallpaperPath;
		file.close();
	}

	std::vector<std::string> wallpapers;


	for (const auto& entry : std::filesystem::directory_iterator(wallpaperPath)) {
		std::string ext = entry.path().extension().string();

		if (ext == ".jpg" || ext == ".png" || ext == ".jpeg") {
			wallpapers.push_back(entry.path().string());
			std::cout << wallpapers.size() << ". " << entry.path().filename() << '\n';
		}
		

	}
	
	

	while (running) {
		int choice;

		std::cout << "\nPick the wallpaper number: ";
		std::cin >> choice;

		if (choice >= 1 && choice <= wallpapers.size()) {
			const char* imgPath = wallpapers[choice - 1].c_str();
			BOOL result = SystemParametersInfoA(
				0x0014,
				0,
				(PVOID)imgPath,
				0x01 | 0x02
			);
			if (result) {
				std::cout << "\nWallpaper set!";
			}
			else {
				std::cout << "\nFailed to set wallpaper!";
			}
		}
	}






	return 0;
}