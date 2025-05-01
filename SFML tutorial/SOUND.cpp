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
	sf::Vector2f newPos = pos + sf::Vector2f{ 20, 20 };
	sf::Vector2f newSize = size - sf::Vector2f{ 40, 40 };

	rr.pos = position;
	rr.size = siez;
	rr.txts = text;
	rr.drawRect(window);

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