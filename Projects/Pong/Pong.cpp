#include "Bat.h"
#include "Ball.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <sstream> 
#include <cstdlib>
#include <ctime>

using namespace sf;
using namespace std;

static Vector2f rectCenter(const FloatRect& r) {
	return{ r.left + r.width * 0.5f, r.top + r.height * 0.5f };
}

int main()
{
	//Creating randomly generated events for each playthrough
	srand(static_cast<unsigned>(time(nullptr)));
	//Creating video mode object
	VideoMode vm(720, 1080);
	//Create and open game window
	RenderWindow window(vm, "2Player Hockey", Style::Default);
	window.setVerticalSyncEnabled(true);

	//HUD 
	int scoreTop = 0;
	int scoreBottom = 0;
	int livesTop = 3;
	int livesBottom = 3;

	//Text object
	Font font;
	font.loadFromFile("fonts/ByteBounce.ttf");
	Text hudTop, hudBottom;
	hudTop.setFont(font);
	hudBottom.setFont(font);
	hudTop.setCharacterSize(48);
	hudBottom.setCharacterSize(48);
	hudTop.setFillColor(Color::White);
	hudBottom.setFillColor(Color::White);
	hudTop.setPosition(20.f, 20.f);
	hudBottom.setPosition(vm.width - 280.f, 20.f);

	//Rink visuals (center line and lines to define boundaries)
	const float margin = 24.f;
	RectangleShape rink({ vm.width - 2 * margin, vm.height - 2 * margin });
	rink.setPosition({ margin, margin });
	rink.setFillColor(Color(200, 225, 255));
	rink.setOutlineColor(Color::White);
	rink.setOutlineThickness(6.f);

	RectangleShape centerLine({ vm.width - 2 * margin, 3.f });
	centerLine.setPosition({ margin, vm.height * 0.5f });
	centerLine.setFillColor(Color(200, 60, 60));

	const float topY = margin + 96.f;
	const float bottomY = vm.height - margin - 96.f;

	Bat batTop(vm.width / 0.5f, topY);
	Bat batBottom(vm.width / 0.5f, bottomY);
	batTop.setXBounds(margin, vm.width - margin);
	batBottom.setXBounds(margin, vm.width - margin);

	Texture batTexture;
	bool batLoaded = batTexture.loadFromFile("graphics/bat.png");
	batTop.setTexture(batTexture, Bat::Mirror::None);
	batBottom.setTexture(batTexture, Bat::Mirror::Horizontal);

	Ball ball(vm.width / 2.f, vm.height / 2.f);

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
		//Top player movement keys (A to move left, D to move right)
		if (Keyboard::isKeyPressed(Keyboard::A)) batTop.moveLeft();  else batTop.stopLeft();
		if (Keyboard::isKeyPressed(Keyboard::D)) batTop.moveRight(); else batTop.stopRight();

		//Bottom player movement keys (J to move left, L to move right)
		if (Keyboard::isKeyPressed(Keyboard::J)) batBottom.moveLeft();  else batBottom.stopLeft();
		if (Keyboard::isKeyPressed(Keyboard::L)) batBottom.moveRight(); else batBottom.stopRight();

		//Update delta time
		Time dt = clock.restart();
		//Update the bat and ball
		batTop.update(dt);
		batBottom.update(dt);
		ball.update(dt);

		//Handle scoring and lives
		if (ball.getPosition().top > window.getSize().y)
		{
			//Reverse the ball direction
			ball.reboundBottom();
			//Remove a life
			livesBottom--;
			//Check if the player has run out of lives
			if (livesBottom < 1) {
				//Reset top and bottom score
				scoreTop = 0;
				scoreBottom = 0;
				//Reset lives
				livesBottom = 3;
			}
		}
		if (ball.getPosition().top < 0) {
			ball.reboundBatOrTop();
			livesTop--;
			if (livesTop < 1) { 
				livesTop = 3; 
				scoreTop = 0; 
				scoreBottom = 0; 
			}
		}
		if (ball.getPosition().left < 0 ||
			ball.getPosition().left + ball.getPosition().width > window.getSize().x) {
			ball.reboundSides();
		}
		if (ball.getPosition().intersects(batTop.getPosition())) {
			ball.reboundBatOrTop();
			scoreTop++;
		}
		if (ball.getPosition().intersects(batBottom.getPosition())) {
			ball.reboundBatOrTop();
			scoreBottom++;
		}
		//Update the HUD
		{
			stringstream sst, ssb;
			sst << "Top  Score: " << scoreTop << "  Lives: " << livesTop;
			ssb << "Bottom Score: " << scoreBottom << "  Lives: " << livesBottom;
			hudTop.setString(sst.str());
			hudBottom.setString(ssb.str());
		}
		
		
		//Draw the bat, ball and hud
		window.clear(Color(10, 30, 60));
		window.draw(rink);
		window.draw(centerLine);
		window.draw(hudTop);
		window.draw(hudBottom);
		window.draw(batTop.getSprite());
		window.draw(batBottom.getSprite());
		window.draw(ball.getShape());
		window.display();

	}
	return 0;
}
