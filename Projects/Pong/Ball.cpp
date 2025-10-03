#include "Ball.h"
#include <cstdlib>

using namespace sf;

//Constructor to initialize the ball
Ball::Ball(float startX, float startY)
    : m_Position(startX, startY)
    , m_DirectionX(0.f)
    , m_DirectionY(1.f)       
    , m_Speed(400.f)    
{
    m_Shape.setRadius(8.f);
    m_Shape.setOrigin(18.f, 18.f); 
    m_Shape.setFillColor(Color(30, 30, 30));
    m_Shape.setOutlineThickness(2.f);
    m_Shape.setOutlineColor(Color::White);
    m_Shape.setPosition(m_Position);

    int rx = std::rand() % 2;
    if (rx == 0) m_DirectionX = -1.f;
    else         m_DirectionX = 1.f;

    int ry = std::rand() % 2;
    if (ry == 0) m_DirectionY = -1.f;
    else         m_DirectionY = 1.f;
}

FloatRect Ball::getPosition() const {
    return m_Shape.getGlobalBounds();
}

const CircleShape& Ball::getShape() const {
    return m_Shape;
}

void Ball::update(Time dt) {
    const float dtSec = dt.asSeconds();

    m_Position.x += m_DirectionX * m_Speed * dtSec;
    m_Position.y += m_DirectionY * m_Speed * dtSec;

    m_Shape.setPosition(m_Position);
}

void Ball::reboundSides() {
    m_DirectionX = -m_DirectionX;
}

void Ball::reboundBatOrTop() {
    m_DirectionY = -m_DirectionY;
}

void Ball::reboundBottom() {
    m_DirectionY = -m_DirectionY;
}

void Ball::reset(float centerX, float centerY, bool serveDown) {
    //Place ball at center
    m_Position.x = centerX;
    m_Position.y = centerY;
    m_Shape.setPosition(m_Position);

    //Random horizontal direction
    int r = std::rand() % 2;
    if (r == 0) m_DirectionX = -1.f;
    else        m_DirectionX = 1.f;

    //Vertical direction based on who missed
    if (serveDown) m_DirectionY = 1.f;  
    else           m_DirectionY = -1.f;  
}