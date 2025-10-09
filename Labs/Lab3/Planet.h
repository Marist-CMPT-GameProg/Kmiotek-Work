#pragma once

#include <set> //Container of unique elements, not allowed to contain duplicates
#include "GameObject.hpp"
#include "GameObserver.h"

class Planet : public GameObject
{
	static sf::Texture texture; //Static member variable, shared by all instances of Planet class
public:
	Planet();
	void hit();
	void destroy();
	void notify(GameObject const& obj, GameEvent evt);
	void addObserver(GameObserver* obj);
	void removeObserver(GameObserver* obj);

private:
	//List of observers
	std::set<GameObserver*> observers;
	int health;

};