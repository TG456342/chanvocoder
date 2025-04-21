#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

using namespace std;

class SOUND
{

private:

	sf::Font font;
	void visualizeAudioData(sf::RenderWindow& window,vector<sf::Int16> data, sf::Vector2f pos, sf::Vector2f size) {
		float highest = 0;
		int highestIndex = 0;
		for (int i = 0; i < data.size();i++) {
			if (abs(data[i]) > highest) {
				highest = abs(data[i]);
				highestIndex = i;
			}
		}

		sf::VertexArray va(sf::Lines);
		size.y /= 2;

		for (int i = 0; i < data.size();i++) {
			va.append(sf::Vertex(sf::Vector2f{ float(i) / data.size() * size.x,+size.y }+pos, sf::Color::Yellow));
			va.append(sf::Vertex(sf::Vector2f{ float(i) / data.size() * size.x , float(data[i]) / highest * size.y + size.y } + pos, sf::Color::Yellow));
		}

		va.append(sf::Vertex(sf::Vector2f{ 0.f  * size.x,+size.y } + pos, sf::Color::Yellow));
		va.append(sf::Vertex(sf::Vector2f{ 1.f* size.x,+size.y } + pos, sf::Color::Yellow));

		window.draw(va);

	}
	bool isPlaying = false;

public:
	sf::SoundBuffer SB;
	sf::Sound s;
	sf::Vector2f position;
	sf::Vector2f siez;
	string txt = "";
	vector<sf::Int16> data;

	SOUND(vector<sf::Int16> data);
	void drawSound(sf::RenderWindow& window, sf::Vector2f pos, sf::Vector2f size, string text);
	void playSound();
	void reloadSound() {
		SB.loadFromSamples(&data[0], data.size(), 1, 44100);
		s.setBuffer(SB);
	}

};

