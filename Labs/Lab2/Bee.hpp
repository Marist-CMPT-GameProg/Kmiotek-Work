#pragma once 

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>


enum class Direction { CLOCKWISE, COUNTERCLOCKWISE };

class Bee {
public:
	//Constructor
	Bee(const sf::Texture& texture) 
		: sprite(texture), direction(Direction::CLOCKWISE),
		orbitCenter(1024.f / 2.f, 1024.f / 2.f),
		orbitRadius(150.f), orbitSpeed(1.5f), orbitAngle(0.f) {
		sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
	}

	void updateBee(float dt) {
		if (direction == Direction::CLOCKWISE)
			orbitAngle += orbitSpeed * dt;
		else
			orbitAngle -= orbitSpeed * dt;

		float x = orbitCenter.x + orbitRadius * std::cos(orbitAngle);
		float y = orbitCenter.y + orbitRadius * std::sin(orbitAngle);
		sprite.setPosition(x, y);
	}

	void changeDirection() {
		if (direction == Direction::CLOCKWISE)
			direction = Direction::COUNTERCLOCKWISE;
		else
			direction = Direction::CLOCKWISE;
	}
	sf::Sprite& getSprite() {
		return sprite;
	}
private:
	sf::Sprite sprite;
	Direction direction;
	sf::Vector2f orbitCenter;
	float orbitRadius;
	float orbitSpeed;
	float orbitAngle;
};


