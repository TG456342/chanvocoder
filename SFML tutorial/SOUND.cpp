#include "SOUND.h"

SOUND::SOUND(vector<sf::Int16> dat, vector<complex<float>> freqDat) {
	data = dat;
	freqData = freqDat;
	font.loadFromFile("calibri.ttf");
	SB.loadFromSamples(&data[0], data.size(), 1, 44100);
	s.setBuffer(SB);
}

void SOUND::drawSound(sf::RenderWindow& window ,sf::Vector2f pos, sf::Vector2f size, string text) {
	// draw border

	sf::VertexArray va(sf::TriangleFan);

	sf::Vector2f newPos = pos + sf::Vector2f{20, 20};
	sf::Vector2f newSize = size - sf::Vector2f{40, 40};

	for (int i = 0; i < 120;i++) {

		sf::Vector2f pDir = { cosf(float(i) / 120.f * 3.141592 * 2), sinf(float(i) / 120.f * 3.141592 * 2) };
		if (i < 30) {
			va.append(sf::Vertex(pDir * 20.f+pos + size, sf::Color::Yellow));
		}
		else if (i < 60) {
			va.append(sf::Vertex(pDir * 20.f + sf::Vector2f{pos.x,pos.y+size.y}, sf::Color::Yellow));
		}
		else if (i < 90) {
			va.append(sf::Vertex(pDir * 20.f + pos, sf::Color::Yellow));
		}
		else if (i < 120) {
			va.append(sf::Vertex(pDir * 20.f + sf::Vector2f{ pos.x+size.x,pos.y }, sf::Color::Yellow));
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
	tex.setPosition(pos + sf::Vector2f{-4,-12});
	tex.setString(text);
	window.draw(tex);

	if (!showFreq) {
		visualizeAudioData(window, data, newPos, newSize);
	}
	else {
		visualizeComplexData(window, freqData, newPos, newSize);
	}

	position = pos;
	siez = size;
	txt = text;

}

void SOUND::playSound() {

	if (!isPlaying) {
		cout << "playing: " << txt << endl;
		s.play();
		isPlaying = true;
	}else if (isPlaying) {
		cout << "stopped playing: " << txt << endl;
		s.stop();
		isPlaying = false;
	}
}