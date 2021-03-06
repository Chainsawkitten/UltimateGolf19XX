#include "editor.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "EditorWindow.hpp"
#include "Util/EditorSettings.hpp"
#include <Core/Util/FileSystem.hpp>
#include <Core/Util/Log.hpp>

int main() {
    // Enable logging if requested.
    if (EditorSettings::GetInstance().GetBool("Logging"))
        freopen(FileSystem::SavePath("UltimateGolf19XX", "EditorLog.txt").c_str(), "a", stderr);
    
    Log() << "Editor started - " << time(nullptr) << "\n";
    
    if (!glfwInit())
        return 1;
    
    if (EditorSettings::GetInstance().GetBool("Debug Context"))
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    EditorWindow* editorWindow = new EditorWindow();
    glewInit();
    
    // Setup error callbacks.
    glfwSetErrorCallback(ErrorCallback);
    if (EditorSettings::GetInstance().GetBool("Debug Context"))
        glDebugMessageCallback(DebugMessageCallback, nullptr);

    editorWindow->Init();
    
    while (!editorWindow->ShouldClose()) {
        editorWindow->Update();
        editorWindow->Render();
        glfwPollEvents();
    }

    delete editorWindow;

    glfwTerminate();
    
    EditorSettings::GetInstance().Save();
    
    Log() << "Editor ended - " << time(nullptr) << "\n";

    return 0;
}
