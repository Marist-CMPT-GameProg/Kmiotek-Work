#include "Planet.h"

using namespace sf;

Texture Planet::texture; //Definition of static member variable

Planet::Planet() : health(100) 
{
	texture.loadFromFile("graphics/planet.png");

	sprite.setTexture(texture);
	sprite.setOrigin(75, 75); //Set origin to the center of the sprite
	sprite.setPosition(512, 512);

	//Atend of the constructor, local variable texture is freed
}

void Planet::hit()
{
	if (health > 0) {
		health--;
		notify(*this, GameEvent::COLLISION_EVENT);
		if (health == 0) {
			destroy();
		}
	}
}

void Planet::destroy()
{
	sprite.setPosition(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
}

void Planet::notify(GameObject const& obj, GameEvent evt)
{
	for (auto observer : observers) {
		(*observer).onNotify(obj, evt);
		//Can also be written as observer->onNotify(obj, evt);
	}
}

void Planet::addObserver(GameObserver* obj)
{
	observers.insert(obj);
}

void Planet::removeObserver(GameObserver* obj)
{
	observers.erase(obj);
}
