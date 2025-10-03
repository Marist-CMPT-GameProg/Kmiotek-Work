#include "Bat.h" 

using namespace std;

//Constructor to initialize the bat
Bat::Bat(float startX, float startY) : m_Position(startX, startY) , m_FixedY(startY)
{
	//Set bat size
	m_Shape.setSize({ 110.f, 24.f });
	//Set its position
	m_Shape.setPosition(m_Position);
	//Set its color
	m_Shape.setFillColor(Color(220, 180, 80)); 
	//Set thickness of bat outline and its color
	m_Shape.setOutlineThickness(2.f);
	m_Shape.setOutlineColor(Color::Black);

	m_MinX = 0.f;
	m_MaxX = 99999.f;

	m_HasSprite = false;
	syncSprite();
}

//Set bounds for bat movement 
void Bat::setXBounds(float minX, float maxX) {
	m_MinX = min(minX, maxX);
	m_MaxX = max(minX, maxX);
}

void Bat::moveLeft() {m_MovingLeft = true;}
void Bat::moveRight() {m_MovingRight = true;}
void Bat::stopLeft() {m_MovingLeft = false;}
void Bat::stopRight() {m_MovingRight = false;}

//Update the bat using delta time
void Bat::update(Time dt) {
	const float dtSeconds = dt.asSeconds();

	if (m_MovingLeft) {
		m_Position.x -= m_Speed * dtSeconds;
	}
	if (m_MovingRight) {
		m_Position.x += m_Speed * dtSeconds;
	}
	//Keep the bat within horizontal bounds
	m_Position.y = m_FixedY;

	const float maxAllowed = m_MaxX - m_Shape.getSize().x;
	if (m_Position.x < m_MinX) m_Position.x = m_MinX;
	if (m_Position.x > maxAllowed) m_Position.x = maxAllowed;
	m_Shape.setPosition(m_Position);
	syncSprite();
}

FloatRect Bat::getPosition() const {
	return m_Shape.getGlobalBounds();
}

const RectangleShape& Bat::getShape() const {
	return m_Shape;
}

void Bat::setTexture(const Texture& tex, Mirror mirror) {
	m_Sprite.setTexture(tex);
	m_HasSprite = true;

	//Centers bat origin 
	FloatRect lb = m_Sprite.getLocalBounds();
	m_Sprite.setOrigin(lb.width * 0.5f, lb.height * 0.5f);

	// Scale to rectangle size
	Vector2f rsz = m_Shape.getSize();
	float sx = rsz.x / lb.width;
	float sy = rsz.y / lb.height;
	float s = min(sx, sy);

	syncSprite();
}

bool Bat::hasSprite() const {
	return m_HasSprite;
}

const Sprite& Bat::getSprite() const {
	return m_Sprite;
}

void Bat::syncSprite() {
	if (!m_HasSprite) return;
	const FloatRect gb = m_Shape.getGlobalBounds();
	m_Sprite.setPosition(gb.left + gb.width * 0.5f, gb.top + gb.height * 0.5f);
}