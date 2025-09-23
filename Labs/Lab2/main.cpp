// Environment configuration
// PATH=..\..\SFML-2.6.2\bin;$(PATH)
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

//Global bee orbit variables
Vector2f orbitCenter(1024.f / 2.f, 1024.f / 2.f);
float orbitRadius = 150.0f;
float orbitSpeed = 1.5f;
float orbitAngle = 0.f;

enum class direction { CLOCKWISE, COUNTERCLOCKWISE };
direction beeDirection = direction::CLOCKWISE;

void updateBee(Sprite& bee, float dt) {
	if (beeDirection == direction::CLOCKWISE)
		orbitAngle += orbitSpeed * dt;
	else
		orbitAngle -= orbitSpeed * dt;

	float x = orbitCenter.x + orbitRadius * cos(orbitAngle);
	float y = orbitCenter.y + orbitRadius * sin(orbitAngle);
	bee.setPosition(x, y);

}

void changeBeeDirection() {
	if (beeDirection == direction::CLOCKWISE)
		beeDirection = direction::COUNTERCLOCKWISE;
	else
		beeDirection = direction::CLOCKWISE;
}

int main() {
	RenderWindow window(VideoMode(500, 500), "Lab2 Window");

	Clock clock;

	Texture textureGrass;
	textureGrass.loadFromFile("resources/grass.png");
	Sprite grass(textureGrass);

	Texture textureSunflower;
	textureSunflower.loadFromFile("resources/sunflower.png");
	Sprite sunflower(textureSunflower);

	Texture textureBee;
	textureBee.loadFromFile("resources/bee.png");
	Sprite bee(textureBee);

	sunflower.setPosition(
		1024.f / 2.f - sunflower.getGlobalBounds().width / 2.f,
		1024.f / 2.f - sunflower.getGlobalBounds().height / 2.f
	);

	updateBee(bee, 0.f);

	View view(FloatRect(0.f, 0.f, 1024.f, 1024.f));
	window.setView(view);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) 
				window.close();

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
				changeBeeDirection();
		}

		float dt = clock.restart().asSeconds();

		updateBee(bee, dt);
		
		window.clear();
		window.draw(grass);
		window.draw(sunflower);
		window.draw(bee);
		window.display();
	}
	return 0;
}

