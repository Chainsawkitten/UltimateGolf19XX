#pragma once

#include "Settings.hpp"

/// %Settings for the game.
/**
 * Available settings:
 * Name                | Description                         | Type   | Default Value
 * ------------------- | ----------------------------------- | ------ | -------------
 * Logging             | Output a log file.                  | bool   | false
 * Debug Context       | Create an OpenGL Debug Context.     | bool   | false
 * Show Frame Times    | Display update and render times.    | bool   | false
 * Mouse Sensitivity   | Mouse sensitivity.                  | double | 0.5
 * Center Mouse Cursor | Center the mouse cursor each frame. | bool   | true
 * Show Mouse Cursor   | Whether to show or hide the cursor. | bool   | false
 * Screen Width        | Width of the game window.           | long   | 800
 * Screen Height       | Height of the game window.          | long   | 600
 * Players		       | Number of players in the game       | long   | 2
 * Fullscreen          | Play in fullscreen mode.            | bool   | false
 * Borderless          | Use a borderless window.            | bool   | false
 * Target FPS          | FPS to aim for when rendering.      | long   | 60
 * FXAA                | Whether to use FXAA.                | bool   | true
 * Audio Volume        | How loud to play sounds.            | double | 1.0
 */
class GameSettings : public Settings {
    public:
        /// Get the instance of the class.
        /**
         * @return The %EditorSettings instance
         */
        static GameSettings& GetInstance();
        
        /// Save the settings to an INI file.
        void Save();
        
    private:
        GameSettings();
        
        GameSettings(GameSettings const&);
        void operator=(GameSettings const&);
};
