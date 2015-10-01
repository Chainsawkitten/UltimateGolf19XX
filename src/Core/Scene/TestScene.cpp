#include "TestScene.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include "../Particles/CuboidParticleEmitter.hpp"
#include "../Audio/SoundSystem.hpp"
#include "../Util/GameSettings.hpp"
#include "../Util/Input.hpp"

TestScene::TestScene(const glm::vec2& screenSize) {
    skyboxTexture = new CubeMapTexture(
        "Resources/TropicalSunnyDay/Right.png",
        "Resources/TropicalSunnyDay/Left.png",
        "Resources/TropicalSunnyDay/Up.png",
        "Resources/TropicalSunnyDay/Down.png",
        "Resources/TropicalSunnyDay/Back.png",
        "Resources/TropicalSunnyDay/Front.png"
    );

	model = new Geometry::Model("Resources/Models/rock/Rock.bin");
	std::string diffusePath = "Resources/Models/rock/Diffuse.png";
	std::string normalPath = "Resources/Models/rock/Normal.png";
	std::string specularPath = "Resources/Models/rock/Specular.png";
	modelObject = new ModelObject(model, diffusePath, normalPath , specularPath);
	modelObject->SetPosition(4.f,0.f,0.f);
	modelObject->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));

	terrain = new Geometry::Terrain("Resources/Terrain/TestMapSmall.png");
	terrain->SetTextureRepeat(glm::vec2(10.f, 10.f));
	terrainObject = new TerrainObject(terrain);
    terrainObject->SetPosition(0.f, -5.f, 0.f);
	terrainObject->SetScale(50.f, 10.f, 50.f);

    skybox = new Skybox(skyboxTexture);
    
    player = new Player();
    player->SetMovementSpeed(2.f);
    
    deferredLighting = new DeferredLighting(screenSize);
    
    postProcessing = new PostProcessing(screenSize);
    fxaaFilter = new FXAAFilter();
    
    geometry = Resources().CreateCube();
    geometryObject = new GeometryObject(geometry);
    
    vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    fragmentShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
    
    texture = Resources().CreateTexture2DFromFile("Resources/CGTextures/cliff.png");
    
    golfBall = new GolfBall(GolfBall::TWOPIECE, terrainObject);
    golfBall->SetPosition(2.f, 0.f, 0.f);
    
    wind = glm::vec3(0.f, 0.f, 4.f);
    
    water =  new Water(screenSize);
    water->SetScale(400.f, 400.f, 400.f);
    water->SetPosition(0.f, -1.f, 0.f);
    
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
}

TestScene::~TestScene() {
	delete skybox;
    delete skyboxTexture;
    
    delete particleSystem;
    Resources().FreeTexture2DFromFile(particleTexture);
    
    delete player;
    
    delete deferredLighting;
    
    delete fxaaFilter;
    delete postProcessing;
    
    delete geometryObject;
    Resources().FreeCube();
    
    delete golfBall;
    
    delete water;
    
    Resources().FreeTexture2DFromFile(texture);
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
}

TestScene::SceneEnd* TestScene::Update(double time) {
    player->Update(time);
    
    if (Input()->Triggered(InputHandler::STRIKE))
		golfBall->Strike(golfBall->iron3, glm::vec3(5.f, 0.f, 0.f));
    golfBall->Update(time, wind);
    
    SoundSystem::GetInstance()->GetListener()->SetPosition(player->GetCamera()->Position());
    SoundSystem::GetInstance()->GetListener()->SetOrientation(player->GetCamera()->Forward(), player->GetCamera()->Up());
    
    particleSystem->Update(time, player->GetCamera());
    
    return nullptr;
}

void TestScene::Render(const glm::vec2& screenSize) {
    // Render refractions.
    RenderToTarget(water->RefractionTarget(), 0.25f, water->RefractionClippingPlane());
    
    // Render reflections
    /// @todo Don't hardcore camera inversion.
    float distance = 2.f * (player->GetCamera()->Position().y - water->Position().y);
    player->GetCamera()->SetPosition(player->GetCamera()->Position() - glm::vec3(0.f, distance, 0.f));
    player->GetCamera()->SetRotation(player->GetCamera()->HorizontalAngle(), -player->GetCamera()->VerticalAngle(), player->GetCamera()->TiltAngle());
    RenderToTarget(water->ReflectionTarget(), 1.f, water->ReflectionClippingPlane());
    player->GetCamera()->SetRotation(player->GetCamera()->HorizontalAngle(), -player->GetCamera()->VerticalAngle(), player->GetCamera()->TiltAngle());
    player->GetCamera()->SetPosition(player->GetCamera()->Position() + glm::vec3(0.f, distance, 0.f));
    
    // Render to screen.
    RenderToTarget(postProcessing->GetRenderTarget(), 1.f, glm::vec4(0.f, 0.f, 0.f, 0.f));
    
    water->Render(player->GetCamera(), screenSize);
    
    if (GameSettings::GetInstance().GetBool("FXAA")) {
        fxaaFilter->SetScreenSize(screenSize);
        postProcessing->ApplyFilter(fxaaFilter);
    }
    
    particleSystem->Render(player->GetCamera(), screenSize);
    
    postProcessing->Render();
}

void TestScene::RenderToTarget(RenderTarget *renderTarget, float scale, const glm::vec4& clippingPlane) {
    deferredLighting->BindForWriting();
    
    glViewport(0, 0, static_cast<int>(renderTarget->Size().x), static_cast<int>(renderTarget->Size().y));
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
    glm::mat4 N = glm::transpose(glm::inverse(view * model));
    
    glUniformMatrix4fv(shaderProgram->UniformLocation("modelMatrix"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("viewMatrix"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(N)[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &player->GetCamera()->Projection(renderTarget->Size())[0][0]);
    
    glUniform4fv(shaderProgram->UniformLocation("clippingPlane"), 1, &clippingPlane[0]);
    
    // Draw the triangles
    glDrawElements(GL_TRIANGLES, geometryObject->Geometry()->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    // End - render cube
    
	modelObject->Render(player->GetCamera(), renderTarget->Size(), clippingPlane);

    golfBall->Render(player->GetCamera(), renderTarget->Size(), clippingPlane);
    
	terrainObject->Render(player->GetCamera(), renderTarget->Size(), clippingPlane);
    renderTarget->SetTarget();
    
    deferredLighting->Render(player->GetCamera(), renderTarget->Size(), scale);
    skybox->Render(player->GetCamera(), renderTarget->Size());
}
