#ifndef BUTTON_H
#define BUTTON_H

#include "lib.h"

#define B_PRESSED 2
#define B_HOVER 1
#define B_IDLE 0

class Button
{
	public:
		
		//Constructor / Destructor
		Button(Font& font, const Color& idleColor, const Color& hoverColor, const Color& activeColor, const Color& textColor, const Vector2f& _windowSize);
		~Button();
		
		//Functions
		void render(RenderTarget* target);
		void update();
		
		void addButton(float x, float y, float width, float height, const String& _text);
		void addButton(float x, float y, float width, float height, const String& _text, const Color& idleColor, const Color& hoverColor, const Color& activeColor, const Color& textColor);
		void setPosition(float x, float y);
		void setSize(float width, float height);
		void setFont(const Font& _font);
		void setText(const String& _text);
		void setColors(const Color& idleColor, const Color& hoverColor, const Color& activeColor, const Color& textColor);
		
		void setWindowSize(float width, float height);
		
		Vector2f windowSize;
		
		short int state;
		
		Vector2i mousePos();
		
		/*
			0 = idle
			1 = hover
			2 = active
		*/
		
	private:
		
		//Game Objects
		RectangleShape shape;
		//Texture tex;
		
		Font font;
		Text text;
		
		Color idleColor;
		Color hoverColor;
		Color activeColor;
		
		//Variables
		int width;
		int height;
		string contain;
		
		bool lastLeftPressed;
		
};

#endif
