#include "GolfBall.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include <cmath>

GolfBall::GolfBall(BallType ballType) : Object() {
    active = false;

	modelGeometry = new Geometry::Model("Resources/Models/rock/Rock.bin");
	std::string diffusePath = "Resources/Models/rock/diffuse.tga";
	std::string normalPath = "Resources/Models/rock/normal.tga";
	std::string specularPath = "Resources/Models/rock/specular.tga";
	modelObject = new ModelObject(modelGeometry, diffusePath, normalPath, specularPath);
	modelObject->SetPosition(2.f, 0.f, 0.f);
	modelObject->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));

    /// @todo Mass based on explosive material.
    mass = 0.0459f;
    this->ballType = ballType;
    
    SetRadius(0.0214f);
}

GolfBall::~GolfBall() {
	delete modelObject;
	delete modelGeometry;
}

void GolfBall::Update(double time, const glm::vec3& wind) {
    if (active) {
		modelObject->Move(static_cast<float>(time)* velocity);
        
        float v = velocity.length();
        float u = (velocity - wind).length();
        glm::vec3 eU = (velocity - wind) / u;
        
        /// Calculate drive force.
        float cD;
        if (ballType == TWOPIECE) {
            cD = v < 65.f ? -0.0051f * v + 0.53f : 0.21f;
        } else {
            cD = v < 60.f ? -0.0084f * v + 0.73f : 0.22f;
        }
        glm::vec3 driveForce = -0.5f * 1.23f * area * cD * u * u * eU;
        
        /// @todo Calculate magnus force.
        glm::vec3 magnusForce = glm::vec3(0.f, 0.f, 0.f);
        
        // Calculate gravitational force.
        glm::vec3 gravitationForce = glm::vec3(0.f, mass * -9.82f, 0.f);
        
        glm::vec3 acceleration = (driveForce + magnusForce + gravitationForce) / mass;
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

void GolfBall::SetRadius(float radius) {
    this->radius = radius;
    SetScale(2.f * glm::vec3(radius, radius, radius));
    area = M_PI * radius * radius;
}
