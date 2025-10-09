#pragma once
#include "GameObject.hpp"

enum class GameEvent{COLLISION_EVENT};

class GameObserver
{
public:
	//Pure virtual
	virtual void onNotify(GameObject const& obj, GameEvent evt) = 0;
};