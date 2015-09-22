#include "GameWindow.hpp"
#include "Player/FirstPersonPlayer.hpp"
#include "Util/Log.hpp"
#include "Util/GameSettings.hpp"
#include "Geometry/Cube.hpp"
#include "Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include "File.png.hpp"

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
    
    delete renderTarget;
    
    delete geometryObject;
    delete geometry;
    
    delete input;
    
    Resources().FreeTexture2D(texture);
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
    
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
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    renderTarget = new RenderTarget(glm::vec2(static_cast<float>(width), static_cast<float>(height)));
    
    geometry = new Geometry::Cube();
    geometryObject = new GeometryObject(geometry);
    
    vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    fragmentShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
    
    texture = Resources().CreateTexture2D(FILE_PNG, FILE_PNG_LENGTH);
    
    glEnable(GL_DEPTH_TEST);
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
    
    renderTarget->BindForWriting();
    
    glViewport(0, 0, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Start - render cube
    
    shaderProgram->Use();
    glBindVertexArray(geometryObject->Geometry()->VertexArray());
    
    // Texture unit 0 is for base images.
    glUniform1i(shaderProgram->UniformLocation("baseImage"), 0);
    
    // Base image texture
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture->TextureID());
    
    // Model matrix, unique for each model.
    glm::mat4 model = geometryObject->ModelMatrix();
    
    // Send the matrices to the shader.
    glm::mat4 view = player->GetCamera()->View();
    glm::mat4 MV = view * model;
    glm::mat4 N = glm::transpose(glm::inverse(MV));
    
    glUniformMatrix4fv(shaderProgram->UniformLocation("modelViewMatrix"), 1, GL_FALSE, &MV[0][0]);
    glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(N)[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &player->GetCamera()->Projection(screenSize)[0][0]);
    
    // Draw the triangles
    glDrawElements(GL_TRIANGLES, geometryObject->Geometry()->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    // End - render cube
    
    renderTarget->ResetWriting();
    renderTarget->Render(player->GetCamera(), screenSize);
    
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
