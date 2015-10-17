#include "TestScene.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "ForwardAlpha.frag.hpp"
#include "../Particles/CuboidParticleEmitter.hpp"
#include "../Particles/PointParticleEmitter.hpp"
#include "../Audio/SoundSystem.hpp"
#include "../Util/GameSettings.hpp"
#include "../Util/Input.hpp"
#include "../Util/Log.hpp"
#include "../Player/ThirdPersonPlayer.hpp"

TestScene::TestScene(const glm::vec2& screenSize) {
    femaleModel = Resources().CreateOBJModel("Resources/Models/Maximo/GolferFemale.obj");
	std::string femaleDiffusePath = "Resources/Models/Maximo/GolferFemaleDiffuse.png";
	std::string femaleNormalPath = "Resources/Models/Maximo/GolferFemaleNormal.png";
	std::string femaleSpecularPath = "Resources/Models/Maximo/GolferFemaleSpecular.png";
	femaleModelObject = new ModelObject(femaleModel, femaleDiffusePath, femaleNormalPath, femaleSpecularPath);
	femaleModelObject->SetPosition(2.f, 0.f, 0.f);
	femaleModelObject->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
    
	maleModel = Resources().CreateOBJModel("Resources/Models/Maximo/GolferMale.obj");
	std::string maleDiffusePath = "Resources/Models/Maximo/GolferMaleDiffuse.png";
	std::string maleNormalPath = "Resources/Models/Maximo/GolferMaleNormal.png";
	std::string maleSpecularPath = "Resources/Models/Maximo/GolferMaleSpecular.png";
	maleModelObject = new ModelObject(maleModel, maleDiffusePath, maleNormalPath, maleSpecularPath);
	maleModelObject->SetPosition(2.f, 0.f, 0.f);
	maleModelObject->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));


    srand(time(NULL));
    
    // Map of all available clubtypes
    clubs["Wood 1"] = ClubType{ 0.2f, glm::radians<float>(11.f) };
    clubs["Wood 3"] = ClubType{ 0.208f, glm::radians<float>(15.f) };
    clubs["Wood 5"] = ClubType{ 0.218f, glm::radians<float>(18.f) };
    clubs["Iron 2"] = ClubType{ 0.232f, glm::radians<float>(18.f) };
    clubs["Iron 3"] = ClubType{ 0.239f, glm::radians<float>(21.f) };
    clubs["Iron 4"] = ClubType{ 0.246f, glm::radians<float>(24.f) };
    clubs["Iron 5"] = ClubType{ 0.253f, glm::radians<float>(27.f) };
    clubs["Iron 6"] = ClubType{ 0.260f, glm::radians<float>(31.f) };
    clubs["Iron 7"] = ClubType{ 0.267f, glm::radians<float>(35.f) };
    clubs["Iron 8"] = ClubType{ 0.274f, glm::radians<float>(39.f) };
    clubs["Iron 9"] = ClubType{ 0.281f, glm::radians<float>(43.f) };
    clubs["Pitching Wedge"] = ClubType{ 0.285f, glm::radians<float>(48.f) };
    clubs["Sand Wedge"] = ClubType{ 0.296f, glm::radians<float>(55.f) };
    clubs["Putter"] = ClubType{ 0.33f, glm::radians<float>(4.f) };
    clubIterator = clubs.begin();
    swingStrength = 0.f;
    maxSwingStrength = 40.f;
    swingTime = 1.f;
    swingDirection = 1.f;
    swingAngle = 0.f;
    
    // Terrain.
    terrain = new Geometry::Terrain("Resources/Terrain/TestMapSmall.png");
    terrain->SetTextureRepeat(glm::vec2(20.f, 20.f));
    terrainObject = new TerrainObject(terrain);
    terrainObject->SetPosition(0.f, -5.f, 0.f);
    terrainObject->SetScale(200.f, 10.f, 200.f);
    
    skyboxTexture = new CubeMapTexture(
                "Resources/TropicalSunnyDay/Right.png",
                "Resources/TropicalSunnyDay/Left.png",
                "Resources/TropicalSunnyDay/Up.png",
                "Resources/TropicalSunnyDay/Down.png",
                "Resources/TropicalSunnyDay/Back.png",
                "Resources/TropicalSunnyDay/Front.png"
                );
    skybox = new Skybox(skyboxTexture);
    
    deferredLighting = new DeferredLighting(screenSize);
    
    // Post processing.
    postProcessing = new PostProcessing(screenSize);
    fxaaFilter = new FXAAFilter();
    
    vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    fragmentShader = Resources().CreateShader(FORWARDALPHA_FRAG, FORWARDALPHA_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
    
    // Water.
    water =  new Water(screenSize);
    water->SetScale(400.f, 400.f, 400.f);
    water->SetPosition(0.f, -1.f, 0.f);
    water->SetTextureRepeat(glm::vec2(75.f, 75.f));
    
    gui = new GUI(screenSize);
    
    swingArrowTexture = Resources().CreateTexture2DFromFile("Resources/GUI/Arrow.png");
    swingArrowGeometry = Resources().CreateSquare();
    swingArrow = new GeometryObject(swingArrowGeometry);
    swingArrow->SetScale(0.25f, 1.f, 1.f);
    
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
    
    // Ducks
    for (int i=0; i<10; i++) {
        ducks.push_back(new Duck());
        glm::vec3 position;
        do {
            position = glm::vec3(rand() / static_cast<float>(RAND_MAX) * 200.f - 100.f, water->Position().y, rand() / static_cast<float>(RAND_MAX) * 200.f - 100.f);
        } while (terrainObject->GetY(position.x, position.z) > water->Position().y - 0.2f);
        ducks[i]->SetPosition(position);
        ducks[i]->SetRotation(rand() / static_cast<float>(RAND_MAX) * 360.f, 0.f, 0.f);
    }
    
    // Lily pads
    for (int i=0; i<50; i++) {
        lilypads.push_back(new LilyPad());
        glm::vec3 position;
        do {
            position = glm::vec3(rand() / static_cast<float>(RAND_MAX) * 200.f - 100.f, water->Position().y + 0.05f, rand() / static_cast<float>(RAND_MAX) * 200.f - 100.f);
        } while (terrainObject->GetY(position.x, position.z) > water->Position().y - 0.2f);
        lilypads[i]->SetPosition(position);
        lilypads[i]->SetRotation(rand() / static_cast<float>(RAND_MAX) * 360.f, 270.f, 0.f);
        lilypads[i]->SetScale(2.f, 2.f, 1.f);
    }
    
    explosionTexture = Resources().CreateTexture2DFromFile("Resources/FireParticle.png");
    
    // Particle type.
    ParticleType explosionParticle;
    explosionParticle.texture = explosionTexture;
    explosionParticle.minLifetime = .1f;
    explosionParticle.maxLifetime = .2f;
    explosionParticle.minVelocity = glm::vec3(-1.f, 1.f, -1.f);
    explosionParticle.maxVelocity = glm::vec3(1.f, -1.f, 1.f);
    explosionParticle.minSize = glm::vec2(0.025f, 0.025f);
    explosionParticle.maxSize = glm::vec2(0.05f, 0.05f);
    explosionParticle.uniformScaling = true;
    explosionParticle.color = glm::vec3(1.f, 0.5f, 0.5f);
    
    // Particle system.
    explosionParticleSystem = new ParticleSystem(explosionParticle, 1000);
    emitterAttached = false;
    
    // Initiate players
    numberOfPlayers = 2;
    playerIndex = 0;
    playerObjects.push_back(PlayerObject{ glm::vec3(5.f, terrainObject->GetY(5.f, 5.f)+0.01f, 5.f) });
    playerObjects.push_back(PlayerObject{ glm::vec3(-5.f, terrainObject->GetY(-5.f, -5.f) + 0.01f, -5.f) });
    playerIterator = playerObjects.begin();
    
    // Golf ball.
    golfBall = new GolfBall(GolfBall::TWOPIECE, terrainObject, water);
    golfBall->SetPosition(playerObjects[0].Position());
    
    // Camera.
    player = new ThirdPersonPlayer(golfBall);
    player->SetMovementSpeed(2.f);
    wind = glm::vec3(static_cast<float>(rand() % 30 + (-15)), 0.f, static_cast<float>(rand() % 30 + (-15)));
    
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

	delete explosionParticleSystem;
	Resources().FreeTexture2DFromFile(explosionTexture);
    
    delete player;
    
    delete deferredLighting;
    
    delete fxaaFilter;
    delete postProcessing;
    
    delete swingArrow;
    Resources().FreeSquare();
    delete femaleModelObject;
    Resources().FreeOBJModel(femaleModel);
	delete maleModelObject;
	Resources().FreeOBJModel(maleModel);
    
    delete golfBall;
    delete terrainObject;
    delete terrain;
    
    delete water;
    
    Resources().FreeTexture2DFromFile(swingArrowTexture);
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
    
    for (Duck* duck : ducks) {
        delete duck;
    }
    
    for (LilyPad* lilypad : lilypads) {
        delete lilypad;
    }
}

