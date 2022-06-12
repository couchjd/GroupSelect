#include "SelectBox.h"

SelectBox::SelectBox() {
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(1.f);
}

void SelectBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(rect, states);
}