#include "GolfBall.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include "glm/gtc/constants.hpp"
#include "../Util/Log.hpp"


GolfBall::GolfBall(BallType ballType, TerrainObject* terrain) : ModelObject(modelGeometry = Resources().CreateOBJModel("Resources/Models/GolfBall/GolfBall.obj"),
                                                                            "Resources/Models/GolfBall/Diffuse.png",
                                                                            "Resources/Models/GolfBall/Normal.png",
                                                                            "Resources/Models/GolfBall/Specular.png") {
    active = false;
    
    restitution = ballType == TWOPIECE ? 0.78f : 0.68f;
    this->terrain = terrain;
    groundLevel = this->terrain->Position().y;
    SetPosition(1.f, 0.f, 1.f);
    
    /// @todo Mass based on explosive material.
    mass = 0.0459f;
    this->ballType = ballType;
    
    this->sphere.position = Position();
    SetRadius(0.0214f);
}

GolfBall::~GolfBall() {
    Resources().FreeOBJModel(modelGeometry);
}

void GolfBall::Reset(){
	active = false;
	velocity = glm::vec3(0.f, 0.f, 0.f);
	angularVelocity = glm::vec3(0.f, 0.f, 0.f);
	SetPosition(1.f, 0.f, 1.f);
	sphere.position = Position();
}

void GolfBall::Update(double time, const glm::vec3& wind) {
    if (active) {
        Move(static_cast<float>(time)*velocity);
        sphere.position = Position();
        
        float horizontal = -static_cast<float>(time)*angularVelocity.x*(glm::pi<float>());
        float vertical = -static_cast<float>(time)*angularVelocity.y*(glm::pi<float>());
        float tilt = -static_cast<float>(time)*angularVelocity.z*(glm::pi<float>());
        
		//check for collision
		if ((sphere.position.y - sphere.radius) < groundLevel){
			//@TODO: Find appropriate collision-coefficient for various materials vs golf ball, 0.30f used temporarily.
			float e = 0.55f;
			float mu = 0.51f;
			//@TODO: Move ball along surfacenormal instead of along y-axis. Need to know distance between balls current position and triangle.
			SetPosition(Position().x, groundLevel+sphere.radius, Position().z);
			sphere.position = Position();
			glm::vec3 surfaceNormal = glm::vec3(0.f, 1.f, 0.f);
			//glm::vec3 originAtTriangle
			//glm::vec3 displacementAlongNormal = surfaceNormal*sphere.radius;
			//modelObject->SetPosition(displacementAlongNormal + originAtTriangle);
			glm::vec3 eRoh = glm::normalize(surfaceNormal);
			glm::vec3 vRoh = velocity*eRoh;
			//uRoh = -evRoh
			glm::vec3 deltaU = -(e + 1.f)*vRoh;
			glm::vec3 eNormal = glm::normalize(sphere.radius*(glm::cross(eRoh, angularVelocity) + glm::vec3(velocity.x, 0.f, velocity.z)));
			velocity = velocity + (deltaU)*(eRoh + mu*eNormal);
			//float angularCoefficient = (mu*sphere.radius*mass) / (0.4f*mass*sphere.radius*sphere.radius);
			//Log() << "Angular coefficient: " << angularCoefficient << "\n";
			glm::vec3 angularDeltaU = (mass*sphere.radius*mu*deltaU)/(mass*sphere.radius*sphere.radius);
			glm::vec3 eR = -eRoh;
			//angularVelocity = angularDeltaU*(glm::cross(eNormal, eR));
		}

        Rotate(horizontal, vertical, tilt);
        
        float v = glm::length(velocity);
        float u = glm::length(velocity - wind);
        float w = glm::length(angularVelocity);
        float Cm = (sqrt(1.f + 0.31f * (w/v)) - 1.f) / 20.f;
        float Fm = 0.5f * Cm * 1.23f * area * u * u;
        glm::vec3 eU = (velocity - wind) / u;
        
        /// Calculate drive force.
        float cD;
        if (ballType == TWOPIECE) {
            cD = v < 65.f ? -0.0051f * v + 0.53f : 0.21f;
        } else {
            cD = v < 60.f ? -0.0084f * v + 0.73f : 0.22f;
        }
        glm::vec3 dragForce = -0.5f * 1.23f * area * cD * u * u * eU;
        
        glm::vec3 magnusForce = Fm * (cross(eU, (angularVelocity / w)));
        
        // Calculate gravitational force.
        glm::vec3 gravitationForce = glm::vec3(0.f, mass * -9.82f, 0.f);
        
        glm::vec3 acceleration = (dragForce + magnusForce + gravitationForce) / mass;
        velocity += acceleration * static_cast<float>(time);
    }
}

void GolfBall::Render(Camera* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane) const{
    ModelObject::Render(camera, screenSize, clippingPlane);
}

void GolfBall::Strike(ClubType club, glm::vec3 clubVelocity) {
    active = true;
    float translatedVelocity = sqrt(pow(clubVelocity.x, 2) + pow(clubVelocity.z, 2));
    
    angularVelocity = glm::vec3(0.f, 0.f, -360.f * (5.f / 7.f) * (sin(club.loft) * translatedVelocity));
    float massCoefficient = club.mass / (club.mass + mass);
    float velocitybx = translatedVelocity * massCoefficient * ((1 + restitution)*pow(cos(club.loft), 2) + (2.f / 7.f) * pow(sin(club.loft), 2));
    float velocityby = translatedVelocity * massCoefficient * sin(club.loft)*cos(club.loft)*((5.f / 7.f) + restitution);
    
    float horizontalAngle = atan(clubVelocity.x / clubVelocity.z);
    velocity = glm::vec3(velocitybx*sin(horizontalAngle), velocityby, velocitybx*cos(horizontalAngle));
}

void GolfBall::SetRadius(float radius) {
    sphere.radius = radius;
    SetScale(glm::vec3(radius, radius, radius));
    area = glm::pi<float>() * radius * radius;
}
