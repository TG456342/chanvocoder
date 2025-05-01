#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class element
{
public:
	sf::Vector2f pos = { 0,0 };
	sf::Vector2f size = { 100,10 };
	string tex = "";
	sf::Color backGrC = sf::Color::Magenta;
	sf::Color borderC;
	sf::Color textC = sf::Color::White;
	float borderW;
	bool hasBorder = false;
	bool hasImage = false;
	sf::Texture img;
	bool clicked = false;

	element() = default;
	element(sf::Vector2f position, sf::Vector2f size2, string text, sf::Color backgroundColor);
	element(sf::Vector2f position, sf::Vector2f size, string text, sf::Color backgroundColor, sf::Color borderColor, float borderWidth);
	element(sf::Vector2f position, sf::Vector2f size, string text, sf::Color backgroundColor, sf::Color borderColor, float borderWidth, sf::Color textColor);
	element(sf::Vector2f position, sf::Vector2f size, string text, sf::Color backgroundColor, sf::Color textColor);

	void draw(sf::RenderWindow& window, sf::Font &ARIAL);
	bool checkHovered(sf::Vector2f mPos);
};


