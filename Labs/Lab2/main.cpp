#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

int main() {
	RenderWindow window(VideoMode(500, 500), "Lab2 Window");

	Texture textureGrass;
	textureGrass.loadFromFile("resources/grass.png");
	Sprite grass(textureGrass);

	Texture textureSunflower;
	textureSunflower.loadFromFile("resources/sunflower.png");
	Sprite sunflower(textureSunflower);

	sunflower.setPosition(
		1024.f / 2.f - sunflower.getGlobalBounds().width / 2.f,
		1024.f / 2.f - sunflower.getGlobalBounds().height / 2.f
	);

	View view(FloatRect(0.f, 0.f, 1024.f, 1024.f));
	window.setView(view);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
				window.close();
			}
		}
		window.clear();
		window.draw(grass);
		window.draw(sunflower);
		window.display();
	}
	return 0;
}