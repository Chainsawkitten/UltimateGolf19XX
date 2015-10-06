#include "TestScene.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include "../Particles/CuboidParticleEmitter.hpp"
#include "../Audio/SoundSystem.hpp"
#include "../Util/GameSettings.hpp"
#include "../Util/Input.hpp"
#include "../Util/Log.hpp"
#include "../Player/ThirdPersonPlayer.hpp"

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

	/// Map of all available clubtypes
	clubs.insert(std::pair<std::string, ClubType>("Wood 1", ClubType{ 0.2f, glm::radians<float>(11.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Wood 3", ClubType{ 0.208f, glm::radians<float>(15.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Wood 5", ClubType{ 0.218f, glm::radians<float>(18.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Iron 2", ClubType{ 0.232f, glm::radians<float>(18.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Iron 3", ClubType{ 0.239f, glm::radians<float>(21.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Iron 4", ClubType{ 0.246f, glm::radians<float>(24.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Iron 5", ClubType{ 0.253f, glm::radians<float>(27.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Iron 6", ClubType{ 0.260f, glm::radians<float>(31.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Iron 7", ClubType{ 0.267f, glm::radians<float>(35.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Iron 8", ClubType{ 0.274f, glm::radians<float>(39.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Iron 9", ClubType{ 0.281f, glm::radians<float>(43.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Pitching Wedge", ClubType{ 0.285f, glm::radians<float>(48.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Sand Wedge", ClubType{ 0.296f, glm::radians<float>(55.f) }));
	clubs.insert(std::pair<std::string, ClubType>("Putter", ClubType{ 0.33f, glm::radians<float>(4.f) }));
	clubIterator = clubs.begin();
	terrain = new Geometry::Terrain("Resources/Terrain/FlatMapSmall.png");
	terrain->SetTextureRepeat(glm::vec2(10.f, 10.f));
	terrainObject = new TerrainObject(terrain);
    terrainObject->SetPosition(0.f, -5.f, 0.f);
	terrainObject->SetScale(50.f, 10.f, 50.f);

    skybox = new Skybox(skyboxTexture);
    
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
    
    player = new ThirdPersonPlayer(golfBall);
    player->SetMovementSpeed(2.f);
    
    wind = glm::vec3(0.f, 0.f, 4.f);
    
    water =  new Water(screenSize);
    water->SetScale(400.f, 400.f, 400.f);
    water->SetPosition(0.f, -1.f, 0.f);
    water->SetTextureRepeat(glm::vec2(75.f, 75.f));
    
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
    glm::vec3 wind = glm::vec3(5.f, 0.f, 0.f);
    if (Input()->Triggered(InputHandler::STRIKE))
		golfBall->Strike(clubIterator->second, wind);
    golfBall->Update(time, wind);

	if (Input()->Triggered(InputHandler::RESET))
		golfBall->Reset();
    
    player->Update(time);
	
	if (Input()->Triggered(InputHandler::NEXTCLUB)){
		clubIterator++;
		if (clubIterator == clubs.end())
			clubIterator = clubs.begin();
		Log() << clubIterator->first;
		Log() << "\n";
	}
    SoundSystem::GetInstance()->GetListener()->SetPosition(player->GetCamera()->Position());
    SoundSystem::GetInstance()->GetListener()->SetOrientation(player->GetCamera()->Forward(), player->GetCamera()->Up());
    
    particleSystem->Update(time, player->GetCamera());
    //water->Update(time, wind);
    
    return nullptr;
}

void TestScene::Render(const glm::vec2& screenSize) {
    // Render refractions.
    RenderToTarget(water->RefractionTarget(), 1.f, water->RefractionClippingPlane());
    
    // Render reflections
    /// @todo Don't hardcore camera inversion.
    float distance = 2.f * (player->GetCamera()->Position().y - water->Position().y);
    player->GetCamera()->SetPosition(player->GetCamera()->Position() - glm::vec3(0.f, distance, 0.f));
    player->GetCamera()->SetRotation(player->GetCamera()->HorizontalAngle(), -player->GetCamera()->VerticalAngle(), player->GetCamera()->TiltAngle());
    RenderToTarget(water->ReflectionTarget(), 0.5f, water->ReflectionClippingPlane());
    player->GetCamera()->SetRotation(player->GetCamera()->HorizontalAngle(), -player->GetCamera()->VerticalAngle(), player->GetCamera()->TiltAngle());
    player->GetCamera()->SetPosition(player->GetCamera()->Position() + glm::vec3(0.f, distance, 0.f));
    
    // Render to screen.
    RenderToTarget(postProcessing->GetRenderTarget(), 1.f, glm::vec4(0.f, 0.f, 0.f, 0.f));
    
    //water->Render(player->GetCamera(), deferredLighting->light, screenSize);
    
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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->TextureID());
    
    // Model matrix, unique for each model.
    glm::mat4 model = geometryObject->ModelMatrix();
    
    // Send the matrices to the shader.
    glm::mat4 view = player->GetCamera()->View();
    glm::mat4 normal = glm::transpose(glm::inverse(view * model));
    
    glUniformMatrix4fv(shaderProgram->UniformLocation("modelMatrix"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("viewMatrix"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normal)[0][0]);
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
