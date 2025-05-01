#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<complex>
#include"RoundRect.h"

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
	void visualizeComplexData(sf::RenderWindow& window, vector<complex<float>> dataC, sf::Vector2f pos2, sf::Vector2f size2) {
		float highest = 0;
		int highestIndex = 0;

		vector<sf::Int16> data2;

		for (int i = 0; i < dataC.size();i++) {
			data2.push_back(-abs(dataC[i]));
		}

		visualizeAudioData(window,data2, pos2, size2);
	}

	bool isPlaying = false;

public:
	sf::SoundBuffer SB;
	sf::Sound s;
	sf::Vector2f position = {0,0};
	sf::Vector2f siez{ 0,0 };
	string txt = "";
	vector<sf::Int16> data;
	vector<complex<float>> freqData;
	RoundRect rr = RoundRect(position,siez,2,"");


	bool showFreq = false;

	SOUND(vector<sf::Int16> data, vector<complex<float>> freqDat);
	void drawSound(sf::RenderWindow& window, sf::Vector2f pos, sf::Vector2f size, string text);
	void playSound();
	void reloadSound() {
		SB.loadFromSamples(&data[0], data.size(), 1, 44100);
		s.setBuffer(SB);
	}

	bool checkIfMouseIn(sf::Vector2f mPos) {
		if (mPos.x > position.x && mPos.y > position.y && mPos.x < position.x + siez.x && mPos.y < position.y + siez.y) {
			return 1;
		}
		return 0;
	}

};

