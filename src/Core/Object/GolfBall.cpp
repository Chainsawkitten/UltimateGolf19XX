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

void GolfBall::Update(double time, const glm::vec3& wind, std::vector<PlayerObject>& players) {
	if (state == GolfBall::ACTIVE) {
		Move(static_cast<float>(time)* velocity);
		sphere.position = Position();

		if (glm::length(angularVelocity) > 0.0001f) {
			glm::quat deltaQuat = glm::angleAxis(static_cast<float>(time)* glm::length(angularVelocity), glm::normalize(angularVelocity));
			orientation = deltaQuat * orientation;
		}

		glm::vec3 dragForce, magnusForce, acceleration = glm::vec3(0.f, 0.f, 0.f);

		float groundLevel = terrain->GetY(Position().x, Position().z);
		float waterLevel = water->Position().y;

		//Check if in water.
		if ((sphere.position.y - sphere.radius < groundLevel && groundLevel + sphere.radius < waterLevel) || sphere.position.y + sphere.radius < waterLevel) {
			state = GolfBall::OUT;
			return;
		}

		// Check for collision
		if ((sphere.position.y - sphere.radius) < groundLevel){
			float vCritical = 1.8f;
			float e = 0.35f;
			float muSliding = 0.51f;
			float muRolling = 0.096f;
			SetPosition(Position().x, groundLevel + sphere.radius, Position().z);
			sphere.position = Position();
			glm::vec3 eRoh = glm::normalize(terrain->GetNormal(Position().x, Position().z));
			glm::vec3 tangentialVelocity = velocity - glm::dot(velocity, eRoh) * eRoh;
			glm::vec3 eFriction = glm::vec3(0.f, 0.f, 0.f);
			if (glm::length(glm::cross(eRoh, angularVelocity) + tangentialVelocity) > 0.0001f){
				eFriction = glm::normalize(sphere.radius * glm::cross(eRoh, angularVelocity) + tangentialVelocity);
			}
			float vRoh = glm::dot(velocity, eRoh);
			float deltaU = -(e + 1.f) * vRoh;
			float velocityRoh = glm::dot(velocity, eRoh);
			float velocityNormal = glm::dot(velocity, eFriction);
			float uRoh = -e*velocityRoh;

			if (uRoh < vCritical && glm::length(tangentialVelocity) > 0.0001f) {
				glm::vec3 mg = (9.82f*glm::vec3(0.f, -1.f, 0.f));
				glm::vec3 tangentialGravityAcceleration = mg - glm::dot(mg, eRoh)*eRoh;
				glm::vec3 normalGravityAcceleration = glm::dot(mg, eRoh)*eRoh;
				glm::vec3 compareVectorPositive = eFriction - normalize(tangentialVelocity);
				glm::vec3 compareVectorNegative = eFriction - normalize(-tangentialVelocity);
				if ((glm::length(compareVectorNegative) < 0.03f) || (glm::length(compareVectorPositive) < 0.03f)) {
					//sliding
					glm::vec3 tangentialSlidingFrictionDeceleration = -muSliding*eFriction*length(normalGravityAcceleration);
					float alpha = (glm::length(tangentialSlidingFrictionDeceleration)*sphere.radius) / (mass*sphere.radius*sphere.radius*0.4f);
					glm::vec3 velocityAfterFriction = velocity + tangentialSlidingFrictionDeceleration*static_cast<float>(time);
					if (glm::length(velocityAfterFriction) > glm::length(velocity))
						velocity = velocity + tangentialGravityAcceleration*static_cast<float>(time);
					else
						velocity = velocityAfterFriction + tangentialGravityAcceleration*static_cast<float>(time);
					glm::vec3 tangentialVelocityDirection = glm::normalize(velocity);
					angularVelocity = angularVelocity + (alpha*static_cast<float>(time))*(glm::cross(sphere.radius*tangentialVelocityDirection, -eRoh));
				} else {
					//rolling
					glm::vec3 tangentialRollingFrictionDeceleration = -muRolling*eFriction*length(normalGravityAcceleration);
					//glm::vec3 velocityAfterFriction = velocity + (tangentialRollingFrictionDeceleration + tangentialGravityAcceleration)*static_cast<float>(time);
					glm::vec3 velocityAfterFriction = velocity + tangentialRollingFrictionDeceleration*static_cast<float>(time);
					if (glm::length(velocityAfterFriction) > glm::length(velocity))
						velocity = velocity + tangentialGravityAcceleration*static_cast<float>(time);
					else
						velocity = velocityAfterFriction + tangentialGravityAcceleration*static_cast<float>(time);
					glm::vec3 tangentialVelocityDirection = glm::normalize(velocity);
					angularVelocity = (glm::length(velocity) / sphere.radius)*(glm::cross(eFriction, -eRoh));
				}
			}
			else {
				float deltaTime = pow(mass * mass / (fabs(vRoh) * sphere.radius), 0.2f) * 0.00251744f;
				float deltauRoh = uRoh - (velocityRoh);
				float deltaUn = muSliding*deltauRoh;
				float rollUn = (5.f / 7.f)*velocityNormal;
				float slideUn = velocityNormal - deltaUn;

				if (velocityNormal > rollUn){
					velocity = uRoh*eRoh + rollUn*eFriction;
					angularVelocity += muRolling * (deltaU + 9.82f*eRoh.y * deltaTime) / sphere.radius * glm::cross(eRoh, eFriction);
				}
				else {
					velocity = uRoh*eRoh + slideUn*eFriction;
					angularVelocity += muSliding * (deltaU + 9.82f *eRoh.y* deltaTime) / sphere.radius * glm::cross(eRoh, eFriction);
				}
			}
		}
		else {
			// Calculate magnus force.
			float v = glm::length(velocity);
			float u = glm::length(velocity - wind);
			float w = glm::length(angularVelocity);
			glm::vec3 eU = (velocity - wind) / u;
			magnusForce = glm::vec3(0.f, 0.f, 0.f);
			if (u > 0.f && v > 0.f && w > 0.f) {
				float Cm = (sqrt(1.f + 0.31f * (w / v)) - 1.f) / 20.f;
				float Fm = 0.5f * Cm * 1.23f * area * u * u;
				magnusForce = Fm * glm::cross(eU, glm::normalize(angularVelocity));
			}

			// Calculate drag force.
			float cD;
			if (ballType == TWOPIECE)
				cD = v < 65.f ? -0.0051f * v + 0.53f : 0.21f;
			else
				cD = v < 60.f ? -0.0084f * v + 0.73f : 0.22f;
			dragForce = glm::vec3(0.f, 0.f, 0.f);
			if (u > 0.f)
				dragForce = -0.5f * 1.23f * area * cD * u * u * eU;

			// Calculate gravitational force.
			glm::vec3 gravitationForce = glm::vec3(0.f, mass * -9.82f, 0.f);
			// Get acceleration from total force.
			acceleration = (dragForce + magnusForce + gravitationForce) / mass;
			velocity += acceleration * static_cast<float>(time);
		}
	}
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
		//Damage for groinal region
        glm::vec3 distanceV = (Position() - player.Position() + glm::vec3(0.f,2.f,0.f));
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

		distanceV = (Position() - player.Position() + glm::vec3(0.f, 1.f, 0.f));
		distance = glm::length(distanceV);
		//pow(meq, 1.f/3.f) => cube root of meq
		z = distance / (pow(massEquivalent, 1.f / 3.f));
		alpha = 1 + pow((z / 4.5f), 2.f);
		beta = 1 + pow((z / 0.048f), 2.f);
		gamma = 1 + pow((z / 1.35f), 2.f);
		delta = 1 + pow((z / 0.32f), 2.f);
		//float Pf = 8.08f*pow(10.f, 7.f)*alpha;
		Pf = ((8.08e7)*alpha) / sqrt(beta*gamma*delta);
		player.TakeDamage(Pf/5.f);

		distanceV = (Position() - player.Position() + glm::vec3(0.f, 0.f, 0.f));
		distance = glm::length(distanceV);
		//pow(meq, 1.f/3.f) => cube root of meq
		z = distance / (pow(massEquivalent, 1.f / 3.f));
		alpha = 1 + pow((z / 4.5f), 2.f);
		beta = 1 + pow((z / 0.048f), 2.f);
		gamma = 1 + pow((z / 1.35f), 2.f);
		delta = 1 + pow((z / 0.32f), 2.f);
		//float Pf = 8.08f*pow(10.f, 7.f)*alpha;
		Pf = ((8.08e7)*alpha) / sqrt(beta*gamma*delta);
		player.TakeDamage(Pf / 10.f);
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
