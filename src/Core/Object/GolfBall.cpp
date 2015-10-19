#include "GolfBall.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include "glm/gtc/constants.hpp"
#include "../Util/Log.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

GolfBall::GolfBall(BallType ballType, TerrainObject* terrain, Water* water) : ModelObject(modelGeometry = Resources().CreateOBJModel("Resources/Models/GolfBall/GolfBall.obj"),
                                                                            "Resources/Models/GolfBall/Diffuse.png",
                                                                            "Resources/Models/GolfBall/Normal.png",
                                                                            "Resources/Models/GolfBall/Specular.png") {
    state = GolfBall::INITIAL;
    
    restitution = ballType == TWOPIECE ? 0.78f : 0.68f;
    this->terrain = terrain;
    this->water = water;
    origin = glm::vec3(1.f, 0.f, 1.f);
    
    mass = 0.0459f;
    this->ballType = ballType;
    
    sphere.position = Position();
    SetRadius(0.0214f);
}

GolfBall::~GolfBall() {
    Resources().FreeOBJModel(modelGeometry);
}

void GolfBall::Reset(glm::vec3 newOrigin){
    state = GolfBall::INITIAL;
    velocity = glm::vec3(0.f, 0.f, 0.f);
    angularVelocity = glm::vec3(0.f, 0.f, 0.f);
    SetPosition(newOrigin);
    sphere.position = Position();
    orientation = glm::quat();
}


void GolfBall::Render(Camera* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane) const{
    if (state != GolfBall::EXPLODED && state != GolfBall::OUT)
        ModelObject::Render(camera, screenSize, clippingPlane);
}

void GolfBall::Explode(std::vector<PlayerObject>& players){
    //@TODO: Set mass equivalent depending on material used.
    float equivalenceFactor = 1.0f;
    float massEquivalent = mass*equivalenceFactor;
    for (auto &player : players){
        glm::vec3 distanceV = (Position() - player.Position());
        float distance = glm::length(distanceV); 
        //pow(meq, 1.f/3.f) => cube root of meq
        float z = distance / (pow(massEquivalent, 1.f / 3.f));
        float alpha = 1 + pow((z / 4.5f), 2.f);
        float beta = 1 + pow((z / 0.048f), 2.f);
        float gamma = 1 + pow((z / 1.35f), 2.f);
        float delta = 1 + pow((z / 0.32f), 2.f);
        //float Pf = 8.08f*pow(10.f, 7.f)*alpha;
		float Pf = ((8.08e7)*alpha) / sqrt(beta*gamma*delta);
        player.TakeDamage(Pf);
    }
    state = GolfBall::EXPLODED;
}

void GolfBall::Strike(ClubType club, const glm::vec3& clubVelocity) {
    state = GolfBall::ACTIVE;
    
    // Club velocity in strike plane.
    float v = glm::length(clubVelocity);
    if (v > 0.f)
    {
        float sinLoft = sin(club.loft);
        float cosLoft = cos(club.loft);
        
        // Ball velocity.
        float massCoefficient = club.mass / (club.mass + mass);
        float Up = (1.f + restitution) * massCoefficient * v * cosLoft;
        float Un = (2.f / 7.f) * massCoefficient * v * sinLoft;
        
        // Go back from strike plane to 3D.
        glm::vec3 forward = clubVelocity / v;
        glm::vec3 up = glm::cross(forward, glm::cross(glm::vec3(0.f, 1.f, 0.f), forward));
        glm::vec3 ep = glm::normalize(cosLoft * forward + sinLoft * up);
        glm::vec3 en = glm::normalize(sinLoft * forward - cosLoft * up);
        
        // Total velocity.
        velocity = Up * ep + Un * en;
        angularVelocity = -Un / sphere.radius * glm::cross(ep, en);
    } else {
        velocity = glm::vec3(0.f, 0.f, 0.f);
        angularVelocity = glm::vec3(0.f, 0.f, 0.f);
    }
}

float GolfBall::Radius() const {
    return sphere.radius;
}

void GolfBall::SetRadius(float radius) {
    sphere.radius = radius;
    SetScale(glm::vec3(radius, radius, radius));
    area = glm::pi<float>() * radius * radius;
}

glm::mat4 GolfBall::Orientation() const {
    return glm::toMat4(orientation);
}

GolfBall::State GolfBall::GetState() const {
    return state;
}
