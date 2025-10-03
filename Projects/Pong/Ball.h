#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Ball
{
public:
    Ball(float startX, float startY);

    FloatRect getPosition() const;

    const CircleShape& getShape() const;

    void reboundSides();
    void reboundBatOrTop();
    void reboundBottom();

    void update(Time dt);

    void reset(float centerX, float centerY, bool serveDown);

    void setBaseSpeed(float pixelPerSecond);
    float getBaseSpeed() const;

    void applySpeedBoost(float amount);

private:
    Vector2f   m_Position;
    CircleShape m_Shape;

    float m_DirectionX;    
    float m_DirectionY;  

    float m_BaseSpeed;
    float m_Speed;
};