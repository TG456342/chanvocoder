#include <iostream>
#include<vector>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<string>
#include<complex>
#include"SOUND.h"
#include"settings.h"
using namespace std;
typedef float (*WINDOWINGFUNCTIONS)(float index, float total);
#define euler = 2.718; 

sf::RenderWindow window(sf::VideoMode(1600, 1000), "vocoder STEM", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);

const double PI = acos(-1);

vector<complex<float>> fft(vector<complex<float>>& a, bool invert = false) {
	if (a.size() == 0) {
		return a;
	}
	int power = ceil(log2f(a.size()));
	//cout << "power = " << power << endl;
	while (a.size() < pow(2, power)) {
		a.push_back(complex<float>(0,0));
	}
	int n = a.size();
	if (n == 1)
		return { a[0] };

	vector<complex<float>> a_even(n / 2), a_odd(n / 2);
	for (int i = 0; i < n / 2; ++i) {
		a_even[i] = a[2 * i];
		a_odd[i] = a[2 * i + 1];
	}

	vector<complex<float>> y_even = fft(a_even, invert);
	vector<complex<float>> y_odd = fft(a_odd, invert);

	vector<complex<float>> y(n);
	float angle = 2 * PI / n * (invert ? -1 : 1);
	complex<float> w(1), wn(cos(angle), sin(angle));

	for (int k = 0; k < n / 2; ++k) {
		complex<float> t = w * y_odd[k];
		y[k] = y_even[k] + t;
		y[k + n / 2] = y_even[k] - t;
		w *= wn;

		if (!invert) {
			y[k] /= 2.0;
			y[k + n / 2] /= 2.0;
		}
	}

	return y;
}

vector<complex<float>> FFT(vector<sf::Int16> data) {

	vector<complex<float>> data2;

	for (int i = 0; i < data.size();i++) {
		data2.push_back(complex<float>(data[i], 0));
	}

	data2 = fft(data2);

	int a = data2.size() / 2;

	for (int i = 0; i < a;i++) {
		data2.pop_back();
	}

	return data2;
}

vector<sf::Int16> IFFT(vector<complex<float>> data2) {

	int a = data2.size();
	for (int i = a-1; i >= 0;i--) {
		data2.push_back(complex<float>(-data2[i].real(),data2[i].imag()));
	}

	data2 = fft(data2,1);


	vector<sf::Int16> data;

	for (int i = 0; i < data2.size();i++) {
		data.push_back(data2[i].real());
	}
	return data;
}

vector<complex<float>> suppressNoise(vector<complex<float>> data) {
	float highest = 0;
	for (int i = 0; i < data.size();i++) {
		if (abs(data[i]) > highest) {
			highest = abs(data[i]);
		}
	}
	for (int i = 0; i < data.size();i++) {
		if (abs(data[i]) < highest / 6000) {
			data[i] = complex<float>(0,0);
		}
	}
	return data;
}

void visualizeAudioData(vector<sf::Int16> data, sf::Vector2f pos, sf::Vector2f size) {
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
		va.append(sf::Vertex(sf::Vector2f{float(i)/data.size()*size.x,0} + pos, sf::Color::Yellow));
		va.append(sf::Vertex(sf::Vector2f{float(i)/data.size()*size.x, float(data[i])/highest*size.y} + pos, sf::Color::Yellow));
	}

	window.draw(va);

}

void visualizeComplexData(vector<complex<float>> dataC, sf::Vector2f pos, sf::Vector2f size) {
	float highest = 0;
	int highestIndex = 0;

	vector<sf::Int16> data;

	for (int i = 0; i < dataC.size();i++) {
		data.push_back(-abs(dataC[i]));
	}

	visualizeAudioData(data, pos, size);
}

float defaultWindowFunct(float index, float total) {
	return 1;
}

float cosineWindowingFunct(float index, float total) {
	return (cos(index / total * 2 * PI + PI) + 1) / 2.f;
}
float halfCosineWindowingFunct(float index, float total) {
	return (cos(index / total * PI - PI / 2));
}

float defaultOverlapFunct(float index, float total) {
	return 0.5;
}

