#include "GameSettings.hpp"

#include <Core/Util/FileSystem.hpp>

GameSettings::GameSettings() {
    ini.SetUnicode(true);
    ini.LoadFile(FileSystem::SavePath("UltimateGolf19XX", "GameSettings.ini").c_str());
    
    AddBoolSetting("Logging", "Debug", "Logging", false);
    AddBoolSetting("Debug Context", "Debug", "Debug Context", false);
    
    AddDoubleSetting("Mouse Sensitivity", "Controls", "Mouse Sensitivity", 0.5);
    AddBoolSetting("Center Mouse Cursor", "Controls", "Center Mouse Cursor", true);
}

GameSettings& GameSettings::GetInstance() {
    static GameSettings settings;
    
    return settings;
}

void GameSettings::Save() {
    ini.SaveFile(FileSystem::SavePath("UltimateGolf19XX", "GameSettings.ini").c_str());
}
