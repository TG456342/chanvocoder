#include "settings.h"
typedef float (*WINDOWINGFUNCTIONS)(float index, float total);

float PI = 3.14159265358979323846264338;

float defaultWindowFuncts(float index, float total) {
	return 0.5;
}

float cosineWindowingFuncts(float index, float total) {
	return (cos(index / total *2* PI+PI)+1)/2.f;
}
float halfCosineWindowingFuncts(float index, float total) {
	return (cos(index / total * PI - PI / 2));
}
vector<WINDOWINGFUNCTIONS> windowingFunctions = {

	defaultWindowFuncts,
	cosineWindowingFuncts,
	halfCosineWindowingFuncts

};

settings::settings(sf::Vector2f p, sf::Vector2f s) {
	Font.loadFromFile("calibri.ttf");
	pos = p;
	size = s;
	backGr.pos = p;
	backGr.size = s;

	sf::Vector2f newPos = pos + sf::Vector2f{ size.x * 0.10f, 180.f };
	sf::Vector2f newSize = sf::Vector2f{ size.x * 0.80f, size.x * 0.40f };

	windowFunctRect.pos = newPos;
	windowFunctRect.size = newSize;
	elements[0].pos = pos + sf::Vector2f{ -4,-18 } + sf::Vector2f{ 20,60 };
	elements[0].size = { float(backGr.size.x) * 0.7f,24 };
	elements[0].tex = "window width: " + to_string(windowWidth);
	elements[0].textC = sf::Color::Yellow;
	elements[1].pos = pos + sf::Vector2f{ -4,-18 } + sf::Vector2f{ 20,100 };
	elements[1].size = { backGr.size.x * 0.7f,24 };
	elements[1].tex = "amount of bands: " + to_string(bandCount);
	elements[1].textC = sf::Color::Yellow;
	elements[2].pos = pos + sf::Vector2f{ -4,-18 } + sf::Vector2f{ 20,140 };
	elements[2].size = { backGr.size.x * 0.7f,24 };
	elements[2].tex = "windowing function: ";
	elements[2].textC = sf::Color::Yellow;

}

void settings::drawPanel(sf::RenderWindow& window, sf::Vector2f posi, sf::Vector2f siz) {

	sf::VertexArray va(sf::TriangleFan);

	backGr.drawRect(window);


	elements[0].tex = "window width: " + to_string(windowWidth);
	elements[0].draw(window, Font);
	elements[1].tex = "amount of bands: " + to_string(bandCount);
	elements[1].draw(window, Font);
	elements[2].tex = "windowing function: ";
	elements[2].draw(window, Font);


	windowFunctRect.drawRect(window);

	sf::Vector2f newPos = pos + sf::Vector2f{ size.x * 0.10f, 180.f };
	sf::Vector2f newSize = sf::Vector2f{ size.x * 0.80f, size.x * 0.40f };

	va.clear();

	va.append(sf::Vertex(sf::Vector2f{ 0.5f * newSize.x * 0.9f + newSize.x * 0.05f, newSize.y } + newPos, sf::Color::Yellow));
	va.append(sf::Vertex(sf::Vector2f{ newSize.x * 0.05f, newSize.y } + newPos, sf::Color::Yellow));

	for (float i = 0;i < 400;i++) {
		va.append(sf::Vertex(sf::Vector2f{i*newSize.x*0.9f/400.f+newSize.x*0.05f, windowingFunctions[windowIndex](i,400)*newSize.y*-0.9f + newSize.y} + newPos, sf::Color::Yellow));
	}

	va.append(sf::Vertex(sf::Vector2f{ 1.f * newSize.x * 0.9f  + newSize.x * 0.05f, newSize.y } + newPos, sf::Color::Yellow));

	window.draw(va);

}

void settings::updatePanel(sf::Vector2f mPos, bool left, bool right, int scroll) {
	if (left) {
		if (elements[0].checkHovered(mPos)) {
			windowWidth *= 2;
		}
		if (elements[1].checkHovered(mPos)) {
			bandCount *= 2;
		}
		if (windowFunctRect.mouseInRect(mPos)&&windowIndex<windowingFunctions.size()-1) {
			windowIndex += 1;
		}
	}
	if (right) {
		if (elements[0].checkHovered(mPos)) {
			windowWidth /= 2;
		}
		if (elements[1].checkHovered(mPos)) {
			bandCount /= 2;
		}
		if (windowFunctRect.mouseInRect(mPos)&& windowIndex >0) {
			windowIndex -= 1;
		}
	}
}