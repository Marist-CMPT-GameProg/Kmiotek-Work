#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Bat
{
public:
    Bat(float startX, float startY);

    enum class Facing { Right, Left };


    void moveLeft();
    void moveRight();
    void stopLeft();
    void stopRight();

    void update(Time dt);

    FloatRect getPosition() const;            
    const RectangleShape& getShape() const;    

    void setTexture(const Texture& tex, Facing facing);
    bool hasSprite() const;
    const Sprite& getSprite() const;

    //Horizontal movement limits
    void setXBounds(float minX, float maxX);

    void setGlow(bool on);
    bool isGlowOn() const;
    void drawGlow(RenderTarget& target, float pixels) const;

private:
    Vector2f m_Position;
    RectangleShape m_Shape;

    //Movement
    float m_Speed = 1000.f;
    bool  m_MovingLeft = false;
    bool  m_MovingRight = false;

    float m_MinX = 0.f;
    float m_MaxX = 0.f;
    float m_FixedY = 0.f;

    Sprite m_Sprite;
    bool m_HasSprite = false;

    bool m_GlowOn = false;

    void syncSprite();
};