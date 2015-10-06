#pragma once 
#include "ModelObject.hpp"
#include "../Util/Log.hpp"

class PlayerObject : public Object {
public:
	void TakeDamage(float Pf);
private:
	int health;
};


/** @} */
