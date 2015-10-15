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
    model = Resources().CreateOBJModel("Resources/Models/Maximo/GolferFemale.obj");
    std::string diffusePath = "Resources/Models/Maximo/GolferFemaleDiffuse.png";
    std::string normalPath = "Resources/Models/Maximo/GolferFemaleNormal.png";
    std::string specularPath = "Resources/Models/Maximo/GolferFemaleSpecular.png";
    modelObject = new ModelObject(model, diffusePath, normalPath , specularPath);
    modelObject->SetPosition(2.f, 0.f, 0.f);
    modelObject->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
    
	/// Map of all available clubtypes
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

	terrain = new Geometry::Terrain("Resources/Terrain/TestMapSmall.tga");
	terrain->SetTextureRepeat(glm::vec2(10.f, 10.f));
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

	gui = new GUI(screenSize);
    
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

	///Initiate players
	numberOfPlayers = 2;
	playerIndex = 0;
	playerObjects.push_back(PlayerObject{ glm::vec3(5.f, terrainObject->GetY(5.f, 5.f)+0.01f, 5.f) });
	playerObjects.push_back(PlayerObject{ glm::vec3(-5.f, terrainObject->GetY(-5.f, -5.f) + 0.01f, -5.f) });
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
    delete modelObject;
    Resources().FreeOBJModel(model);
    
    delete golfBall;
    delete terrainObject;
    delete terrain;
    
    delete water;
    
    Resources().FreeTexture2DFromFile(texture);
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
}

TestScene::SceneEnd* TestScene::Update(double time) {
    glm::vec3 wind = glm::vec3(0.f, 0.f, 0.f);
	swingStrength += time / swingTime * swingDirection;
    if (swingStrength > 1.f || swingStrength < 0.f) {
        swingDirection = -swingDirection;
        swingStrength = glm::clamp(swingStrength, 0.f, 1.f);
    }
	
	if (Input()->Triggered(InputHandler::STRIKE)) {
		glm::vec3 tempCamera = player->GetCamera()->Position();
		glm::vec3 tempBall = golfBall->Position();
		glm::vec3 strikeDirection = glm::normalize(glm::vec3(tempBall.x - tempCamera.x, 0.f, tempBall.z - tempCamera.z));
		golfBall->Strike(clubIterator->second, maxSwingStrength * swingStrength * strikeDirection);
		
	}
	golfBall->Update(time, wind, playerObjects);


	if (Input()->Triggered(InputHandler::RESET))
		golfBall->Reset();
    
    player->Update(time);
	
	if (Input()->Triggered(InputHandler::NEXTCLUB)) {
		clubIterator++;
		if (clubIterator == clubs.end())
			clubIterator = clubs.begin();
		Log() << clubIterator->first << "\n";
	}
    
	if (Input()->Triggered(InputHandler::EXPLODE)) {
		if (playerIndex < (numberOfPlayers-1))
			playerIndex++;
		else
			playerIndex = 0;
		golfBall->Explode(playerObjects, playerIndex);
	}
    
    SoundSystem::GetInstance()->GetListener()->SetPosition(player->GetCamera()->Position());
    SoundSystem::GetInstance()->GetListener()->SetOrientation(player->GetCamera()->Forward(), player->GetCamera()->Up());
    
    particleSystem->Update(time, player->GetCamera());
    water->Update(time, wind);
    
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
    
    postProcessing->Render();
    
    gui->Render(screenSize, playerObjects, swingStrength);
}

void TestScene::RenderToTarget(RenderTarget *renderTarget, float scale, const glm::vec4& clippingPlane) {
    deferredLighting->BindForWriting();
    
    glViewport(0, 0, static_cast<int>(renderTarget->Size().x), static_cast<int>(renderTarget->Size().y));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	for (auto &playerObject : playerObjects){
		modelObject->SetPosition(playerObject.Position());
		modelObject->Render(player->GetCamera(), renderTarget->Size(), clippingPlane);
	}
    
    golfBall->Render(player->GetCamera(), renderTarget->Size(), clippingPlane);
    
    terrainObject->Render(player->GetCamera(), renderTarget->Size(), clippingPlane);

	renderTarget->SetTarget();
    
    deferredLighting->Render(player->GetCamera(), renderTarget->Size(), scale);
    skybox->Render(player->GetCamera(), renderTarget->Size());
}