TestScene::SceneEnd* TestScene::Update(double time) {
    swingStrength += time / swingTime * swingDirection;
    if (swingStrength > 1.f || swingStrength < 0.f) {
        swingDirection = -swingDirection;
        swingStrength = glm::clamp(swingStrength, 0.f, 1.f);
    }
    
    swingAngle += time * (Input()->Pressed(InputHandler::RIGHT) - Input()->Pressed(InputHandler::LEFT));
    glm::vec3 strikeDirection = glm::vec3(cos(swingAngle), 0.f, sin(swingAngle));
    
    if (Input()->Triggered(InputHandler::STRIKE)) {
        golfBall->Strike(clubIterator->second, maxSwingStrength * swingStrength * strikeDirection);
        
    }
    golfBall->Update(time, wind, playerObjects);
    
    
    if (Input()->Triggered(InputHandler::RESET)) {
        playerIterator++;
        if (playerIterator == playerObjects.end())
            playerIterator = playerObjects.begin();
        golfBall->Reset(playerIterator->Position());
        wind = glm::vec3(static_cast<float>(rand() % 30 + (-15)), static_cast<float>(rand() % 4 + (-2)), static_cast<float>(rand() % 30 + (-15)));
        Log() << wind << "\n";
    }
    
    player->Update(time);
    
    if (Input()->Triggered(InputHandler::NEXTCLUB)) {
        clubIterator++;
        if (clubIterator == clubs.end())
            clubIterator = clubs.begin();
        Log() << clubIterator->first;
        Log() << "\n";
    }
    
    if (Input()->Triggered(InputHandler::EXPLODE) && golfBall->GetState() == GolfBall::ACTIVE) {
        if (playerIndex < (numberOfPlayers-1))
            playerIndex++;
        else
            playerIndex = 0;
        
        // Emitters.
        explosionEmitter = new PointParticleEmitter(golfBall->Position(), 0.1, 0.2, false);
        explosionParticleSystem->AddParticleEmitter(explosionEmitter);
        explosionEmitter->Update(15, explosionParticleSystem, player->GetCamera());
        emitterAttached = true;
        explosionEmitter->resetLifetime();
        
        golfBall->Explode(playerObjects);
    }
    
    SoundSystem::GetInstance()->GetListener()->SetPosition(player->GetCamera()->Position());
    SoundSystem::GetInstance()->GetListener()->SetOrientation(player->GetCamera()->Forward(), player->GetCamera()->Up());
    
    if (emitterAttached && explosionEmitter->getLifetime() > 0.5 ){
        explosionParticleSystem->RemoveParticleEmitter();
        emitterAttached = false;
    }
    
    particleSystem->Update(time, player->GetCamera());
    explosionParticleSystem->Update(time, player->GetCamera());
    water->Update(time, wind);
    
    swingArrow->SetRotation(-glm::degrees(swingAngle) - 90.f, 270.f, 0.f);
    glm::vec3 swingPosition = golfBall->Position() + strikeDirection * 0.5f * swingArrow->Scale().y - glm::vec3(0.f, golfBall->Radius(), 0.f);
    swingArrow->SetPosition(swingPosition);
    
    for (Duck* duck : ducks) {
        duck->Update(time, terrainObject, water);
    }
    
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
    
    water->Render(player->GetCamera(), deferredLighting->light, screenSize);
    
    if (GameSettings::GetInstance().GetBool("FXAA")) {
        fxaaFilter->SetScreenSize(screenSize);
        postProcessing->ApplyFilter(fxaaFilter);
    }
    
    particleSystem->Render(player->GetCamera(), screenSize);
    explosionParticleSystem->Render(player->GetCamera(), screenSize);
    
    postProcessing->Render();
    
    // Start - swing arrow
    if (golfBall->GetState() == GolfBall::INITIAL) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shaderProgram->Use();
        glBindVertexArray(swingArrow->Geometry()->VertexArray());
        
        // Texture unit 0 is for base images.
        glUniform1i(shaderProgram->UniformLocation("baseImage"), 0);
        
        // Base image texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, swingArrowTexture->TextureID());
        
        // Model matrix, unique for each model.
        glm::mat4 model = swingArrow->ModelMatrix();
        
        // Send the matrices to the shader.
        glm::mat4 view = player->GetCamera()->View();
        glm::mat4 normal = glm::transpose(glm::inverse(view * model));
        
        glUniformMatrix4fv(shaderProgram->UniformLocation("modelMatrix"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(shaderProgram->UniformLocation("viewMatrix"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normal)[0][0]);
        glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &player->GetCamera()->Projection(screenSize)[0][0]);
        
        glUniform4fv(shaderProgram->UniformLocation("clippingPlane"), 1, &glm::vec4(0.f, 0.f, 0.f, 0.f)[0]);
        
        // Draw the triangles
        glDrawElements(GL_TRIANGLES, swingArrow->Geometry()->IndexCount(), GL_UNSIGNED_INT, (void*)0);
        glDisable(GL_BLEND);
        // End - swing arrow
    }
    
    gui->Render(screenSize, playerObjects, swingStrength);
}

