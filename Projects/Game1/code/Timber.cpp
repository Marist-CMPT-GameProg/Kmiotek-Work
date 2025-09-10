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

	//Creating the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png")
		Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	bool beeActive = false;
	//Set bee speed
	float beeSpeed = 0.0f;

	//Making cloud sprites
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	spriteCloud1.setTexture(textureCloud);
	Sprite spriteCloud2;
	spriteCloud2.setTexture(textureCloud);
	Sprite spriteCloud3;
	spriteCloud3.setTexture(textureCloud);

	//Positioning clouds on screen at different heights
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);

	//Are the clouds on screen
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	//Cloud speed
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	//Time variables
	Clock clock;

	while (window.isOpen())
	{
		//Track player input
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//Measure time
		Time dt = clock.restart();

		//Setup bee
		if (!beeActive)
		{
			srand((int)time(0));
			beeSpeed = (rand() % 200) + 200;

			//Bee height
			srand((int)time(0) * 10);
			float height = (rand() % 500) + 500;
			spriteBee.setPosition(2000, height);
			beeActive = true;
		}
		else
		{
			//Move bee
			spriteBee.setPosition(
				spriteBee.getPosition().x -
				(beeSpeed * dt.asSeconds()),
				spriteBee.getPosition().y);

			//Set new bee to appear if current bee reaches left edge of the screen
			if (spriteBee.getPosition().x < -100)
			{
				beeActive = false
			}
		}


	}

}

