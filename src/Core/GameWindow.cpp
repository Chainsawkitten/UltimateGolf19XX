#include "GameWindow.hpp"
#include "Player/FirstPersonPlayer.hpp"
#include "Util/Log.hpp"
#include "Util/GameSettings.hpp"

GameWindow::GameWindow() {
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(GameSettings::GetInstance().GetLong("Screen Width"), GameSettings::GetInstance().GetLong("Screen Height"), "Ultimate Golf 19XX", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        /// @todo Print error to log.
    }

    glfwMakeContextCurrent(window);
    
    input = new InputHandler(window);
    input->Update();
    input->SetActive();
    AssignKeyboardBindings();
}

GameWindow::~GameWindow() {
    delete skybox;
    delete skyboxTexture;
    
    delete player;
    
    delete input;
    
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
    
    player = new FirstPersonPlayer();
    player->SetMovementSpeed(2.f);
}

bool GameWindow::ShouldClose() const {
    return (glfwWindowShouldClose(window) != GL_FALSE);
}

void GameWindow::Update() {
    input->Update();
    input->SetActive();
    
    player->Update(1.f);
    
    if (input->Pressed(InputHandler::QUIT))
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void GameWindow::Render() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    Render(glm::vec2(static_cast<float>(width), static_cast<float>(height)));
}

void GameWindow::Render(const glm::vec2& screenSize) {
    glfwMakeContextCurrent(window);
    
    skybox->Render(player->GetCamera(), screenSize);
    
    glfwSwapBuffers(window);
}

void GameWindow::AssignKeyboardBindings() {
    input->AssignKeyboard(InputHandler::FORWARD, GLFW_KEY_W);
    input->AssignKeyboard(InputHandler::BACKWARD, GLFW_KEY_S);
    input->AssignKeyboard(InputHandler::LEFT, GLFW_KEY_A);
    input->AssignKeyboard(InputHandler::RIGHT, GLFW_KEY_D);
    input->AssignKeyboard(InputHandler::QUIT, GLFW_KEY_ESCAPE);
}
