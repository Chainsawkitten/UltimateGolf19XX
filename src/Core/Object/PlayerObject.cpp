#include "PlayerObject.hpp"

void PlayerObject::TakeDamage(float Pf){
	if (Pf > 10000.f){
		///Damage model: Lose 1HP per 1000 Pascals overpressure.
		float damage = Pf*0.001f;
		Log() << "I was hit by a pressure wave: " << Pf << " and took " << damage << "points of damage.\n";
		health -= damage;
		Log() << "My current HP: " << health << "\n";
		if (health < 0.f)
			Log() << "I am now ded, oh nooes!\n";
	}
	else if ((Pf > 7000.f) && (Pf < 10000.f))
		Log() << "I am knocked over, oh noeees!\n";
	else
		Log() << "I was not affected by the blast. Oh noeees!\n";
}