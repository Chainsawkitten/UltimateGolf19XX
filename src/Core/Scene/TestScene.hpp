#pragma once

#include <map>
#include "Scene.hpp"
#include "../Skybox.hpp"
#include "../Player/Player.hpp"
#include "../Util/Input.hpp"
#include "../Lighting/DeferredLighting.hpp"
#include "../Object/GeometryObject.hpp"
#include "../Object/ModelObject.hpp"
#include "../Texture/Texture2D.hpp"
#include "../PostProcessing/PostProcessing.hpp"
#include "../PostProcessing/FXAAFilter.hpp"
#include "../Particles/ParticleSystem.hpp"
#include "../Object/TerrainObject.hpp"
#include "../Geometry/Terrain.hpp"
#include "../Geometry/Model.hpp"
#include "../Object/GolfBall.hpp"
#include "../Object/Water.hpp"
#include "../Object/PlayerObject.hpp"
#include "../Object/GUI.hpp"

/** @ingroup Core
 * @{
 */

/// Test scene used for testing purposes.
class TestScene : public Scene {
    public:
        /// Create new test scene.
        /**
         * @param screenSize Size of the screen in pixels.
         */
        TestScene(const glm::vec2& screenSize);
        
        /// Destructor.
        ~TestScene();
        
        /// Update the scene.
        /**
         * @param time Time since last frame (in seconds).
         * @return SceneEnd-struct defining what to do next. nullptr if nothing should be done
         */
        SceneEnd* Update(double time);
        
        /// Render the scene.
        /**
         * @param screenSize Size of the screen in pixels.
         */
        void Render(const glm::vec2& screenSize);
        
    private:
        void RenderToTarget(RenderTarget* renderTarget, float scale, const glm::vec4& clippingPlane);
        
        CubeMapTexture* skyboxTexture;
        Skybox* skybox;
        Player* player;
        DeferredLighting* deferredLighting;
        
        // GUI
        GUI* gui;
        float swingStrength;
        float maxSwingStrength;
        float swingTime;
        float swingDirection;
        float swingAngle;
        
        // Swing arrow
        Geometry::Square* swingArrowGeometry;
        GeometryObject* swingArrow;
        Texture2D* swingArrowTexture;
        
        // Terrain
        TerrainObject* terrainObject;
        Geometry::Terrain* terrain;
        
        // Model
        ModelObject* modelObject;
        Geometry::OBJModel* model;
        
        Shader* vertexShader;
        Shader* geometryShader;
        Shader* fragmentShader;
        ShaderProgram* shaderProgram;
        
        PostProcessing* postProcessing;
        FXAAFilter* fxaaFilter;
        std::map<std::string, ClubType> clubs;
        std::map<std::string, ClubType>::iterator clubIterator;
        GolfBall* golfBall;
        std::vector<PlayerObject> playerObjects;
        int playerIndex;
        int numberOfPlayers;
        glm::vec3 wind;
        
        Water* water;
        
        // Particles
		ParticleSystem* explosionParticleSystem;
		ParticleEmitter* explosionEmitter;
		bool emitterAttached;
		Texture2D* explosionTexture;
        ParticleSystem* particleSystem;
        Texture2D* particleTexture;
};

/** @} */
