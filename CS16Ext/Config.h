#pragma once
#include <Windows.h>
#include <fstream>
#include <vector>
#include <string>
#include "Settings.h"
#include "INIReader.h"

static std::vector<std::string> SettingsList;
static bool IsConfigSelected[50];
static int SelectedConfig = 0;
static char cfgname[25] = "";

inline bool IsFileExists(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void getFilesList(std::string filePath, std::string extension, std::vector<std::string>& returnFileName)
{
	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	std::string  fullPath = filePath + extension;
	hFind = FindFirstFile(fullPath.c_str(), &fileInfo);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		returnFileName.push_back(filePath + fileInfo.cFileName);
		while (FindNextFile(hFind, &fileInfo) != 0)
		{
			returnFileName.push_back(filePath + fileInfo.cFileName);
		}
	}
}

void RefreshSettings()
{
	memset(&SettingsList, 0, sizeof(SettingsList));

	std::string configDir = std::string(getenv("appdata")) + std::string("\\INTERIUM\\CS16Ext\\");

	std::vector<std::string> filesPaths;
	getFilesList(configDir, "*.ini*", filesPaths);
	std::vector<std::string>::const_iterator it = filesPaths.begin();
	while (it != filesPaths.end())
	{
		std::string outpath = it->c_str();
		SettingsList.push_back(outpath);
		//remove(it->c_str());
		it++;
	}
}

void SaveConfig(std::string ConfigName)
{
	std::ofstream ofs(ConfigName);

	ofs << "[AIMBOT]\n";

	ofs << "Enabled = " << Aimbot::Enabled << "\n";
	ofs << "Deathmatch = " << Aimbot::Deathmatch << "\n";
	ofs << "SquareFov = " << Aimbot::SquareFov << "\n";
	ofs << "DrawFov = " << Aimbot::DrawFov << "\n";
	ofs << "DrawFovColorR = " << Aimbot::DrawFovColor[0] << "\n";
	ofs << "DrawFovColorG = " << Aimbot::DrawFovColor[1] << "\n";
	ofs << "DrawFovColorB = " << Aimbot::DrawFovColor[2] << "\n";
	ofs << "DrawFovColorA = " << Aimbot::DrawFovColor[3] << "\n";
	ofs << "DrawFovWidth = " << Aimbot::DrawFovWidth << "\n";

	for (int i = 0; i < 40; i++)
	{
		ofs << "WeaponID" << std::to_string(i) << "Enable = " << Weapons[i].Enabled << "\n";
		ofs << "WeaponID" << std::to_string(i) << "FOV = " << Weapons[i].FOV << "\n";
		ofs << "WeaponID" << std::to_string(i) << "Smootch = " << Weapons[i].Smootch << "\n";
		ofs << "WeaponID" << std::to_string(i) << "RCS = " << Weapons[i].RCS << "\n";
	}

	ofs << "[ESP]\n";

	ofs << "Box = " << ESP::Box << "\n";
	ofs << "BoxType = " << ESP::BoxType << "\n";
	ofs << "BoxColorR = " << ESP::BoxColor[0] << "\n";
	ofs << "BoxColorG = " << ESP::BoxColor[1] << "\n";
	ofs << "BoxColorB = " << ESP::BoxColor[2] << "\n";
	ofs << "BoxColorA = " << ESP::BoxColor[3] << "\n";
	ofs << "BoxRounding = " << ESP::BoxRounding << "\n";
	ofs << "BoxWidth = " << ESP::BoxWidth << "\n";

	ofs << "Dist = " << ESP::Dist << "\n";
	ofs << "DistColorR = " << ESP::DistColor[0] << "\n";
	ofs << "DistColorG = " << ESP::DistColor[1] << "\n";
	ofs << "DistColorB = " << ESP::DistColor[2] << "\n";
	ofs << "DistColorA = " << ESP::DistColor[3] << "\n";
	ofs << "Names = " << ESP::Names << "\n";
	ofs << "NamesColorR = " << ESP::NamesColor[0] << "\n";
	ofs << "NamesColorG = " << ESP::NamesColor[1] << "\n";
	ofs << "NamesColorB = " << ESP::NamesColor[2] << "\n";
	ofs << "NamesColorA = " << ESP::NamesColor[3] << "\n";

	ofs << "Crosshair = " << ESP::Crosshair << "\n";
	ofs << "CrosshairColorR = " << ESP::CrosshairColor[0] << "\n";
	ofs << "CrosshairColorG = " << ESP::CrosshairColor[1] << "\n";
	ofs << "CrosshairColorB = " << ESP::CrosshairColor[2] << "\n";
	ofs << "CrosshairColorA = " << ESP::CrosshairColor[3] << "\n";
	ofs << "CrosshairSize = " << ESP::CrosshairSize << "\n";
	ofs << "CrosshairWidth = " << ESP::CrosshairWidth << "\n";

	ofs << "[MISC]\n";

	ofs << "Bhop = " << MISC::Bhop << "\n";
	ofs << "DDrun = " << MISC::DDrun << "\n";
	ofs << "AutoPistol = " << MISC::AutoPistol << "\n";
	ofs << "FpsUnlock = " << MISC::FpsUnlock << "\n";

	ofs << "[KEYS]\n";

	ofs << "AimbotKey1 = " << KEYS::AimbotKey1 << "\n";
	ofs << "AimbotKey2 = " << KEYS::AimbotKey2 << "\n";
	ofs << "BhopKey = " << KEYS::BhopKey << "\n";
	ofs << "DDrunKey = " << KEYS::DDrunKey << "\n";
	ofs << "MenuKey = " << KEYS::MenuKey << "\n";

	ofs.close();
}

