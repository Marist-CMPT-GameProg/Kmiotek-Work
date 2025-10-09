#include "GameObject.hpp"
#include <SFML/Graphics.hpp>

void GameObject::handleInput(sf::Event const& event)
{

}

void GameObject::update(sf::Time dt) {

}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
}

sf::FloatRect GameObject::getCollider() const
{
	return sprite.getGlobalBounds();
}