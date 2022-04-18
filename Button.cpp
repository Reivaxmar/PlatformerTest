#include "Button.h"

Button::Button(Font& font, const Color& idleColor, const Color& hoverColor, const Color& activeColor, const Color& textColor, const Vector2f& _windowSize)
{
	this->font = font;
	this->text.setFont(this->font);
	this->text.setFillColor(textColor);
	this->text.setCharacterSize(20);
	
	this->idleColor = idleColor;
	this->hoverColor = hoverColor;
	this->activeColor = activeColor;
	
	this->setWindowSize(_windowSize.x, _windowSize.y);
	
	//this->tex.loadFromFile("Asstes/Textures/Button2.png");
}

Button::~Button()
{
	
}

void Button::render(RenderTarget* target) {
	target->draw(this->shape);
	target->draw(this->text);
}

void Button::update() {
	//this->shape.setTexture(&tex);
	const Vector2f mousePos = Vector2f(this->mousePos().x, this->mousePos().y);
	this->state = 0;
	if(this->shape.getGlobalBounds().contains(mousePos)) {
		this->state = 1;
		
		if(Mouse::isButtonPressed(Mouse::Left)) {
			this->lastLeftPressed = true;
		} else {
			if(this->lastLeftPressed) {
				this->state = 2;
			}
			this->lastLeftPressed = false;
		}
	}
	return;
	switch(this->state) {
		case 0:
			//Not inside the button
			this->shape.setFillColor(this->idleColor);
			break;
		case 1:
			//Button hovered
			this->shape.setFillColor(this->hoverColor);
			break;
		case 2:
			//Button pressed
			this->shape.setFillColor(this->activeColor);
			
			break;
	}
	
	
}


void Button::addButton(float x, float y, float width, float height, const String& _text) {
	this->setText(_text);
	this->setSize(width, height);
	this->setPosition(x, y);
	this->update();
}

void Button::addButton(float x, float y, float width, float height, const String& _text, const Color& idleColor, const Color& hoverColor, const Color& activeColor, const Color& textColor) {
	this->setText(_text);
	this->setSize(width, height);
	this->setPosition(x, y);
	this->setColors(idleColor, hoverColor, activeColor, textColor);
	this->update();
}

void Button::setPosition(float x, float y) {
	this->shape.setPosition(Vector2f(x, y));
	
	this->text.setPosition(
		this->shape.getPosition().x + this->shape.getSize().x / 2.f - this->text.getGlobalBounds().width / 2.f, 
		this->shape.getPosition().y + this->shape.getSize().y / 2.f - this->text.getGlobalBounds().height / 2.f
	);
}

void Button::setSize(float width, float height) {
	this->shape.setSize(Vector2f(width, height));
}

void Button::setFont(const Font& _font) {
	this->font = _font;
	
	this->text.setFont(this->font);
}

void Button::setColors(const Color& idleColor, const Color& hoverColor, const Color& activeColor, const Color& textColor) {
	this->idleColor = idleColor;
	this->hoverColor = hoverColor;
	this->activeColor = activeColor;
	this->text.setFillColor(textColor);
}

void Button::setText(const String& _text) {
	this->text.setString(_text);
}

Vector2i Button::mousePos() {
	return Vector2i(Mouse::getPosition().x * (1920 / 1920), Mouse::getPosition().y * (1080 / 1080));
}

void Button::setWindowSize(float width, float height) {
	this->windowSize.x = width;
	this->windowSize.y = height;
}
