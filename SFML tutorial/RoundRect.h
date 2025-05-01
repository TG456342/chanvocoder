#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<complex>
using namespace std;

class RoundRect
{

private:
	sf::Font font;

public:
	sf::Vector2f pos;
	sf::Vector2f size;
	float border;
	string txts;

	RoundRect(sf::Vector2f p, sf::Vector2f s, float b, string t) {
		font.loadFromFile("calibri.ttf");
		pos = p;
		size = s;
		border = b;
		txts = t;
	}
	void drawRect(sf::RenderWindow &window) {
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
		tex.setFont(font);
		tex.setFillColor(sf::Color::Yellow);
		tex.setCharacterSize(20);
		tex.setPosition(pos + sf::Vector2f{ -4,-12 });
		tex.setString(txts);
		window.draw(tex);
	}

	bool mouseInRect(sf::Vector2f mPos) {
		return (mPos.x > pos.x && mPos.y > pos.y && mPos.x < pos.x + size.x && mPos.y < pos.y + size.y);
	}

};