vector<sf::Int16> vocoder(vector<sf::Int16> modRawData, vector<sf::Int16> carRawData, int windowWidth = 1024, int bandWidth = 32, float windowFunction(float index, float total) = defaultWindowFunct, float overlapFunction(float index, float total) = defaultWindowFunct) {

	// divide in windows
	cout << " divide in windows" << endl;

	int windowingSize = windowWidth;
	int amountOfSamples = modRawData.size();
	if (carRawData.size() < modRawData.size()) {
		amountOfSamples = carRawData.size();
	}



	vector<vector<sf::Int16>> windowedModData;
	int amountOfModWindows = (float(amountOfSamples) / float(windowingSize) + 1) * 2;

	while (modRawData.size()<windowingSize*amountOfModWindows)
	{
		modRawData.push_back(0);
	}

	for (int i = 0; i < amountOfModWindows;i++) {
		windowedModData.push_back({});
		for (int j = 0; j < windowingSize;j++) {
			windowedModData[i].push_back(modRawData[i * windowingSize / 2 + j]);
		}
	}

	vector<vector<sf::Int16>> windowedCarData;
	int amountOfCarWindows = (float(amountOfSamples) / float(windowingSize) + 1) * 2;

	while (carRawData.size() < windowingSize * amountOfModWindows)
	{
		carRawData.push_back(0);
	}

	for (int i = 0; i < amountOfCarWindows;i++) {
		windowedCarData.push_back({});
		for (int j = 0; j < windowingSize;j++) {
			windowedCarData[i].push_back(carRawData[i * windowingSize / 2 + j]);
		}
	}
	// aply windowing function

	for (int i = 0; i < windowedModData.size();i++) {
		for (int j = 0; j < windowedModData[i].size();j++) {
			windowedModData[i][j] *= windowFunction(float(j), float(windowedModData[i].size()));
			//(cos(float(j) / float(windowedModData[i].size()) * PI - PI / 2) + 1) / 2;
		}
	}

	for (int i = 0; i < windowedCarData.size();i++) {
		for (int j = 0; j < windowedCarData[i].size();j++) {
			windowedCarData[i][j] *= windowFunction(float(j), float(windowedCarData[i].size()));
		}
	}

	// get frequenties per window
	cout << "get frequenties per window" << endl;

	vector<vector<complex<float>>> windowedModFreqSpec;
	for (int i = 0; i < windowedModData.size();i++) {
		windowedModFreqSpec.push_back((FFT(windowedModData[i])));
	}

	vector<vector<complex<float>>> windowedCarFreqSpec;
	for (int i = 0; i < windowedCarData.size();i++) {
		windowedCarFreqSpec.push_back((FFT(windowedCarData[i])));
	}

	// get max values per band
	cout << "get max values per band" << endl;

	int amountOfBands = bandWidth;

	vector<vector<complex<float>>> newWindowedCarFreqSpec;

	for (int i = 0; i < windowedModFreqSpec.size();i++) {
		// get highest mod amplitude
		for (int x = 0; x < amountOfBands;x++) {

			float highestMod = 0;
			for (int j = 0; j < windowedModFreqSpec[i].size() / amountOfBands;j++) {
				if (abs(windowedModFreqSpec[i][j]) > highestMod) {
					highestMod = abs(windowedModFreqSpec[i][j + x * windowedModFreqSpec[i].size() / amountOfBands]);
				}
			}

			// get highest car amplitude
			float highestCar = 0;
			for (int j = 0; j < windowedCarFreqSpec[i].size() / amountOfBands;j++) {
				if (abs(windowedCarFreqSpec[i][j]) > highestCar) {
					highestCar = abs(windowedCarFreqSpec[i][j + x * windowedModFreqSpec[i].size() / amountOfBands]);
				}
			}

			// change carrier values 

			newWindowedCarFreqSpec.push_back({});
			for (int j = 0; j < windowedCarFreqSpec[i].size() / amountOfBands;j++) {
				newWindowedCarFreqSpec[i].push_back(windowedCarFreqSpec[i][j + x * windowedModFreqSpec[i].size() / amountOfBands] * highestMod / highestCar);
			}
		}

	}

	// IDFT new carrier signal
	cout << "IDFT new carrier signal" << endl;

	vector<vector<sf::Int16>> windowedModCarData;

	for (int i = 0; i < newWindowedCarFreqSpec.size();i++) {
		windowedModCarData.push_back(IFFT(newWindowedCarFreqSpec[i]));
	}

	// overlap frames
	cout << "overlap frames" << endl;

	vector<sf::Int16> modulatedCarrierSamples;

	for (int i = 0; i < windowedModCarData.size();i++) {
		for (int j = 0; j < windowedModCarData[i].size() / 2;j++) {
			modulatedCarrierSamples.push_back(0);
		}
	}

	for (int i = 0; i < windowedModCarData.size();i++) {
		for (int j = 0; j < windowedModCarData[i].size();j++) {
			modulatedCarrierSamples[i * windowingSize / 2 + j] += windowedModCarData[i][j] * ((cos((float(j) / float(windowedModCarData[i].size()) * PI) - PI / 2) + 1) / 4);
		}
	}



	return modulatedCarrierSamples;
}

vector<sf::Int16> vocoderPath(string modPath, string carPath, int windowWidth = 1024, int bandWidth = 32, float windowFunction(float index, float total) = defaultWindowFunct, float overlapFunction(float index, float total) = defaultWindowFunct) {

	// get raw data from files
	cout << "get raw data from files" << endl;
	sf::SoundBuffer modSB;
	modSB.loadFromFile(modPath);
	vector<sf::Int16> modRawData;
	for (int i = 0; i < modSB.getSampleCount(); i++) {
		modRawData.push_back(modSB.getSamples()[i]);
	}

	sf::SoundBuffer carSB;
	carSB.loadFromFile(carPath);
	vector<sf::Int16> carRawData;
	for (int i = 0; i < carSB.getSampleCount(); i++) {
		carRawData.push_back(carSB.getSamples()[i]);
	}

	return vocoder(modRawData , carRawData, windowWidth,bandWidth,windowFunction,overlapFunction);
}

