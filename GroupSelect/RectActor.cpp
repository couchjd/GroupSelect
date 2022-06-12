#include "RectActor.h"

void RectActor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(rect, states);
}