#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <sstream>

using namespace sf;
using namespace std;

//Branch sprites
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class Side { LEFT, RIGHT, NONE };
Side branchPositions[NUM_BRANCHES];
int currentBranchSlot = -1;
Side currentBranchSide = Side::NONE;

//Function to create new branches as old ones are chopped off
void updateBranches(int seed) {
	for (int i = 0; i < NUM_BRANCHES; ++i)
		branchPositions[i] = Side::NONE;

	if (currentBranchSlot == -1) {
		srand((int)time(0) + seed);
		int r = rand() % 3;         
		if (r == 0) {
			currentBranchSide = Side::LEFT;
			currentBranchSlot = 0;
		}
		else if (r == 1) {
			currentBranchSide = Side::RIGHT;
			currentBranchSlot = 0;
		}
		else {
			currentBranchSide = Side::NONE; 
			currentBranchSlot = -1;
		}
	}
	else {
		//Move existing branch down one slot
		currentBranchSlot++;

		//Remove bottom branches
		if (currentBranchSlot >= NUM_BRANCHES) {
			currentBranchSlot = -1;
			currentBranchSide = Side::NONE;
		}
	}
	if (currentBranchSlot >= 0 && currentBranchSide != Side::NONE) {
		branchPositions[currentBranchSlot] = currentBranchSide;
	}
}

