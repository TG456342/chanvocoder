#include "element.h"

element::element(sf::Vector2f position, sf::Vector2f size2, string text, sf::Color backgroundColor) {
	 
	pos = position;
	size = size2;
	tex = text;
	backGrC = backgroundColor;
	hasBorder = false;
}
element::element(sf::Vector2f position, sf::Vector2f size2, string text, sf::Color backgroundColor, sf::Color borderColor, float borderWidth) {
	pos = position;
	size = size2;
	tex = text;
	backGrC = backgroundColor;
	borderC = borderColor;
	borderW = borderWidth;
	hasBorder = true;
	 
}
element::element(sf::Vector2f position, sf::Vector2f size2, string text, sf::Color backgroundColor, sf::Color borderColor, float borderWidth, sf::Color textColor) {
	pos = position;
	size = size2;
	tex = text;
	backGrC = backgroundColor;
	borderC = borderColor;
	textC = textColor;
	borderW = borderWidth;
	hasBorder = true;
	 
}
element::element(sf::Vector2f position, sf::Vector2f size2, string text, sf::Color backgroundColor, sf::Color textColor) {
	pos = position;
	size = size2;
	tex = text;
	backGrC = backgroundColor;
	textC = textColor;
	hasBorder = false;
}

void element::draw(sf::RenderWindow& window, sf::Font &ARIAL) {
	sf::RectangleShape rect;
	if (hasBorder) {
		rect.setSize({size.x+2*borderW, size.y+2*borderW});
		rect.setPosition({pos.x-borderW,pos.y-borderW});
		rect.setFillColor(borderC);
		window.draw(rect);
	}

	rect.setSize(size);
	rect.setPosition(pos);
	if (hasImage) {
		rect.setTexture(&img);
	}
	else {
		rect.setFillColor(backGrC);
	}
	window.draw(rect);

	sf::Text txt;
	txt.setFillColor(textC);
	txt.setString(tex);
	txt.setFont(ARIAL);
	txt.setCharacterSize(size.y - 4 - borderW * 2);

	//crop & center text
	int i = 0;
	while (txt.getLocalBounds().width > size.x - 9 - borderW * 2&&i<100)
	{
		txt.setCharacterSize(txt.getCharacterSize() - 1);
		i++;
	}
	sf::FloatRect textRect = txt.getLocalBounds();
	/*/rect.setSize({textRect.width,textRect.height});
	rect.setPosition({ pos.x + siz.x / 2-textRect.width / 2, pos.y + txt.getCharacterSize() / 4 });

	rect.setFillColor(sf::Color::Yellow);
	window.draw(rect);*/
	txt.setOrigin(0, textRect.height/2+ txt.getCharacterSize() / 4);
	txt.setPosition(sf::Vector2f({ pos.x,pos.y+size.y/2}));
	window.draw(txt); 
}
bool element::checkHovered(sf::Vector2f mPos) {
	return (mPos.x > pos.x && mPos.x<pos.x + size.x && mPos.y>pos.y && mPos.y < pos.y + size.y);
}