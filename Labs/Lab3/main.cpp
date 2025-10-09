#include <SFML/Graphics.hpp>
#include "GameObject.hpp"
#include <vector>
#include "Planet.h"

using namespace sf;
using namespace std;

//Game loop and auxiliary functions
void gameLoop(RenderWindow& window, vector<GameObject*>& gameObjects);
void handleInput(RenderWindow& window, vector<GameObject*>& gameObjects);
void updateGame(vector<GameObject*>& gameObjects, Time dt);
void renderGame(RenderWindow& window, vector<GameObject*>& gameObjects);

void initializeGame(vector<GameObject*>& gameObjects);
void finalizeGame(vector<GameObject*>& gameObjects);

int main()
{
	VideoMode vm(1024, 1024);
	RenderWindow window(vm, "Lab #3");

	View view(FloatRect(0, 0, 1024, 1024));
	window.setView(view);

	std::vector<GameObject*> gameObjects;
	initializeGame(gameObjects);
	gameLoop(window, gameObjects);
	return 0;
}

void gameLoop(RenderWindow& window, vector<GameObject*>& gameObjects) {
	Clock clock;

	while (window.isOpen()) {
		Time dt = clock.restart();
		handleInput(window, gameObjects);
		updateGame(gameObjects, dt);
		renderGame(window, gameObjects);
	}
}
void handleInput(RenderWindow& window, vector<GameObject*>& gameObjects) {
	Event event;
	while (window.pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			window.close();
			return;
		case Event::KeyPressed:
			if (event.key.code == Keyboard::Escape) {
				window.close();
				return;
			}
		default:
			break; //intentionally do nothing
		}
		for (auto obj : gameObjects) {
			obj->handleInput(event);
		}
	}
}
void updateGame(std::vector<GameObject*>& gameObjects, Time dt) {
	for (auto obj : gameObjects) {
		obj->update(dt);
	}
	//TODO Check for collisions
}
void renderGame(RenderWindow& window, vector<GameObject*>& gameObjects) {
	window.clear();
	for (auto obj : gameObjects) {
		window.draw(*obj);
	}
	window.display();
}
void initializeGame(vector<GameObject*>& gameObjects) {
	//Add game objects to the vector
	GameObject* planet = new Planet();
	gameObjects.push_back(planet);
}
void finalizeGame(vector<GameObject*>& gameObjects) {
	for (auto obj : gameObjects) {
		delete obj; //Free memory
	}
}