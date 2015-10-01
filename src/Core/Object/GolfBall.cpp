#include "GolfBall.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include "../Geometry/OBJModel.hpp"
#include "glm/gtc/constants.hpp"
#include "../Util/Log.hpp"

GolfBall::GolfBall(BallType ballType, TerrainObject* terrain) : Object() {
    active = false;
    
    modelGeometry = new Geometry::OBJModel("Resources/Models/GolfBall/GolfBall.obj");
    std::string diffusePath = "Resources/Models/GolfBall/Diffuse.png";
    std::string normalPath = "Resources/Models/GolfBall/Normal.png";
    std::string specularPath = "Resources/Models/GolfBall/Specular.png";
    
    restitution = ballType == TWOPIECE ? 0.78f : 0.68f;
    this->terrain = terrain;
    groundLevel = this->terrain->Position().y;
    modelObject = new ModelObject(modelGeometry, diffusePath, normalPath, specularPath);
    modelObject->SetPosition(1.f, 0.f, 1.f);
    
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
        
        float horizontal = -static_cast<float>(time)*angularVelocity.x*(glm::pi<float>());
        float vertical = -static_cast<float>(time)*angularVelocity.y*(glm::pi<float>());
        float tilt = -static_cast<float>(time)*angularVelocity.z*(glm::pi<float>());
        
        if ((sphere.position.y - sphere.radius) < groundLevel){
            float mu = 0.11f;
            glm::vec3 surfaceNormal = glm::vec3(0.f, 1.f, 0.f);
            
            modelObject->SetPosition(sphere.position.x, groundLevel + sphere.radius, sphere.position.z);
            glm::vec3 eRoh = surfaceNormal;
            glm::vec3 er = glm::normalize(glm::cross(velocity, eRoh));
            glm::vec3 velocityCrosseRoh = glm::normalize(cross(velocity, eRoh));
            
            glm::vec3 velocityProjectedOneRoh = glm::dot(velocity, eRoh)*eRoh;
            glm::vec3 velocityAfterCollisionProjectedOneRoh = restitution*velocityProjectedOneRoh;
            velocity = velocity + (velocityAfterCollisionProjectedOneRoh - velocityProjectedOneRoh)*(eRoh);
            //angularVelocity = ((mass*sphere.radius)/(2*sphere.radius))*(velocityAfterCollisionProjectedOneRoh - velocityProjectedOneRoh)*(glm::cross(glm::cross(eRoh,eNormal),eNormal));
            
        }
        
        //float e = 0.f;
        //float mu = 0.51f;
        //this->SetPosition(sphere.position.x, groundLevel + sphere.radius, sphere.position.z);
        //modelObject->SetPosition(this->Position());
        
        //if (ballType == TWOPIECE){
        //	float e = 0.78f;
        
        //}
        //else {
        //	float e = 0.68f;
        
        //}
        
        //glm::vec3 eNormal = glm::vec3(0.f, 1.f, 0.f);
        //glm::vec3 eRoh = glm::vec3(0.f, 1.f, 0.f);
        //glm::vec3 muNormal = mu*eNormal;
        //glm::vec3 velocityProjectedOneRoh = glm::dot(velocity, eRoh)*eRoh;
        //glm::vec3 velocityAfterCollisionProjectedOneRoh = e*velocityProjectedOneRoh;
        //velocity = velocity + (velocityAfterCollisionProjectedOneRoh - velocityProjectedOneRoh)*(eRoh + muNormal);
        
        modelObject->Rotate(horizontal, vertical, tilt);
        
        float v = glm::length(velocity);
        float u = glm::length(velocity - wind);
        float w = glm::length(angularVelocity);
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
    
    modelObject->Rotate(0.2f, 0.f, 0.f);
}

void GolfBall::Render(Camera* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane) const{
    modelObject->Render(camera, screenSize, clippingPlane);
}

void GolfBall::Strike(ClubType club, glm::vec3 clubVelocity) {
    active = true;
    float translatedVelocity = sqrt(pow(clubVelocity.x, 2) + pow(clubVelocity.z, 2));
    
    angularVelocity = glm::vec3(0.f, 0.f, -360.f*(5.f / 7.f)*(sin(club.loft)*translatedVelocity));
    float massCoefficient = club.mass / (club.mass + mass);
    float velocitybx = translatedVelocity*massCoefficient*((1 + restitution)*pow(cos(club.loft), 2) + (2.f / 7.f)*pow(sin(club.loft), 2));
    float velocityby = translatedVelocity*massCoefficient*sin(club.loft)*cos(club.loft)*((5.f / 7.f) + restitution);
    
    float horizontalAngle = atan(clubVelocity.x / clubVelocity.z);
    velocity = glm::vec3(velocitybx*sin(horizontalAngle), velocityby, velocitybx*cos(horizontalAngle));
}

void GolfBall::SetRadius(float radius) {
    sphere.radius = radius;
    modelObject->SetScale(glm::vec3(radius, radius, radius));
    area = glm::pi<float>() * radius * radius;
}