void TestScene::RenderToTarget(RenderTarget *renderTarget, float scale, const glm::vec4& clippingPlane) {
    deferredLighting->BindForWriting();
    
    glViewport(0, 0, static_cast<int>(renderTarget->Size().x), static_cast<int>(renderTarget->Size().y));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	bool male = true;
    for (auto &playerObject : playerObjects) {
		if (male == false){
			femaleModelObject->SetPosition(playerObject.Position());
			femaleModelObject->Render(player->GetCamera(), renderTarget->Size(), clippingPlane);
			male = true;
		} else {
			maleModelObject->SetPosition(playerObject.Position());
			maleModelObject->Render(player->GetCamera(), renderTarget->Size(), clippingPlane);
			male = false;
		}
    }
    
    golfBall->Render(player->GetCamera(), renderTarget->Size(), clippingPlane);
    
    terrainObject->Render(player->GetCamera(), renderTarget->Size(), clippingPlane);
    
    for (Duck* duck : ducks) {
        duck->Render(player->GetCamera(), renderTarget->Size(), clippingPlane);
    }
    
    for (LilyPad* lilypad : lilypads) {
        lilypad->Render(player->GetCamera(), renderTarget->Size(), clippingPlane);
    }
    
    renderTarget->SetTarget();
    
    deferredLighting->Render(player->GetCamera(), renderTarget->Size(), scale);
    skybox->Render(player->GetCamera(), renderTarget->Size());
}

