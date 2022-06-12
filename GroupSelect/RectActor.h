#pragma once
#include <SFML/Graphics.hpp>

class RectActor : public sf::Drawable {
private:
	sf::RectangleShape rect;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	
	sf::Color color;
	bool selected;

	sf::Vector2f getPosition() const { return rect.getPosition(); }
	sf::Vector2f getSize() const { return rect.getSize(); }
	sf::Vector2f getOrigin() const { return rect.getOrigin(); }

	template<typename ... Args>
	void setPosition(Args&& ... args) {
		rect.setPosition(std::forward<Args>(args)...);
	}
	template<typename ... Args>
	void setSize(Args&& ... args) {
		rect.setSize(std::forward<Args>(args)...);
	}
	template<typename ... Args>
	void setOrigin(Args&& ... args) {
		rect.setOrigin(std::forward<Args>(args)...);
	}
	template<typename ... Args>
	void setFillColor(Args&& ... args) {
		rect.setFillColor(std::forward<Args>(args)...);
	}
	template<typename ... Args>
	void setRotation(Args&& ... args) {
		rect.setRotation(std::forward<Args>(args)...);
	}
};