#include "Bat.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <sstream> 
#include <cstdlib>

using namespace sf;
using namespace std;

int main()
{
	//Creating video mode object
	VideoMode vm(1920, 1080);
	//Create and open game window
	RenderWindow window(vm, "Pong", Style::Default);
	int score = 0;
	int lives = 3;

	//Creating the bat and positioning at the bottom of the screen
	Bat bat(1920 / 2, 1080 - 20);

	//Text object
	Text hud;

	Font font;
	font.loadFromFile("fonts/ByteBounce.ttf");
	hud.setFont(font);
	hud.setCharacterSize(75);
	hud.setFillColor(Color::White);
	hud.setPosition(20, 20);

	Clock clock;
	while (window.isOpen()) {
		//End game if window closed 
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		//Escape key to exit
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		//Handle press and release of movement keys
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			bat.moveLeft();
		}
		else {
			bat.stopLeft();
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			bat.moveRight();
		}
		else {
			bat.stopRight();
		}

		//Update the bat, ball and hud
		// 
		//Update delta time
		Time dt = clock.restart();
		bat.update(dt);

		//Update HUD text
		stringstream ss;
		ss << "Score: " << score << "   Lives: " << lives;
		hud.setString(ss.str());

		//Draw the bat, ball and hud
		window.clear();
		window.draw(hud);
		window.draw(bat.getShape());
		window.display();

	}
	return 0;
}
