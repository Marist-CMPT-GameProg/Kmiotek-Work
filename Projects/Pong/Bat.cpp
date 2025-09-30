#include "Bat.h" 

//Constructor to initialize the bat
Bat::Bat(float startX, float startY) : m_Position(startX, startY)
{
	//Set bat size
	m_Shape.setSize(Vector2f(50, 5));
	//Set bat color
	m_Shape.setFillColor(Color::Red);
	//Set bat position
	m_Shape.setPosition(m_Position);
}

FloatRect Bat::getPosition()
{
	return m_Shape.getGlobalBounds();
}
RectangleShape Bat::getShape()
{
	return m_Shape;
}
void Bat::moveLeft()
{
	m_MovingLeft = true;
}
void Bat::moveRight()
{
	m_MovingRight = true;
}
void Bat::stopLeft()
{
	m_MovingLeft = false;
}
void Bat::stopRight()
{
	m_MovingRight = false;
}
void Bat::update(Time dt)
{
	//Update bat position
	if (m_MovingLeft)
	{
		m_Position.x -= m_Speed * dt.asSeconds();
	}
	if (m_MovingRight)
	{
		m_Position.x += m_Speed * dt.asSeconds();
	}
	//Make sure the bat doesn't leave the screen
	if (m_Position.x < 0)
	{
		m_Position.x = 0;
	}
	if (m_Position.x > 800 - m_Shape.getSize().x)
	{
		m_Position.x = 800 - m_Shape.getSize().x;
	}
	//Update the bat's position
	m_Shape.setPosition(m_Position);
}