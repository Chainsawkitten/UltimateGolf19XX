#pragma once 
#include "ModelObject.hpp"
#include "../Util/Log.hpp"

class PlayerObject : public Object {
public:
	PlayerObject();
	PlayerObject(glm::vec3 startingPosition);
	void TakeDamage(float Pf);
private:
	float health = 100.f;
};


/** @} */
