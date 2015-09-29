#include "GolfBall.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include "glm\gtc\constants.hpp"
#include "../Util/Log.hpp"

GolfBall::GolfBall(BallType ballType, TerrainObject* terrain) : Object() {
    active = false;
	
	this->terrain = terrain;
	groundLevel = this->terrain->Position().y;

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

	this->sphere.position = modelObject->Position();
	SetRadius(0.0214f);
}

GolfBall::~GolfBall() {
	delete modelObject;
	delete modelGeometry;
}

void GolfBall::Update(double time, const glm::vec3& wind) {
    if (active) {
		modelObject->Move(static_cast<float>(time)*velocity);
		sphere.position = modelObject->Position();
		float horizontal = -static_cast<float>(time)*angularVelocity.x*(180.f / glm::pi<float>());
		float vertical = -static_cast<float>(time)*angularVelocity.y*(180.f / glm::pi<float>());
		float tilt = -static_cast<float>(time)*angularVelocity.z*(180.f / glm::pi<float>());

		if ((sphere.position.y - sphere.radius) < groundLevel){
			float e = 0.f;
			float mu = 0.51f;
			this->SetPosition(sphere.position.x, groundLevel + sphere.radius, sphere.position.z);
			modelObject->SetPosition(this->Position());

			if (ballType == TWOPIECE){
				float e = 0.78f;
			} else {
				float e = 0.68f;
			}

			glm::vec3 eNormal = glm::vec3(0.f, 1.f, 0.f);
			glm::vec3 eRoh = glm::vec3(0.f, 1.f, 0.f);
			glm::vec3 muNormal = mu*eNormal;
			glm::vec3 velocityProjectedOneRoh = glm::dot(velocity, eRoh)*eRoh;
			glm::vec3 velocityAfterCollisionProjectedOneRoh = e*velocityProjectedOneRoh;
			velocity = velocity + (velocityAfterCollisionProjectedOneRoh - velocityProjectedOneRoh)*(eRoh + muNormal);
			angularVelocity = ((mass*sphere.radius)/(2*sphere.radius))*(velocityAfterCollisionProjectedOneRoh - velocityProjectedOneRoh)*(glm::cross(eRoh,eNormal));
			return;
		}

		modelObject->Rotate(horizontal, vertical, tilt);
        
        float v = velocity.length();
		float w = angularVelocity.length();
        float u = (velocity - wind).length();
		float Cm = (sqrt(1.f + 0.31f*(v/w))-1.f)/20.f;
		float Fm = 0.5f*Cm*1.23*area*u*u;

        glm::vec3 eU = (velocity - wind) / u;
        
        /// Calculate drive force.
        float cD;
        if (ballType == TWOPIECE) {
            cD = v < 65.f ? -0.0051f * v + 0.53f : 0.21f;
        } else {
            cD = v < 60.f ? -0.0084f * v + 0.73f : 0.22f;
        }
        glm::vec3 dragForce = -0.5f * 1.23f * area * cD * u * u * eU;
        
		glm::vec3 magnusForce = Fm*(cross(eU, (angularVelocity / w)));

        // Calculate gravitational force.
        glm::vec3 gravitationForce = glm::vec3(0.f, mass * -9.82f, 0.f);
        
        glm::vec3 acceleration = (dragForce + magnusForce + gravitationForce) / mass;
        velocity += acceleration * static_cast<float>(time);
    }
}

void GolfBall::Render(Camera* camera, const glm::vec2& screenSize) const{
	modelObject->Render(camera, screenSize);
}

void GolfBall::Strike() {
    active = true;
    
    /// @todo Calculate velocity based on club mass, loft and velocity.
    
    velocity = glm::vec3(5.f, 5.f, 0.f);
	angularVelocity = glm::vec3(0.f,0.f,-800.f);
}

void GolfBall::SetRadius(float radius) {
	this->sphere.radius = radius;
    SetScale(2.f * glm::vec3(radius, radius, radius));
    area = glm::pi<float>() * radius * radius;
}
