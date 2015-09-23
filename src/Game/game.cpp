#include <GL/glew.h>
#include "game.hpp"
#include <GLFW/glfw3.h>
#include <Core/GameWindow.hpp>
#include <Core/Util/GameSettings.hpp>
#include <Core/Util/FileSystem.hpp>
#include <Core/Util/Log.hpp>

int main() {
    // Enable logging if requested.
    if (GameSettings::GetInstance().GetBool("Logging"))
        freopen(FileSystem::SavePath("UltimateGolf19XX", "GameLog.txt").c_str(), "a", stderr);
    
    Log() << "Game started - " << time(nullptr) << "\n";
    
    if (!glfwInit())
        return 1;
    
    if (GameSettings::GetInstance().GetBool("Debug Context"))
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    
    GameWindow* gameWindow = new GameWindow();
    glewInit();
    
    // Setup error callbacks.
    glfwSetErrorCallback(ErrorCallback);
    if (GameSettings::GetInstance().GetBool("Debug Context"))
        glDebugMessageCallback(DebugMessageCallback, nullptr);
    
    gameWindow->Init();

    while (!gameWindow->ShouldClose()) {
        gameWindow->Update();
        gameWindow->Render();
        glfwPollEvents();
    }

    delete gameWindow;

    glfwTerminate();
    
    GameSettings::GetInstance().Save();
    
    Log() << "Game ended - " << time(nullptr) << "\n";

    return 0;
}
