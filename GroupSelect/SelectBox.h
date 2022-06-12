#pragma once
#include <SFML/Graphics.hpp>

class SelectBox : public sf::Drawable {
private:
	sf::RectangleShape rect;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	

public:
	SelectBox();
};
