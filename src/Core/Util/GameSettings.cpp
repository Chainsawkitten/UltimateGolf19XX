#include "GameSettings.hpp"

#include <Core/Util/FileSystem.hpp>

GameSettings::GameSettings() {
    ini.SetUnicode(true);
    ini.LoadFile(FileSystem::SavePath("UltimateGolf19XX", "GameSettings.ini").c_str());
    
    AddBoolSetting("Logging", "Debug", "Logging", false);
    AddBoolSetting("Debug Context", "Debug", "Debug Context", false);
    AddBoolSetting("Show Frame Times", "Debug", "Show Frame Times", false);
	AddLongSetting("Players", "Debug", "Number of players", 2);
    AddDoubleSetting("Mouse Sensitivity", "Controls", "Mouse Sensitivity", 0.5);
    AddBoolSetting("Center Mouse Cursor", "Controls", "Center Mouse Cursor", true);
    AddBoolSetting("Show Mouse Cursor", "Controls", "Show Mouse Cursor", false);
    
    AddLongSetting("Screen Width", "Graphics", "Width", 800);
	AddLongSetting("Screen Height", "Graphics", "Height", 600);
    AddBoolSetting("Fullscreen", "Graphics", "Fullscreen", false);
    AddBoolSetting("Borderless", "Graphics", "Borderless Window", false);
    AddLongSetting("Target FPS", "Graphics", "Target FPS", 60);
    AddBoolSetting("FXAA", "Graphics", "FXAA", true);
    
    AddDoubleSetting("Audio Volume", "Sound", "Volume", 1.0);
}

GameSettings& GameSettings::GetInstance() {
    static GameSettings settings;
    
    return settings;
}

void GameSettings::Save() {
    ini.SaveFile(FileSystem::SavePath("UltimateGolf19XX", "GameSettings.ini").c_str());
}
