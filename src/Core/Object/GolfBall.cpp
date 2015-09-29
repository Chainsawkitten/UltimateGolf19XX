#include "GolfBall.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include "glm\gtc\constants.hpp"

GolfBall::GolfBall(BallType ballType) : Object() {
    active = false;
	resistution = 0.78f;

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

	
    
	sphere = new Physics::Sphere(modelObject->Position(), 0.0214f);
}

GolfBall::~GolfBall() {
	delete modelObject;
	delete modelGeometry;
}

void GolfBall::Update(double time, const glm::vec3& wind) {
    if (active) {
		modelObject->Move(static_cast<float>(time)*velocity);
		sphere->position = modelObject->Position();
		float horizontal = -static_cast<float>(time)*angularVelocity.x*(180.f / glm::pi<float>());
		float vertical = -static_cast<float>(time)*angularVelocity.y*(180.f / glm::pi<float>());
		float tilt = -static_cast<float>(time)*angularVelocity.z*(180.f / glm::pi<float>());

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

void GolfBall::Strike(float clubMass, float clubLoft, glm::vec3 clubVelocity) {
	active = true;
	float translatedVelocity = sqrt(pow(clubVelocity.x, 2) + pow(clubVelocity.z, 2));

	angularVelocity = glm::vec3(0.f, 0.f, -(5.f / 7.f)*(sin(clubLoft)*translatedVelocity));

	float massCoefficient = clubMass / (clubMass + mass);
	float velocitybx = translatedVelocity*massCoefficient*((1 + resistution)*pow(cos(clubLoft), 2) + (2.f / 7.f)*pow(sin(clubLoft), 2));
	float velocityby = translatedVelocity*massCoefficient*sin(clubLoft)*cos(clubLoft)*((5.f / 7.f) + resistution);

	float horizontalAngle = atan(clubVelocity.x / clubVelocity.z);
	velocity = glm::vec3(velocitybx*cos(horizontalAngle), velocityby, velocitybx*sin(horizontalAngle));
}

void GolfBall::SetRadius(float radius) {
	sphere->radius = radius;
    SetScale(2.f * glm::vec3(radius, radius, radius));
    area = glm::pi<float>() * radius * radius;
}
