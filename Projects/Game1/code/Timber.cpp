#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;
using namespace std;

//Creating branches on tree
const int NUM_BRANCHES = 6;
//Branch sprites
Sprite branches[NUM_BRANCHES];
enum class Side { LEFT, RIGHT, NONE };
Side branchPositions[NUM_BRANCHES];

//Function to create new branches as old ones are chopped off
void updateBranches(int seed) {
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}
	srand((int)time(0) + seed);
	int r = rand() % 3;
	if (r == 0) {
		branchPositions[0] = Side::LEFT;
	} else if(r == 1) {
		branchPositions[0] = Side::RIGHT;
	} else {
		branchPositions[0] = Side::NONE;
	}
}
// Game Start
int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open game window
	RenderWindow window(vm, "Timber!", Style::Default);	

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	// Load a graphic into the texture from a file
	textureBackground.loadFromFile("graphics/background.WEBP");

	// Create a sprite
	Sprite spriteBackground;

	// Attaching a texture to the sprite
	spriteBackground.setTexture(textureBackground);

	//Setting spriteBackground to cover screen
	spriteBackground.setPosition(0, 0);

	//Creating tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.WEBP");
		Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	//Creating the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
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

	//Textures for branches and player
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.WEBP");
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setOrigin(220, 20);
	}

	//Player sprite
	Texture texPlayer;
	texPlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texPlayer);
	//Starts player on left side of screen
	spritePlayer.setPosition(580, 720);

	//Game state variables
	int score = 0;
	float timeRemaining = 10.0f;
	Side playerSide = Side::LEFT;

	//Time variables
	Clock clock;

	while (window.isOpen())
	{
		//Track player input
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//Chop to the right
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			playerSide = Side::RIGHT;
			score++;
			timeRemaining += .15f;
				spritePlayer.setPosition(1200, 720);
			updateBranches(score);
		}

		//Chop to the left
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			playerSide = Side::LEFT;
			score++;
			timeRemaining += .15f;
				spritePlayer.setPosition(580, 720);
			updateBranches(score);
		}	

		//Duck to avoid bee obstacle
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			spritePlayer.move(0, 50);
		}

		//Measure time
		Time dt = clock.restart();
		timeRemaining -= dt.asSeconds();
		if (timeRemaining <= 0) {
			timeRemaining = 0;
			window.close();	
		}
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
				beeActive = false;
			}
		}
		//Managing clouds
		//Cloud 1
		if (!cloud1Active)
		{
			//Cloud 1 speed
			srand((int)time(0) * 10);
			cloud1Speed = (rand() % 200);
			//Cloud 1 height
			srand((int)time(0) * 10);
			float height = (rand() % 150);
			spriteCloud1.setPosition(-200, height);
			cloud1Active = true;
		}
		else
		{
			//Move cloud 1
			spriteCloud1.setPosition(
				spriteCloud1.getPosition().x +
				(cloud1Speed * dt.asSeconds()),
				spriteCloud1.getPosition().y);
			//Set new cloud to appear if cloud 1 reaches right edge of the screen
			if (spriteCloud1.getPosition().x > 1920)
			{
				cloud1Active = false;
			}
		}

		//Cloud 2
		if (!cloud2Active)
		{
			//Cloud 2 speed
			srand((int)time(0) * 20);
			cloud2Speed = (rand() % 200);
			//Cloud 2 height
			srand((int)time(0) * 20);
			float height = (rand() % 300) - 150;
			spriteCloud2.setPosition(-200, height);
			cloud2Active = true;
		}
		else
		{
			//Move cloud 2
			spriteCloud2.setPosition(
				spriteCloud2.getPosition().x +
				(cloud2Speed * dt.asSeconds()),
				spriteCloud2.getPosition().y);
			//Set new cloud to appear if cloud 2 reaches right edge of the screen
			if (spriteCloud2.getPosition().x > 1920)
			{
				cloud2Active = false;
			}
		}

		//Cloud 3
		if (!cloud3Active)
		{
			//Cloud 3 speed
			srand((int)time(0) * 30);
			cloud3Speed = (rand() % 200);
			//Cloud 3 height
			srand((int)time(0) * 30);
			float height = (rand() % 450) - 150;
			spriteCloud3.setPosition(-200, height);
			cloud3Active = true;
		}
		else
		{
			//Move cloud 3
			spriteCloud3.setPosition(
				spriteCloud3.getPosition().x +
				(cloud3Speed * dt.asSeconds()),
				spriteCloud3.getPosition().y);
			//Set new cloud to appear if cloud 3 reaches right edge of the screen
			if (spriteCloud3.getPosition().x > 1920)
			{
				cloud3Active = false;
			}
		}
	}   if (branchPositions[NUM_BRANCHES - 1] == playerSide) {
		//Game over
		window.close();
	    }
	if (spriteBee.getGlobalBounds().intersects(spritePlayer.getGlobalBounds())) {
		//Game over
		window.close();
	}
	//Drawing scene
		window.clear();
		window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		window.draw(spriteTree);
		//Draw tree branches
		for (int i = 0; i < NUM_BRANCHES; i++){
			if (branchPositions[i] != Side::NONE){
				branches[i].setPosition(810, (i * 150));
				
				if (branchPositions[i] == Side::LEFT) {
					branches[i].setRotation(180);
				}
				else {
					branches[i].setRotation(0);
				}
				window.draw(branches[i]);
			}
		}
		//Draw the player
		window.draw(spritePlayer);
		//Draw the bee
		window.draw(spriteBee);
		window.display();

		return 0;
}

