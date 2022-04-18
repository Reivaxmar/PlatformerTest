#ifndef GAME_H
#define GAME_H

#include "lib.h"
#include "Button.h"
#include "Slider.h"
#include "TextInput.h"

#define MAIN_MENU 0
#define SETTINGS_MENU 1
#define GAME 2

class Game
{
	private:
		
		struct block {
			int t;
			int dir;
			bool tr;
		};
		
		//Variables
		
		//All one tap key variables
		bool MousePressed;
		bool UpPressed;
		bool DownPressed;
		bool Tpressed;
		bool Spressed;
		bool RightPressed;
		bool LeftPressed;
		bool EscPressed;
		
		//Other variables
		int fps;
		int currentTex;
		int resolution;
		block brush;
		bool trBrush;
		
		bool playMode;
		
		map<bool, int> kills;
		
		bool TileArrange;
		
		float sizeRelation;
		
		bool paused;
		
		string saveT;
		string saveDir;
		string saveTr;
		
		ifstream readFile;
		string Info;
		
		int scene;
		
		//Window
		RenderWindow* window;
		VideoMode videoMode;
		Event ev;
		
		//Game objects
		Sprite tile;
		Texture tileT;
		
		Sprite pauseBackground;
		Texture pauseBackTex;
		
		Font font;
		Text text;
		
		View view;
		
		//Private functions
		void initVariables();
		void PauseMenu();
		void initWindow();
		void initGrid();
		void setCam();
		void setPlaceTile();
		void saveCode();
		void loadCode();
		void kill();
		
		void refreshTile(int tPos, bool o = false);
		
		bool touching();
		int strToInt(string s);
		
		float lerp(float a, float b, float t);
		
		Vector2f mousePos();
		
		void mmenuUpdate();
		void gameUpdate();
		void settingsUpdate();
		
		void mmenuRender();
		void gameRender();
		void settingsRender();
		
		
		
		
		
		/*
			Here comes the game code
		*/
		
		//Tile grid based code
		//Here goes all the map variables, functions, and Game objects
		
		//Variables
		map<int, block> grid;
		int gWidth;
		int gHeight;
		
		//Player variables
		int px;
		int py;
		float sx;
		float sy;
		int gravity;
		int psy;
		
		//Camera variables
		int camX;
		int camY;
		
		//Game objects
		Button* button;
		
		//Functions
		int pos(int x, int y);
		void genLevel();
		void setTex(int num);
		
	public:
		//Accessors
		const bool running();
			
		//Constructors / destructors
		Game(int _fps, int tgx, int tgy);
		virtual ~Game();
		
		//Functions
		void pollEvents();
		void update();
		void render();
		
		istream& unserialize (istream& in);
		ostream& serialize (ostream& out);
		
};

#endif
