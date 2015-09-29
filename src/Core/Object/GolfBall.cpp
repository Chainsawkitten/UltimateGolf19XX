#include "GolfBall.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"

GolfBall::GolfBall() : Object() {
    active = false;

	modelGeometry = new Geometry::Model("Resources/Models/rock/Rock.bin");
	std::string diffusePath = "Resources/Models/rock/diffuse.tga";
	std::string normalPath = "Resources/Models/rock/normal.tga";
	std::string specularPath = "Resources/Models/rock/specular.tga";
	modelObject = new ModelObject(modelGeometry, diffusePath, normalPath, specularPath);
	modelObject->SetPosition(2.f, 0.f, 0.f);
	modelObject->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
}

GolfBall::~GolfBall() {
	delete modelObject;
	delete modelGeometry;
}

void GolfBall::Update(double time) {
    if (active) {
		modelObject->Move(static_cast<float>(time)* velocity);
        
        /// @todo Calculate forces.
        
        glm::vec3 acceleration = glm::vec3(0.f, -9.82f, 0.f);
        velocity += acceleration * static_cast<float>(time);
    }
}

void GolfBall::Render(Camera* camera, const glm::vec2& screenSize) const{
	modelObject->Render(camera, screenSize);
}

void GolfBall::Strike() {
    active = true;
    
    /// @todo Calculate velocity based on club mass, loft and velocity.
    
    velocity = glm::vec3(20.f, 5.f, 0.f);
}
