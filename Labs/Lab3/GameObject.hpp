#pragma once
#include <SFML/Graphics.hpp>

/// <summary>
/// Simple abstract base class for game objects
/// </summary>
/// <remarks>
/// All game objects have a sprite collision rectangle.
/// This class also provides for object-specific game loop operations,
/// such as handling input, updating the object, and drawing the object.
/// </remarks>
class GameObject
	: public sf::Drawable
{
public:
	/// <summary>
	/// </summary>
	/// <param name=""></param>
	virtual void handleInput(sf::Event const& event);

	/// <summary>
	/// Update the state of this object incrementally (i.e., one frame worth of update)
	/// </summary>
	/// <param name="dt">the elapsed time since the last frame in seconds</param>
	virtual void update(sf::Time dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::FloatRect getCollider() const;

protected:
	sf::Sprite sprite;
};
