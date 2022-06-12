#define _USE_MATH_DEFINES
#include "Grouping.h"
#include "RectActor.h"
#include "SelectBox.h"
#include <iostream>
#include <algorithm>
#include <list>
#include <math.h>

sf::RectangleShape selectRect(sf::Vector2i start, sf::Vector2i end) {
	float xStart = start.x;
	float yStart = start.y;
	float xLen = end.x - xStart;
	float yLen = end.y - yStart;
	sf::RectangleShape rect(sf::Vector2f(xStart, yStart));
	rect.setSize(sf::Vector2f(xLen, yLen));
	return rect;
}

float selectedArea(const std::vector<RectActor> rects) {
	float area = 0.f;
	int size = rects.size();

	float a1 = 0.f;
	float a2 = 0.f;
	for (int x = 0; x < size; ++x) {
		a1 += rects[x].getPosition().x * rects[(x + 1) % size].getPosition().y;
		a2 += rects[x].getPosition().y * rects[(x + 1) % size].getPosition().x;
	}
	area = std::abs(a2 - a1) / 2;
	return area;
}

float moveDir(const sf::Vector2i& start, const sf::Vector2i& end) {
	float dx = end.x - start.x;
	float dy = start.y - end.y;

	float dir = (atan2(dy, dx) * 180) / M_PI;
	return (dir < 0) ? (dir + 360.f) : dir;
}

sf::Vector2f moveVec(const sf::Vector2i& start, const sf::Vector2i& end) {
	float dx = end.x - start.x;
	float dy = end.y - start.y;
	float magnitude = sqrt((dx*dx) + (dy*dy));
	return sf::Vector2f(dx / magnitude, dy / magnitude);
}

sf::Vector2f getCentroid(const std::vector<RectActor> rects) {
	int size = rects.size();
	float x = 0;
	float y = 0;

	if (size == 1) {
		x = rects[0].getPosition().x;
		y = rects[0].getPosition().y;
	}

	if (size == 2) {
		x = (rects[0].getPosition().x + rects[1].getPosition().x) / 2;
		y = (rects[0].getPosition().y + rects[1].getPosition().y) / 2;
	}

	if (size > 2) {
		float t1 = 0.f;
		float t2 = 0.f;
		float area = selectedArea(rects);

		for (int x = 0; x < size; ++x) {
			RectActor r1 = rects[x];
			RectActor r2 = rects[(x + 1) % size];

			float a1 = r1.getPosition().x + r2.getPosition().x;
			float a2 = r1.getPosition().y + r2.getPosition().y;
			float b1 = (r1.getPosition().x * r2.getPosition().y) - (r2.getPosition().x * r1.getPosition().y);

			t1 += a1 * b1;
			t2 += a2 * b1;
		}
		x = t1 / (6 * area);
		y = t2 / (6 * area);
	}
	return sf::Vector2f(x, y);
}

float distance(const sf::Vector2i& start, const sf::Vector2i& end) {
	return sqrt(pow((end.x - start.x), 2) + pow((end.y - start.y), 2));
}