int main() {

	sf::Event evnt;

	WINDOWINGFUNCTIONS windowingFunctions[] = {

	defaultWindowFunct,
	cosineWindowingFunct,
	halfCosineWindowingFunct

	};

	vector<sf::Int16> vocOutp = vocoderPath("modulator1.wav", "carrier.wav", 512,32,windowingFunctions[1], windowingFunctions[1]);
	sf::SoundBuffer carrierSB;
	carrierSB.loadFromFile("carrier.wav");
	vector < sf::Int16> carrierSamples;
	for (int i = 0; i < carrierSB.getSampleCount();i++) {
		carrierSamples.push_back(carrierSB.getSamples()[i]);
	}
	sf::SoundBuffer modSB;
	modSB.loadFromFile("modulator1.wav");
	vector < sf::Int16> modSamples;
	for (int i = 0; i < modSB.getSampleCount();i++) {
		modSamples.push_back(modSB.getSamples()[i]);
	}


	SOUND carrierSound(carrierSamples);
	SOUND modulSound(modSamples);
	SOUND vocodSound(vocOutp);

	settings vocSet;

	bool prevLeftPressed = false;
	bool prevRightPressed = false;

	// git werkt ook voor karel

	while (window.isOpen()) {

		while (window.pollEvent(evnt)) {
			if (evnt.type == evnt.Closed) {
				window.close();
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			prevLeftPressed = true;
		}
		else {
			if (prevLeftPressed) {
				sf::Vector2f mPos = sf::Vector2f(sf::Mouse::getPosition(window));
				if (mPos.x > carrierSound.position.x && mPos.x < carrierSound.position.x + carrierSound.siez.x && mPos.y > carrierSound.position.y && mPos.y < carrierSound.position.y + carrierSound.siez.y) {
					carrierSound.playSound();
				}
				if (mPos.x > modulSound.position.x && mPos.x < modulSound.position.x + modulSound.siez.x && mPos.y > modulSound.position.y && mPos.y < modulSound.position.y + modulSound.siez.y) {
					modulSound.playSound();
				}
				if (mPos.x > vocodSound.position.x && mPos.x < vocodSound.position.x + vocodSound.siez.x && mPos.y > vocodSound.position.y && mPos.y < vocodSound.position.y + vocodSound.siez.y) {
					vocodSound.playSound();
				}
			}
			prevLeftPressed = false;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			prevRightPressed = true;
		}
		else {
			if (prevRightPressed) {
				sf::Vector2f mPos = sf::Vector2f(sf::Mouse::getPosition(window));
				if (mPos.x > carrierSound.position.x && mPos.x < carrierSound.position.x + carrierSound.siez.x && mPos.y > carrierSound.position.y && mPos.y < carrierSound.position.y + carrierSound.siez.y) {
					string newPath;
					cout << "Give path to new sound: ";
					cin >> newPath;
					cout << newPath << endl;

					sf::SoundBuffer newpathSB;
					newpathSB.loadFromFile(newPath);
					vector < sf::Int16> newpathSamples;
					for (int i = 0; i < newpathSB.getSampleCount();i++) {
						newpathSamples.push_back(newpathSB.getSamples()[i]);
					}
					carrierSound.data = newpathSamples;
					carrierSound.reloadSound();
					vocodSound.data = vocoder(modulSound.data, carrierSound.data);
					vocodSound.reloadSound();

				}
				if (mPos.x > modulSound.position.x && mPos.x < modulSound.position.x + modulSound.siez.x && mPos.y > modulSound.position.y && mPos.y < modulSound.position.y + modulSound.siez.y) {
					string newPath;
					cout << "Give path to new sound: ";
					cin >> newPath;
					cout << newPath << endl;

					sf::SoundBuffer newpathSB;
					newpathSB.loadFromFile(newPath);
					vector < sf::Int16> newpathSamples;
					for (int i = 0; i < newpathSB.getSampleCount();i++) {
						newpathSamples.push_back(newpathSB.getSamples()[i]);
					}
					modulSound.data = newpathSamples;
					modulSound.reloadSound();
					vocodSound.data = vocoder(modulSound.data, carrierSound.data);
					vocodSound.reloadSound();

				}
			}
			prevRightPressed = false;
		}

		window.clear();

		carrierSound.drawSound(window, { 100,100 }, { 1000,200 }, "carrier");
		modulSound.drawSound(window, { 100,400 }, { 1000,200 }, "modulator");
		vocodSound.drawSound(window, { 100,700 }, { 1000,200 }, "vocoded output");
		vocSet.drawPanel(window, { 1200,100 }, {300, 800});

		window.display();

	}

	return 0;

}