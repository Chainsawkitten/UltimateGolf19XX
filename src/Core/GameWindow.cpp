#include "Scene/TestScene.hpp"
#include "GameWindow.hpp"
#include "Util/Log.hpp"
#include "Util/GameSettings.hpp"
#include <chrono>
#include <thread>

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
    
    currentScene = nullptr;
    
    lastTime = 0.0;
    lastTimeRender = 0.0;
}

GameWindow::~GameWindow() {
    delete currentScene;
    delete input;
    
    glfwDestroyWindow(window);
}

void GameWindow::Init() {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    currentScene = new TestScene(glm::vec2(static_cast<float>(width), static_cast<float>(height)));
    
    glEnable(GL_DEPTH_TEST);
    
    lastTime = glfwGetTime();
    lastTimeRender = glfwGetTime();
}

bool GameWindow::ShouldClose() const {
    return (glfwWindowShouldClose(window) != GL_FALSE);
}

void GameWindow::Update() {
    double deltaTime = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();
    
    input->Update();
    input->SetActive();
    
    Scene::SceneEnd* status = currentScene->Update(deltaTime);
    if (status == nullptr) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        currentScene->Render(glm::vec2(static_cast<float>(width), static_cast<float>(height)));
        
        long wait = static_cast<long>((1.0 / GameSettings::GetInstance().GetLong("Target FPS") + lastTimeRender - glfwGetTime()) * 1000000.0);
        if (wait > 0)
            std::this_thread::sleep_for(std::chrono::microseconds(wait));
        lastTimeRender = glfwGetTime();
    } else {
        /// @todo Handle new scene.
        /*if (status->command == Scene::SceneEnd::NEW_SCENE) {
            delete currentScene;
            currentScene = sceneVector[sceneNumber]();
            sceneNumber = (sceneNumber + 1) % sceneVector.size();
        }*/
        
        if (status->command == Scene::SceneEnd::QUIT)
            glfwSetWindowShouldClose(window, GL_TRUE);
        
        delete status;
    }
    
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
    
    glfwSwapBuffers(window);
}

void GameWindow::AssignKeyboardBindings() {
    input->AssignKeyboard(InputHandler::FORWARD, GLFW_KEY_W);
    input->AssignKeyboard(InputHandler::BACKWARD, GLFW_KEY_S);
    input->AssignKeyboard(InputHandler::LEFT, GLFW_KEY_A);
    input->AssignKeyboard(InputHandler::RIGHT, GLFW_KEY_D);
    input->AssignKeyboard(InputHandler::QUIT, GLFW_KEY_ESCAPE);
}
