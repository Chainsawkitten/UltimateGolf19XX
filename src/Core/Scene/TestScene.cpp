#include "TestScene.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include "File.png.hpp"
#include "../Particles/CuboidParticleEmitter.hpp"
#include "../Audio/SoundSystem.hpp"
#include "../Util/GameSettings.hpp"

TestScene::TestScene(const glm::vec2& screenSize) {
    skyboxTexture = new CubeMapTexture(
        "Resources/TropicalSunnyDay/Right.png",
        "Resources/TropicalSunnyDay/Left.png",
        "Resources/TropicalSunnyDay/Up.png",
        "Resources/TropicalSunnyDay/Down.png",
        "Resources/TropicalSunnyDay/Back.png",
        "Resources/TropicalSunnyDay/Front.png"
    );
	terrain = new Geometry::Terrain("Resources/Terrain/FlatMapSmall.png");
	terrain->SetTextureRepeat(glm::vec2(10.f, 10.f));
	terrainObject = new TerrainObject(terrain);

    skybox = new Skybox(skyboxTexture);
    
    player = new Player();
    player->SetMovementSpeed(2.f);
    
    renderTarget = new RenderTarget(screenSize);
    
    postProcessing = new PostProcessing(screenSize);
    fxaaFilter = new FXAAFilter();
    
    geometry = Resources().CreateCube();
    geometryObject = new GeometryObject(geometry);
    
    vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    fragmentShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
    
    texture = Resources().CreateTexture2D(FILE_PNG, FILE_PNG_LENGTH);
    
    // Particle texture.
    particleTexture = Resources().CreateTexture2DFromFile("Resources/DustParticle.png");
    
    // Particle type.
    ParticleType dustParticle;
    dustParticle.texture = particleTexture;
    dustParticle.minLifetime = 6.f;
    dustParticle.maxLifetime = 10.f;
    dustParticle.minVelocity = glm::vec3(-0.025f, -0.01f, -0.025f);
    dustParticle.maxVelocity = glm::vec3(0.025f, -0.1f, 0.025f);
    dustParticle.minSize = glm::vec2(0.025f, 0.025f);
    dustParticle.maxSize = glm::vec2(0.05f, 0.05f);
    dustParticle.uniformScaling = true;
    dustParticle.color = glm::vec3(0.2f, 1.f, 0.f);
    
    // Particle system.
    particleSystem = new ParticleSystem(dustParticle, 1000);
    
    // Emitters.
    ParticleEmitter* emitter = new CuboidParticleEmitter(glm::vec3(0.f, 0.f, 0.f), glm::vec3(20.f, 4.f, 20.f), 0.01, 0.02, true);
    particleSystem->AddParticleEmitter(emitter);
    emitter->Update(5.0, particleSystem, player->GetCamera());
    
    // Audio.
    waveFile = new VorbisFile("Resources/Testing.ogg");
    buffer = new SoundBuffer(waveFile);
    sound = new Sound(buffer);
    sound->SetLooping(true);
    sound->SetGain(2.f);
    sound->Play();
}

TestScene::~TestScene() {
    delete sound;
    delete buffer;
    delete waveFile;
    
    delete skybox;
    delete skyboxTexture;
    
    delete particleSystem;
    Resources().FreeTexture2DFromFile(particleTexture);
    
    delete player;
    
    delete renderTarget;
    
    delete fxaaFilter;
    delete postProcessing;
    
    delete geometryObject;
    Resources().FreeCube();
    
    Resources().FreeTexture2D(texture);
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
}

TestScene::SceneEnd* TestScene::Update(double time) {
    player->Update(time);
    
    SoundSystem::GetInstance()->GetListener()->SetPosition(player->GetCamera()->Position());
    SoundSystem::GetInstance()->GetListener()->SetOrientation(player->GetCamera()->Forward(), player->GetCamera()->Up());
    
    particleSystem->Update(time, player->GetCamera());
    
    return nullptr;
}

void TestScene::Render(const glm::vec2 &screenSize) {
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
    
	terrainObject->Render(player->GetCamera(), screenSize);
    postProcessing->SetTarget();
    
    renderTarget->Render(player->GetCamera(), screenSize);
    skybox->Render(player->GetCamera(), screenSize);
    particleSystem->Render(player->GetCamera(), screenSize);
    
    if (GameSettings::GetInstance().GetBool("FXAA"))
        postProcessing->ApplyFilter(fxaaFilter);
    
    postProcessing->Render();
}
