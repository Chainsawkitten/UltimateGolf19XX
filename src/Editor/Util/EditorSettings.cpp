#include "EditorSettings.hpp"

#include <Core/Util/FileSystem.hpp>

EditorSettings::EditorSettings() {
    ini.SetUnicode(true);
    ini.LoadFile(FileSystem::SavePath("UltimateGolf19XX", "EditorSettings.ini").c_str());
    
    AddBoolSetting("Logging", "Debug", "Logging", false);
    AddBoolSetting("Debug Context", "Debug", "Debug Context", false);
}

EditorSettings& EditorSettings::GetInstance() {
    static EditorSettings settings;
    
    return settings;
}

void EditorSettings::Save() {
    ini.SaveFile(FileSystem::SavePath("UltimateGolf19XX", "EditorSettings.ini").c_str());
}
