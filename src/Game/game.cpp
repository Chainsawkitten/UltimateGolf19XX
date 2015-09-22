#include <GL/glew.h>
#include "game.hpp"
#include <GLFW/glfw3.h>
#include <Core/GameWindow.hpp>

int main() {
    if (!glfwInit())
        return 1;

    GameWindow* gameWindow = new GameWindow();
    glewInit();
    gameWindow->Init();

    while (!gameWindow->ShouldClose()) {
        gameWindow->Update();
        gameWindow->Render();
        glfwPollEvents();
    }

    delete gameWindow;

    glfwTerminate();

    return 0;
}
