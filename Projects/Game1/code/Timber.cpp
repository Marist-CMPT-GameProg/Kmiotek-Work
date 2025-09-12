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
Side playerSide = Side::LEFT;
bool acceptInput = true;
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
	textureBackground.loadFromFile("graphics/background.png");

	// Create a sprite
	Sprite spriteBackground;

	// Attaching a texture to the sprite
	spriteBackground.setTexture(textureBackground);

	//Set fixed sizes for background
	spriteBackground.setScale(
		1920.0f / textureBackground.getSize().x,
		1080.0f / textureBackground.getSize().y);
	spriteBackground.setPosition(0, 0);

	//Creating tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setScale(1.5f, 1.5f);
	spriteTree.setPosition(840, -400);

	//Creating the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setScale(0.2f, 0.2f);	
	spriteBee.setPosition(2000, 800);

	bool beeActive = false;
	//Set bee speed
	float beeSpeed = 0.0f;

	//Making cloud sprites
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud1.setScale(0.2f, 0.2f);
	Sprite spriteCloud2;
	spriteCloud2.setTexture(textureCloud);
	spriteCloud2.setScale(0.2f, 0.2f);
	Sprite spriteCloud3;
	spriteCloud3.setTexture(textureCloud);
	spriteCloud3.setScale(0.2f, 0.2f);

	//Positioning clouds on screen at different heights
	spriteCloud1.setPosition(0, 700);
	spriteCloud2.setPosition(0, 900);
	spriteCloud3.setPosition(0, 1000);

	//Are the clouds on screen
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	//Cloud speed
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	//Textures for branches 
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; ++i)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setScale(0.7f, 0.7f);
		branches[i].setOrigin(220, 20);
		branchPositions[i] = Side::NONE; 
	}

	//Player sprite
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	//Set player scale
	spritePlayer.setScale(0.8f, 0.8f);
	//Starts player on left side of screen
	spritePlayer.setPosition(580, 700); 

	//Gravestone sprite
	Texture textureGravestone;
	textureGravestone.loadFromFile("graphics/gravestone.png");
	Sprite spriteGravestone(textureGravestone);
	spriteGravestone.setScale(0.5f, 0.5f);
	spriteGravestone.setPosition(580, 700);

	//Chopped log sprite
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog(textureLog);
	spriteLog.setScale(1.5f, 1.5f);
	bool logActive = false;
	float logSpeedX = 1000.0f, logSpeedY = -500.0f;

	//Game state variables
	int score = 0;
	float timeRemaining = 10.0f; 
		bool playerDead = false;
		bool isDucking = false;
		const float playerGroundY = 700.f;
	Clock clock;

	//Make branches align with trunk
	const float trunkX = 860.0f + 35.0f;
	//Make base of tree ground level
	const float baseY = 720.0f;
	//Distance between each branch
	const float slotHeight = 150.0f;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyReleased && !playerDead)
			{
				//Actions
				if (event.key.code == Keyboard::Right) {
					//Chop to the right
					playerSide = Side::RIGHT;
					score++;
					timeRemaining += 0.15f;
					spritePlayer.setPosition(1200, 720);
					updateBranches(score);
					
					//Set log to fly out
					logActive = true;
					spriteLog.setPosition(860, 720);
					logSpeedX = 1000.0f;
					logSpeedY = -500.0f;
				}
				else if (event.key.code == Keyboard::Left) {
					//Chop to the left
					playerSide = Side::LEFT;
					score++;
					timeRemaining += .15f;
					spritePlayer.setPosition(580, 720);
					updateBranches(score);

					logActive = true;
					spriteLog.setPosition(860, 720);
					logSpeedX = -1000.0f;
					logSpeedY = -500.0f;
				}
			
			}
		}

		Time dt = clock.restart();
		if (!playerDead) timeRemaining -= dt.asSeconds();
		if (timeRemaining <= 0 && !playerDead) {
			playerDead = true;
		}

		//Move log
		if (logActive) {
			spriteLog.setPosition(
				spriteLog.getPosition().x + logSpeedX * dt.asSeconds(),
				spriteLog.getPosition().y + logSpeedY * dt.asSeconds());
			if (spriteLog.getPosition().x < 0 || spriteLog.getPosition().x>1920) {
				logActive = false;
			}
		}

		//Duck to avoid bee obstacle
		if (!playerDead){
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				if (!isDucking) {
					spritePlayer.move(0, 50);
					isDucking = true;
				}
			}
			else {
				if (isDucking) {
					spritePlayer.setPosition(spritePlayer.getPosition().x, playerGroundY);
					isDucking = false;
				}
			}
		}

		//Setup bee
		if (!beeActive)
		{
			srand((int)time(0));
			beeSpeed = (rand() % 200) + 200;
			//Bee height
			srand((int)time(0) * 10);
			float height = 630.f;
			spriteBee.setPosition(2000, height);
			beeActive = true;
		}
		else
		{
			//Move bee
			spriteBee.setPosition(
				spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
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
			float height = (rand() % 150);
			spriteCloud1.setPosition(-200, height);
			cloud1Active = true;
		}
		else
		{
			//Move cloud 1
			spriteCloud1.setPosition(
				spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
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
			float height = (rand() % 300) - 150;
			spriteCloud2.setPosition(-200, height);
			cloud2Active = true;
		}
		else
		{
			//Move cloud 2
			spriteCloud2.setPosition(
				spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
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
			float height = (rand() % 450) - 150;
			spriteCloud3.setPosition(-200, height);
			cloud3Active = true;
		}
		else
		{
			//Move cloud 3
			spriteCloud3.setPosition(
				spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
				spriteCloud3.getPosition().y);
			//Set new cloud to appear if cloud 3 reaches right edge of the screen
			if (spriteCloud3.getPosition().x > 1920)
			{
				cloud3Active = false;
			}
		}
		//Obstacles
		if (!playerDead && branchPositions[NUM_BRANCHES - 1] == playerSide) {
			//Game over
			playerDead = true;
			spritePlayer.setPosition(2000, 720);
			spriteGravestone.setPosition(580, 720);
		}
		if (!playerDead && spriteBee.getGlobalBounds().intersects(spritePlayer.getGlobalBounds())) {
			//Game over
			playerDead = true;
			spritePlayer.setPosition(2000, 720);
			spriteGravestone.setPosition(580, 720);
		}

		//Drawing scene
		window.clear();
		window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		window.draw(spriteTree);

		//Draw tree branches
		const float trunkX = 860.0f;
		const float baseY = 720.0f;
		const float slotHeight = 250.0f;

		for (int i = 0; i < NUM_BRANCHES; ++i) {
			if (branchPositions[i] != Side::NONE) {
				float branchY = baseY - ((NUM_BRANCHES - 1 - i)* slotHeight);

				if (branchPositions[i] == Side::LEFT) {
					branches[i].setPosition(trunkX - -62.0f, branchY);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == Side::RIGHT) {
					branches[i].setPosition(trunkX + 366.0f, branchY);
					branches[i].setRotation(0);
				}
				window.draw(branches[i]);
			}
		}
		//Draw the player and gravestone
		if(!playerDead) window.draw(spritePlayer);
		else window.draw(spriteGravestone);
		//Draw the bee
		window.draw(spriteBee);
		//Draw the log
		if (logActive) window.draw(spriteLog);
		window.display();
	}
	return 0;
}


