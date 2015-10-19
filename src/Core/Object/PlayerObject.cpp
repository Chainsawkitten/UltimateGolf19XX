#include "PlayerObject.hpp"

void PlayerObject::TakeDamage(float Pf){
	if (Pf > 10000.f){
		///Damage model: Lose 1HP per 1000 Pascals overpressure.
		float damage = Pf*0.001f;
		health -= damage;
	}
}
float PlayerObject::getHealth(){
	return health;
}

PlayerObject::PlayerObject(glm::vec3 startingPosition){
	SetPosition(startingPosition);
}

PlayerObject::PlayerObject(){

}