enum class GameState { COUNTDOWN, PLAYING, GAMEOVER };
GameState gameState = GameState::COUNTDOWN;
Side playerSide = Side::LEFT;
bool acceptInput = false;
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
	float beeSpeed = -10.0f;

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
	for (int i = 0; i < NUM_BRANCHES; i++)
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
	

	//Chopped log sprite
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog(textureLog);
	spriteLog.setScale(1.5f, 1.5f);
	bool logActive = false;
	float logSpeedX = 0, logSpeedY = 0;

	//Text font
	Font font; font.loadFromFile("graphics/font.ttf");
	Text countdownText, timerText, scoreText, gameOverText;
	countdownText.setFont(font); countdownText.setCharacterSize(100); countdownText.setFillColor(Color::Black);
	timerText.setFont(font); timerText.setCharacterSize(75); timerText.setFillColor(Color::Black);
	scoreText.setFont(font); scoreText.setCharacterSize(75); scoreText.setFillColor(Color::Black);
	gameOverText.setFont(font); gameOverText.setCharacterSize(150); gameOverText.setFillColor(Color::Red);

	//Game state variables
	int score = 0;
	float countdownTime = 5.0f;
	float timeRemaining = 10.0f; 
	bool playerDead = false;
	bool isDucking = false;
	const float playerGroundY = 700.f;
	Clock clock;

	SoundBuffer windBuffer;
	windBuffer.loadFromFile("sound/wind.wav");

	Sound windSound;
	windSound.setBuffer(windBuffer);
	windSound.setLoop(true);
	windSound.setVolume(40.f);
	windSound.play();

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chopSound;
	chopSound.setBuffer(chopBuffer);

	SoundBuffer beeBuffer;
	beeBuffer.loadFromFile("sound/bee.wav");
	Sound beeSound;
	beeSound.setBuffer(beeBuffer);

	//Game loop
	while (window.isOpen())
	{
		//Handle time
		Time dt = clock.restart();
		float delta = dt.asSeconds();
		if (gameState == GameState::COUNTDOWN) {
			countdownTime -= delta;
			if (countdownTime <= 0.f) {
				gameState = GameState::PLAYING;
				acceptInput = true;
				timeRemaining = 15.0f;
			}
		}
		else if (gameState == GameState::PLAYING) {
			if (!playerDead) {
				timeRemaining -= delta;
				if(timeRemaining <= 0.f) {
					timeRemaining = 0.0f;
					playerDead = true;
					gameState = GameState::GAMEOVER;
					spriteGravestone.setPosition(spritePlayer.getPosition().x, spritePlayer.getPosition().y);
				}
			}
			if (!playerDead && currentBranchSlot == NUM_BRANCHES - 1 && currentBranchSide == playerSide)
			{
				playerDead = true;
				gameState = GameState::GAMEOVER;
				spriteGravestone.setPosition(spritePlayer.getPosition().x, spritePlayer.getPosition().y);
			}
		}
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (gameState == GameState::GAMEOVER && event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
				//Restarts game
				gameState = GameState::COUNTDOWN;
				countdownTime = 5.f;
				timeRemaining = 15.f;
				score = 0;
				playerDead = false;
				isDucking = false;
				logActive = false;
				beeActive = false;
				playerSide = Side::LEFT;

				currentBranchSlot = -1;
				currentBranchSide = Side::NONE;
				for (int i = 0; i < NUM_BRANCHES; i++) branchPositions[i] = Side::NONE;

				spritePlayer.setPosition(580, 700);
				spriteGravestone.setPosition(-2000, -2000);
				acceptInput = false; isDucking = false;
				logActive = false; beeActive = false;
				clock.restart();
			}

			if (gameState == GameState::PLAYING &&
				!playerDead &&
				event.type == Event::KeyReleased)
			{
				//Actions
				if (event.key.code == Keyboard::Right) {
					//Chop to the right
					playerSide = Side::RIGHT;
					score++;
					timeRemaining += 0.05f;
					if (timeRemaining > 15.0f) timeRemaining = 15.0f;
					spritePlayer.setPosition(1200, 720);
					updateBranches(score);
					
					//Set log to fly out
					logActive = true;
					spriteLog.setPosition(860, 720);
					logSpeedX = 2000.f; logSpeedY = -1500.f;
				}
				else if (event.key.code == Keyboard::Left) {
					//Chop to the left
					playerSide = Side::LEFT;
					score++;
					timeRemaining += 0.05f;
					if (timeRemaining > 15.0f) timeRemaining = 15.0f;
					spritePlayer.setPosition(580, 720);
					updateBranches(score);

					logActive = true;
					spriteLog.setPosition(860, 720);
					logSpeedX = -2000.f; logSpeedY = -1500.f;
				}
			
			}if (event.key.code == Keyboard::Right || event.key.code == Keyboard::Left) {
				chopSound.play();
			}
		}
		if (logActive) {
			spriteLog.move(logSpeedX * delta, logSpeedY * delta);
			logSpeedY += 3000.f * delta; 
			if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2100 ||
				spriteLog.getPosition().y > 1200) {
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
		if (gameState == GameState::PLAYING)
		{
			if (!beeActive)
			{
				beeSpeed = (rand() % 200) + 200;
				//Bee height
				float height = 630.f;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				//Move bee
				spriteBee.setPosition(
					spriteBee.getPosition().x - (beeSpeed * delta),
					spriteBee.getPosition().y);

				//Set new bee to appear if current bee reaches left edge of the screen
				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}
			}
			//Play bee sound
			if (beeActive)
			{
				beeSound.play();
			}
		}
		//Managing clouds
		//Cloud 1
		if (!cloud1Active)
		{
			//Cloud 1 speed
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
				spriteCloud1.getPosition().x + (cloud1Speed * delta),
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
				spriteCloud2.getPosition().x + (cloud2Speed * delta),
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
				spriteCloud3.getPosition().x + (cloud3Speed * delta),
				spriteCloud3.getPosition().y);
			//Set new cloud to appear if cloud 3 reaches right edge of the screen
			if (spriteCloud3.getPosition().x > 1920)
			{
				cloud3Active = false;
			}
		}
		//Obstacles
		if (!playerDead && currentBranchSlot == NUM_BRANCHES - 1 &&
			currentBranchSide == playerSide){
			//Game over
			playerDead = true;
			gameState = GameState::GAMEOVER;
			//Position gravestone
			spriteGravestone.setPosition(spritePlayer.getPosition().x, spritePlayer.getPosition().y);
		}
		if (!playerDead && spriteBee.getGlobalBounds().intersects(spritePlayer.getGlobalBounds())) {
			//Game over
			playerDead = true;
			gameState = GameState::GAMEOVER;
			spriteGravestone.setPosition(spritePlayer.getPosition().x, spritePlayer.getPosition().y);
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

		for (int i = 0; i < NUM_BRANCHES; i++) {
			float branchY = baseY - ((NUM_BRANCHES - 1 - i) * slotHeight);
			if (branchPositions[i] != Side::NONE) {
				if (branchPositions[i] == Side::LEFT) {
					branches[i].setPosition(trunkX - -63.f, branchY);
					branches[i].setRotation(180);
				}
				else {
					branches[i].setPosition(trunkX + 366.f, branchY);
					branches[i].setRotation(0);
				}
				window.draw(branches[i]);
			}
		}
		if (gameState != GameState::GAMEOVER) {
			window.draw(spritePlayer);
		}
		
		if (gameState == GameState::COUNTDOWN) {
			stringstream ss; ss << "Get Ready: " << int(countdownTime + 1);
			countdownText.setString(ss.str());
			countdownText.setPosition(700, 500);
			window.draw(countdownText);
		}
		else if (gameState == GameState::PLAYING) {
			stringstream timeSS; timeSS << "Time: " << int(timeRemaining);
			timerText.setString(timeSS.str());
			timerText.setPosition(20, 20);
			window.draw(timerText);

			stringstream scoreSS; scoreSS << "Score: " << score;
			scoreText.setString(scoreSS.str());
			scoreText.setPosition(20, 80);
			window.draw(scoreText);
		}    
		else if (gameState == GameState::GAMEOVER) {
			gameOverText.setString("GAME OVER\nScore: " + to_string(score) + "\nPress ESC to Restart");
			gameOverText.setPosition(300, 200);
			//Display game over text
			window.draw(gameOverText);
			//Draw gravestone
			window.draw(spriteGravestone);
		}
		//Draw the bee
		window.draw(spriteBee);
		//Draw the log
		if (logActive) window.draw(spriteLog);
		window.display();
	}
	return 0;
}