bool LoadConfig(std::string ConfigName)
{
	if (IsFileExists(ConfigName))
	{
		INIReader reader(ConfigName);
		if (reader.ParseError() == 0)
		{
			Aimbot::Enabled = reader.GetBoolean("AIMBOT", "Enabled", true);
			Aimbot::Deathmatch = reader.GetBoolean("AIMBOT", "Deathmatch", true);
			Aimbot::SquareFov = reader.GetBoolean("AIMBOT", "SquareFov", true);
			Aimbot::DrawFov = reader.GetBoolean("AIMBOT", "DrawFov", true);
			Aimbot::DrawFovColor[0] = reader.GetReal("AIMBOT", "DrawFovColorR", 1.f);
			Aimbot::DrawFovColor[1] = reader.GetReal("AIMBOT", "DrawFovColorG", 1.f);
			Aimbot::DrawFovColor[2] = reader.GetReal("AIMBOT", "DrawFovColorB", 1.f);
			Aimbot::DrawFovColor[3] = reader.GetReal("AIMBOT", "DrawFovColorA", 1.f);
			Aimbot::DrawFovWidth = reader.GetReal("AIMBOT", "DrawFovWidth", 1.f);

			for (int i = 0; i < 40; i++)
			{
				Weapons[i].Enabled = reader.GetBoolean("AIMBOT", "WeaponID"+ std::to_string(i) +"Enable", false);
				Weapons[i].FOV = reader.GetReal("AIMBOT", "WeaponID" + std::to_string(i) + "FOV", 12.5f);
				Weapons[i].Smootch = reader.GetReal("AIMBOT", "WeaponID" + std::to_string(i) + "Smootch", 5.f);
				Weapons[i].RCS = reader.GetReal("AIMBOT", "WeaponID" + std::to_string(i) + "RCS", 4.f);
			}

			ESP::Box = reader.GetBoolean("ESP", "Box", false);
			ESP::BoxType = reader.GetInteger("ESP", "BoxType", 0);
			ESP::BoxColor[0] = reader.GetReal("ESP", "BoxColorR", 1.f);
			ESP::BoxColor[1] = reader.GetReal("ESP", "BoxColorG", 1.f);
			ESP::BoxColor[2] = reader.GetReal("ESP", "BoxColorB", 1.f);
			ESP::BoxColor[3] = reader.GetReal("ESP", "BoxColorA", 1.f);
			ESP::BoxRounding = reader.GetReal("ESP", "BoxRounding", 1.f);
			ESP::BoxWidth = reader.GetReal("ESP", "BoxWidth", 1.f);

			ESP::Dist = reader.GetReal("ESP", "Dist", 1.f);
			ESP::DistColor[0] = reader.GetReal("ESP", "DistColorR", 1.f);
			ESP::DistColor[1] = reader.GetReal("ESP", "DistColorG", 1.f);
			ESP::DistColor[2] = reader.GetReal("ESP", "DistColorB", 1.f);
			ESP::DistColor[3] = reader.GetReal("ESP", "DistColorA", 1.f);

			ESP::Names = reader.GetReal("ESP", "Names", 1.f);
			ESP::NamesColor[0] = reader.GetReal("ESP", "NamesColorR", 1.f);
			ESP::NamesColor[1] = reader.GetReal("ESP", "NamesColorG", 1.f);
			ESP::NamesColor[2] = reader.GetReal("ESP", "NamesColorB", 1.f);
			ESP::NamesColor[3] = reader.GetReal("ESP", "NamesColorA", 1.f);

			ESP::Crosshair = reader.GetReal("ESP", "Crosshair", 1.f);
			ESP::CrosshairColor[0] = reader.GetReal("ESP", "CrosshairColorR", 1.f);
			ESP::CrosshairColor[1] = reader.GetReal("ESP", "CrosshairColorG", 1.f);
			ESP::CrosshairColor[2] = reader.GetReal("ESP", "CrosshairColorB", 1.f);
			ESP::CrosshairColor[3] = reader.GetReal("ESP", "CrosshairColorA", 1.f);
			ESP::CrosshairSize = reader.GetReal("ESP", "CrosshairSize", 5.f);
			ESP::CrosshairWidth = reader.GetReal("ESP", "CrosshairWidth", 1.f);

			MISC::Bhop = reader.GetBoolean("MISC", "Bhop", false);
			MISC::DDrun = reader.GetBoolean("MISC", "DDrun", false);
			MISC::AutoPistol = reader.GetBoolean("MISC", "AutoPistol", false);
			MISC::FpsUnlock = reader.GetBoolean("MISC", "FpsUnlock", false);

			KEYS::AimbotKey1 = reader.GetInteger("KEYS", "AimbotKey1", VK_LBUTTON);
			KEYS::AimbotKey2 = reader.GetInteger("KEYS", "AimbotKey2", 0x56);
			KEYS::BhopKey = reader.GetInteger("KEYS", "BhopKey", VK_SPACE);
			KEYS::DDrunKey = reader.GetInteger("KEYS", "DDrunKey", VK_MENU);
			KEYS::MenuKey = reader.GetInteger("KEYS", "MenuKey", VK_INSERT);
			return true;
		}
	}
	return false;
}



