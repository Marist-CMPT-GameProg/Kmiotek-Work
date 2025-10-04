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

	//Background music
	Music music;
	if (music.openFromFile("audio/backgroundmusic.mp3")) {
		music.setLoop(true);
		music.setVolume(50.f);
		music.play();
	}

	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("audio/hit.mp3");
	Sound hitP1;
	Sound hitP2;
	hitP1.setBuffer(hitBuffer);
	hitP2.setBuffer(hitBuffer);
	hitP1.setVolume(75.f);
	hitP2.setVolume(75.f);
	
	//HUD 
	int scoreTop = 0;
	int scoreBottom = 0;
	int hitsTop = 0;
	int hitsBottom = 0;
	const int WIN_SCORE = 7;
	bool gameOver = false;
	bool chargedTop = false;
	bool chargedBottom = false;
	bool canHitTop = true;
	bool canHitBottom = true;

	//Game text font
	Font font;
	font.loadFromFile("fonts/ByteBounce.ttf");
	Text hudTop, hudBottom, winText, restartText;
	hudTop.setFont(font);
	hudBottom.setFont(font);
	winText.setFont(font);
	restartText.setFont(font);
	hudTop.setCharacterSize(45);
	hudBottom.setCharacterSize(45);
	winText.setCharacterSize(72);
	restartText.setCharacterSize(40);
	hudTop.setFillColor(Color::White);
	hudBottom.setFillColor(Color::White);
	winText.setFillColor(Color::Yellow);
	restartText.setFillColor(Color(240, 240, 240));
	hudTop.setPosition(20.f, 20.f);
	hudBottom.setPosition(vm.width - 280.f, 20.f);
	winText.setString("");

	//Player can press "1" to restart game after a player wins
	restartText.setString("Press 1 to restart");
	{
		FloatRect br = restartText.getLocalBounds();
		restartText.setOrigin(br.left + br.width * 0.5f, br.top + br.height * 0.5f);
		restartText.setPosition(vm.width * 0.5f, vm.height * 0.5f + 90.f);
	}

	//Rink visuals (center line and lines to define boundaries)
	const float margin = 20.f;
	RectangleShape rink({ vm.width - 2 * margin, vm.height - 2 * margin });
	rink.setPosition({ margin, margin });
	rink.setFillColor(Color(200, 225, 255));
	rink.setOutlineColor(Color::White);
	rink.setOutlineThickness(6.f);

	RectangleShape centerLine({ vm.width - 2 * margin, 3.f });
	centerLine.setPosition({ margin, vm.height * 0.5f });
	centerLine.setFillColor(Color(200, 60, 60));

	const float batW = 100.f;            
	const float batH = 100.f;            
	const float minX = margin;
	const float maxX = vm.width - margin;          
	const float startX = (minX + maxX - batW) * 0.5f; 

	const float centerX = vm.width * 0.5f;
	const float centerY = vm.height * 0.5f;

	const float topY = margin;                      
	const float bottomY = vm.height - margin - batH;

	Bat batTop(startX, topY);
	Bat batBottom(startX, bottomY);
	batTop.setXBounds(margin, vm.width - margin);
	batBottom.setXBounds(margin, vm.width - margin);

	Texture batTexture;
	bool batLoaded = batTexture.loadFromFile("graphics/bat.png");
	batTop.setTexture(batTexture, Bat::Facing::Left);
	batBottom.setTexture(batTexture, Bat::Facing::Right);

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

		if (gameOver) {
			if (Keyboard::isKeyPressed(Keyboard::Num1)) {
				//Resets score on game restart
				scoreTop = 0;
				scoreBottom = 0;
				gameOver = false;

				winText.setString("");

				//Puck is reset back to center
				bool serveDown;
				if (rand() % 2 == 0) serveDown = true;
				else                      serveDown = false;
				ball.reset(centerX, centerY, serveDown);

				music.play();
			}
		}

		if (!gameOver) {
			batTop.stopLeft();  batTop.stopRight();
			batBottom.stopLeft(); batBottom.stopRight();

			//Top player movement keys (A to move left, D to move right)
			if (Keyboard::isKeyPressed(Keyboard::A)) batTop.moveLeft();  else batTop.stopLeft();
			if (Keyboard::isKeyPressed(Keyboard::D)) batTop.moveRight(); else batTop.stopRight();

			//Bottom player movement keys (J to move left, L to move right)
			if (Keyboard::isKeyPressed(Keyboard::J)) batBottom.moveLeft();  else batBottom.stopLeft();
			if (Keyboard::isKeyPressed(Keyboard::L)) batBottom.moveRight(); else batBottom.stopRight();
		}

		//Update delta time
		Time dt = clock.restart();
		if (!gameOver) {
			//Update the bats and ball
			batTop.update(dt);
			batBottom.update(dt);
			ball.update(dt);
		}

		//Handle scoring and lives
		//Bottom player misses, top player scores
		if (!gameOver && ball.getPosition().top > window.getSize().y) {
			scoreTop = scoreTop + 1;

			if (scoreTop >= WIN_SCORE) {
				gameOver = true;
				winText.setString("Player 1 Wins!");
				FloatRect b = winText.getLocalBounds();
				winText.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
				winText.setPosition(vm.width * 0.5f, vm.height * 0.5f);

				music.stop();
			}

			ball.reset(centerX, centerY, true);  
		}
		//Top player misses, bottom player scores
		if (!gameOver && ball.getPosition().top < 0) {
			scoreBottom = scoreBottom + 1;

			if (scoreBottom >= WIN_SCORE) {
				gameOver = true;
				winText.setString("Player 2 Wins!");
				FloatRect b = winText.getLocalBounds();
				winText.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
				winText.setPosition(vm.width * 0.5f, vm.height * 0.5f);

				music.stop();
			}

			ball.reset(centerX, centerY, false);  
		}

		if (ball.getPosition().left < 0 ||
			ball.getPosition().left + ball.getPosition().width > window.getSize().x) {
			ball.reboundSides();
		}

		bool hitTopNow = ball.getPosition().intersects(batTop.getPosition());
		bool hitBottomNow = ball.getPosition().intersects(batBottom.getPosition());

		if (hitTopNow && canHitTop && !gameOver){
			ball.reboundBatOrTop();
			hitP1.play();

			if (chargedTop) {
				ball.applySpeedBoost(ball.getBaseSpeed() * 0.5f);
				chargedTop = false;
				scoreTop = 0;
				batTop.setGlow(false);
			}
			else {
				scoreTop = scoreTop + 1;
				if (scoreTop >= 3) {
					chargedTop = true;
					batTop.setGlow(true);
				}
			}
			canHitTop = false;
		}
		if (hitBottomNow && canHitBottom && !gameOver){
			ball.reboundBatOrTop();
			hitP2.play();

			if (chargedBottom) {
				ball.applySpeedBoost(ball.getBaseSpeed() * 0.5f);
				chargedBottom = false;
				scoreBottom = 0;
				batBottom.setGlow(false);
			}
			else {
				scoreBottom = scoreBottom + 1;
				if (scoreBottom >= 3) {
					chargedBottom = true;
					batBottom.setGlow(true);
				}
			}
			canHitTop = false;
		}

		if (!hitTopNow) canHitTop = true;
		if (!hitBottomNow) canHitBottom = true;

		//Update the HUD
		{
			const float marginX = 20.f;
			const float marginY = 20.f;
			stringstream sst, ssb;
			sst << "P1 (Top)  Score: " << scoreTop;
			ssb << "P2 (Bottom)  Score: " << scoreBottom;
			hudTop.setString(sst.str());
			hudBottom.setString(ssb.str());

			FloatRect lt = hudTop.getLocalBounds();
			hudTop.setOrigin(lt.left, lt.top);

			FloatRect lb = hudBottom.getLocalBounds();
			hudBottom.setOrigin(lb.left + lb.width, lb.top);

			hudTop.setPosition(marginX, marginY);
			hudBottom.setPosition(vm.width - marginX, marginY);
		}
		
		
		//Draw the bat, ball and hud
		window.clear(Color(10, 30, 60));
		window.draw(rink);
		window.draw(centerLine);
		window.draw(hudTop);
		window.draw(hudBottom);

		if (batTop.isGlowOn()) batTop.drawGlow(window, 2.f);
		if (batBottom.isGlowOn()) batBottom.drawGlow(window, 2.f);

		window.draw(batTop.getSprite());
		window.draw(batBottom.getSprite());
		window.draw(ball.getShape());

		if (gameOver) {
			window.draw(winText);
			window.draw(restartText);
		}
		window.display();
	}
	return 0;
}
