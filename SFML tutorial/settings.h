#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<functional>
#include"RoundRect.h"
#include"element.h"

using namespace std;

class settings
{
private:

	sf::Font Font;

	float PI = 3.141592653589793238462643;

	RoundRect backGr = RoundRect({0,0}, {0,0}, 2,"settings");
	RoundRect windowFunctRect = RoundRect({0,0}, {0,0}, 2,"");
	vector<element> elements = {

		element({0,0},{100,30},"window width: " + to_string(windowWidth),sf::Color::White),
		element({0,0},{100,30},"amount of bands: " + to_string(bandCount),sf::Color::Black),
		element({0,0},{100,30},"amount of bands: " + to_string(bandCount),sf::Color::Black)
	
	};

public:

	int windowWidth = 1024;
	int bandCount = 32;
	int windowIndex = 0;

	sf::Vector2f pos;
	sf::Vector2f size;
	settings(sf::Vector2f p, sf::Vector2f s);
	void drawPanel(sf::RenderWindow& window, sf::Vector2f posi, sf::Vector2f siz);
	void updatePanel(sf::Vector2f mPos, bool left, bool right, int scroll);

};

