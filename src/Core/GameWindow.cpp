#include "GameWindow.hpp"

GameWindow::GameWindow() {
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(640, 480, "Hymn to Beauty", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        /// @todo Print error to log.
    }

    glfwMakeContextCurrent(window);
}

GameWindow::~GameWindow() {
    delete skybox;
    delete skyboxTexture;
    
    glfwDestroyWindow(window);
}

void GameWindow::Init() {
    skyboxTexture = new CubeMapTexture(
        "Resources/TropicalSunnyDay/Right.png",
        "Resources/TropicalSunnyDay/Left.png",
        "Resources/TropicalSunnyDay/Up.png",
        "Resources/TropicalSunnyDay/Down.png",
        "Resources/TropicalSunnyDay/Back.png",
        "Resources/TropicalSunnyDay/Front.png"
    );
    
    skybox = new Skybox(skyboxTexture);
    
    camera = new Camera();
}

bool GameWindow::ShouldClose() const {
    return (glfwWindowShouldClose(window) != 0);
}

void GameWindow::Update() {
    
}

void GameWindow::Render() {
    glfwMakeContextCurrent(window);
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    skybox->Render(width, height, camera);
    
    glfwSwapBuffers(window);
}