void groupingTest() {
	// create the window 
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	sf::Vector2i selectBoxStart, selectBoxEnd;
	sf::RectangleShape selectBox;
	bool selecting = false;
	bool l_released = false;
	bool r_released = false;
	bool moving = false;

	sf::Vector2i centroid;
	sf::Vector2i destination;

	sf::Clock clock;
	float mDir = 0.f;

	selectBox.setFillColor(sf::Color::Transparent);
	std::vector<RectActor> rects;

	for (int x = 0; x < 4; ++x) {
		RectActor* r = new RectActor;
		rects.push_back(*r);
		rects[x].setSize(sf::Vector2f(20, 20));
		rects[x].setOrigin(rects[x].getSize().x / 2, rects[x].getSize().y / 2);
	}

	rects[0].color = sf::Color::Red;
	rects[1].color = sf::Color::Green;
	rects[2].color = sf::Color::Blue;
	rects[3].color = sf::Color::Yellow;

	for (int x = 0; x < rects.size(); ++x) {
		rects[x].setFillColor(rects[x].color);
	}

	rects[0].setPosition(100.f, 100.f);
	rects[1].setPosition(130.f, 100.f);
	rects[2].setPosition(130.f, 130.f);
	rects[3].setPosition(100.f, 130.f);

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				selecting = true;
				selectBoxStart = sf::Mouse::getPosition(window);
			}

			if (selecting) {
				selectBoxEnd = sf::Mouse::getPosition(window);
				selectBox = selectRect(selectBoxStart, selectBoxEnd);
			}

			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				selecting = false;
				l_released = true;
			}
			
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
				destination = sf::Mouse::getPosition(window);
				moving = true;
			}
		}

		// clear the window with black color
		window.clear(sf::Color::Black);
		selectBox.setPosition((sf::Vector2f)selectBoxStart);
		selectBox.setFillColor(sf::Color::Transparent);
		selectBox.setOutlineColor(sf::Color::Green);
		selectBox.setOutlineThickness(1.f);
		if (selecting)
			window.draw(selectBox);
		if (l_released) {
			l_released = false;
			for (int x = 0; x < rects.size(); ++x) {				
				float rectX = rects[x].getPosition().x + rects[x].getOrigin().x;
				float rectY = rects[x].getPosition().y + rects[x].getOrigin().y;
				
				float x1 = std::min(selectBox.getPosition().x, (selectBox.getPosition().x + selectBox.getPoint(2).x));
				float y1 = std::min(selectBox.getPosition().y, (selectBox.getPosition().y + selectBox.getPoint(2).y));
				float x2 = std::max(selectBox.getPosition().x, (selectBox.getPosition().x + selectBox.getPoint(2).x));
				float y2 = std::max(selectBox.getPosition().y, (selectBox.getPosition().y + selectBox.getPoint(2).y));

				sf::Vector2f p1(x1, y1);
				sf::Vector2f p2(x2, y2);
				
				if ((p1.x < rectX && p2.x > rectX) && (p1.y < rectY && p2.y > rectY)) {
					rects[x].selected = true;
				}

				else {
					rects[x].selected = false;
				}

				if (rects[x].selected)
					rects[x].setFillColor(sf::Color::Cyan);
				else
					rects[x].setFillColor(rects[x].color);
			}
		}
		
		std::vector<RectActor> selected;
		for(const RectActor& rect : rects) {
			if(rect.selected) {
				selected.push_back(rect);
			}
		}

		centroid = (sf::Vector2i)getCentroid(selected);

		if (r_released) {
			r_released = false;
		}

		if(moving) {
			for(RectActor& rect : rects) {
				if(rect.selected) {
					if(distance((sf::Vector2i)rect.getPosition(), destination) > 50.0f) {

						mDir = moveDir((sf::Vector2i)rect.getPosition(), destination);
						rect.setRotation(mDir);

						sf::Vector2f pos = rect.getPosition();
						sf::Vector2f moveTo = moveVec(centroid, destination);
						moveTo.x = moveTo.x;
						moveTo.y = moveTo.y;

						rect.setPosition(pos + sf::Vector2f(moveTo.x, moveTo.y));
					}
					else {
						moving = false;
					}
				}
			}
		}
		//if (moving && distance(centroid, destination) > 5.0f) {
		//	if (clock.getElapsedTime().asSeconds() > 0.03125) {
		//		clock.restart();
		//		mDir = moveDir(centroid, destination);
		//		for (int x = 0; x < rects.size(); ++x) {
		//			if (rects[x].selected) {
		//				rects[x].setRotation(mDir);
		//				sf::Vector2f pos = rects[x].getPosition();
		//				sf::Vector2f moveTo = moveVec(centroid, destination);
		//				moveTo.x = moveTo.x * 10;
		//				moveTo.y = moveTo.y * 10;
		//
		//				rects[x].setPosition(pos + sf::Vector2f(moveTo.x, moveTo.y));
		//			}
		//		}
		//	}
		//}
		//else {
		//	moving = false;
		//}
		//
		//
		//if (centroid == destination) {
		//	moving = false;
		//}

		// draw everything here...
		for(int x = 0; x < rects.size(); ++x){
			window.draw(rects[x]);
		}
		// end the current frame
		window.display();
	}
}