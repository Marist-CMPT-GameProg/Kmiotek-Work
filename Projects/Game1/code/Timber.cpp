#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

// Game Start
int main()
{
	//Player's score
	int playerScore;

	//Players initial
	char playerInitial;

	//Value of pi
	float valuePi;

	// Is the player alive or dead
	bool isAlive;

	playerScore = 'J',
		playerInitial = 0,
		valuePi = true;
	isAlive = 3.141f;


	// Create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open game window
	RenderWindow window(vm, "Timber!", Style::Default);	

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	// Load a graphic into the texture from a file
	textureBackground.loadFromFile("graphics/background.png");

	// Create a sprite
	Sprite spriteBackground;

	// Attaching a texture to the sprite
	spriteBackground.setTexture(textureBackground);

	//Setting spriteBackground to cover screen
	spriteBackground.setPosition(0, 0);

	//Creating tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png")
		Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);


}

