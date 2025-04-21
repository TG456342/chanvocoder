#include "settings.h"
typedef float (*WINDOWINGFUNCTIONS)(float index, float total);

float PI = 3.14159265358979323846264338;

float defaultWindowFuncts(float index, float total) {
	return 1;
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

settings::settings() {
	Font.loadFromFile("calibri.ttf");
	pos = { 0,0 };
	size = { 0,0 };
}

void settings::drawPanel(sf::RenderWindow& window, sf::Vector2f posi, sf::Vector2f siz) {
	pos = posi;
	size = siz;


	sf::VertexArray va(sf::TriangleFan);

	sf::Vector2f newPos = pos + sf::Vector2f{ 20, 20 };
	sf::Vector2f newSize = size - sf::Vector2f{ 40, 40 };

	for (int i = 0; i < 120;i++) {

		sf::Vector2f pDir = { cosf(float(i) / 120.f * 3.141592 * 2), sinf(float(i) / 120.f * 3.141592 * 2) };
		if (i < 30) {
			va.append(sf::Vertex(pDir * 20.f + pos + size, sf::Color::Yellow));
		}
		else if (i < 60) {
			va.append(sf::Vertex(pDir * 20.f + sf::Vector2f{ pos.x,pos.y + size.y }, sf::Color::Yellow));
		}
		else if (i < 90) {
			va.append(sf::Vertex(pDir * 20.f + pos, sf::Color::Yellow));
		}
		else if (i < 120) {
			va.append(sf::Vertex(pDir * 20.f + sf::Vector2f{ pos.x + size.x,pos.y }, sf::Color::Yellow));
		}

	}
	window.draw(va);
	va.clear();

	for (int i = 0; i < 120;i++) {

		sf::Vector2f pDir = { cosf(float(i) / 120.f * 3.141592 * 2), sinf(float(i) / 120.f * 3.141592 * 2) };
		if (i < 30) {
			va.append(sf::Vertex(pDir * 18.f + pos + size, sf::Color::Black));
		}
		else if (i < 60) {
			va.append(sf::Vertex(pDir * 18.f + sf::Vector2f{ pos.x,pos.y + size.y }, sf::Color::Black));
		}
		else if (i < 90) {
			va.append(sf::Vertex(pDir * 18.f + pos, sf::Color::Black));
		}
		else if (i < 120) {
			va.append(sf::Vertex(pDir * 18.f + sf::Vector2f{ pos.x + size.x,pos.y }, sf::Color::Black));
		}

	}
	window.draw(va);

	sf::Text tex;
	tex.setFont(Font);
	tex.setFillColor(sf::Color::Yellow);
	tex.setCharacterSize(20);
	tex.setPosition(pos + sf::Vector2f{ -4,-12 });
	tex.setString("settings");
	window.draw(tex);

	tex.setPosition(pos + sf::Vector2f{ -4,-12 } + sf::Vector2f{20,60});
	tex.setString("window width: " + to_string(windowWidth));
	window.draw(tex);
	tex.setPosition(pos + sf::Vector2f{ -4,-12 } + sf::Vector2f{ 20,100 });
	tex.setString("amount of bands: " + to_string(bandCount));
	window.draw(tex);
	tex.setPosition(pos + sf::Vector2f{ -4,-12 } + sf::Vector2f{ 20,140 });
	tex.setString("windowing function: ");
	window.draw(tex);

	newPos = pos + sf::Vector2f{ size.x * 0.10f, 180.f };
	newSize = sf::Vector2f{ size.x*0.80f, size.x * 0.40f };

	va.clear();
	for (int i = 0; i < 120;i++) {

		sf::Vector2f pDir = { cosf(float(i) / 120.f * 3.141592 * 2), sinf(float(i) / 120.f * 3.141592 * 2) };
		if (i < 30) {
			va.append(sf::Vertex(pDir * 20.f + newPos + newSize, sf::Color::Yellow));
		}
		else if (i < 60) {
			va.append(sf::Vertex(pDir * 20.f + sf::Vector2f{ newPos.x,newPos.y + newSize.y }, sf::Color::Yellow));
		}
		else if (i < 90) {
			va.append(sf::Vertex(pDir * 20.f + newPos, sf::Color::Yellow));
		}
		else if (i < 120) {
			va.append(sf::Vertex(pDir * 20.f + sf::Vector2f{ newPos.x + newSize.x,newPos.y }, sf::Color::Yellow));
		}

	}
	window.draw(va);
	va.clear();

	for (int i = 0; i < 120;i++) {

		sf::Vector2f pDir = { cosf(float(i) / 120.f * 3.141592 * 2), sinf(float(i) / 120.f * 3.141592 * 2) };
		if (i < 30) {
			va.append(sf::Vertex(pDir * 18.f + newPos + newSize, sf::Color::Black));
		}
		else if (i < 60) {
			va.append(sf::Vertex(pDir * 18.f + sf::Vector2f{ newPos.x,newPos.y + newSize.y }, sf::Color::Black));
		}
		else if (i < 90) {
			va.append(sf::Vertex(pDir * 18.f + newPos, sf::Color::Black));
		}
		else if (i < 120) {
			va.append(sf::Vertex(pDir * 18.f + sf::Vector2f{ newPos.x + newSize.x,newPos.y }, sf::Color::Black));
		}

	}
	window.draw(va);
	va.clear();

	va.append(sf::Vertex(sf::Vector2f{ 0.5f * newSize.x * 0.9f + newSize.x * 0.05f, newSize.y } + newPos, sf::Color::Yellow));
	va.append(sf::Vertex(sf::Vector2f{ newSize.x * 0.05f, newSize.y } + newPos, sf::Color::Yellow));

	for (float i = 0;i < 400;i++) {
		va.append(sf::Vertex(sf::Vector2f{i*newSize.x*0.9f/400.f+newSize.x*0.05f, windowingFunctions[1](i,400)*newSize.y*-0.9f + newSize.y} + newPos, sf::Color::Yellow));
	}

	va.append(sf::Vertex(sf::Vector2f{ 1.f * newSize.x * 0.9f  + newSize.x * 0.05f, newSize.y } + newPos, sf::Color::Yellow));

	window.draw(va);

}