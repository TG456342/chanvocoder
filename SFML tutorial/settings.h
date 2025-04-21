#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<functional>

using namespace std;

class settings
{
private:

	sf::Font Font;

	float PI = 3.141592653589793238462643;
	



public:

	int windowWidth = 1024;
	int bandCount = 32;
	int windowIndex = 0;

	sf::Vector2f pos;
	sf::Vector2f size;
	settings();
	void drawPanel(sf::RenderWindow& window, sf::Vector2f posi, sf::Vector2f siz);
	void updatePanel(sf::Vector2f mPos);

};

