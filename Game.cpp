#include "Game.h"

//Constructors / Destructors

Game::Game(int _fps, int tgx, int tgy)
{
	this->font.loadFromFile("Assets/Textures/Arial.ttf");
	this->button = new Button(this->font, Color(200, 200, 200), Color(150, 150, 150), Color(100, 100, 100), Color::Black, Vector2f(1920.f, 1080.f));
	
	this->gravity = 10;
	this->fps = _fps;
	this->gWidth = 30;
	this->gHeight = 15;
	this->initVariables();
	this->initWindow();
	this->initGrid();
	
	this->loadCode();
}


Game::~Game()
{
	delete this->window;
	delete this->button;
}

//Accessors

const bool Game::running() {
	return this->window->isOpen();
} 


//Functions

void Game::pollEvents() {
	
	//If close button pressed or escape key pressed close the window and finish the program
	while(this->window->pollEvent(this->ev)) {
		if (this->ev.type == Event::Closed)
            this->window->close();
        if(Keyboard::isKeyPressed(Keyboard::Escape)) {
			
			if(this->EscPressed == false && scene == GAME) {
				this->PauseMenu();
			}
			
			this->EscPressed = true;
		} else {
			this->EscPressed = false;
		}
	}
}

void Game::PauseMenu() {
	this->paused = !this->paused;
}

void Game::update() {
	this->pollEvents();
	
	switch(this->scene) {
		case 0:
			this->mmenuUpdate();
			break;
		case 1:
			this->settingsUpdate();
			break;
		case 2:
			this->gameUpdate();
			break;
	}
}

void Game::mmenuUpdate() {
	
}

void Game::settingsUpdate() {
	
}

void Game::gameUpdate() {
	//Code goes here
	//Player controller and collisions scrpits
	
	/*
		Acceleration: x = 0, y = -1
		Speed: x = sx, y = psy
		Position: x = px, y = py
	*/
	
	//Add Gravity(1 pixel per frame)
	if(!this->paused && this->playMode) {
		this->psy++;
		
		//Move player on the x axis
		if(Keyboard::isKeyPressed(Keyboard::D)) {
			this->sx++;
		}
		
		if(Keyboard::isKeyPressed(Keyboard::A)) {
			this->sx--;
		}
		
		//Smooth the movement a bit
		this->sx *= 0.85f;
		this->px += round(sx) * 2;
			
		//Collisions on the x axis
		if(this->touching()) {
			if(round(sx * 100) / 100 < 0) {
				while(this->touching()) {
					sx = 0;
					this->px ++;
				}	
			} else if(round(sx * 100) / 100 > 0) {
				while(this->touching()) {
					sx = 0;
					this->px --;
				}
				
			}
		}
		
		//Add velocity
		this->py += this->psy;
		
		//Collisions in the y axis
		if(abs(this->psy) == this->psy) {
			while(this->touching()) {
				this->py--;
				this->psy = 0;
			}
		} else {
			while(this->touching()) {
				this->py++;
				this->psy = 0;
			}
		}
		
		//Touching ground condition
		this->py++;
		
		//Jump!
		if(Keyboard::isKeyPressed(Keyboard::W) && this->touching()) {
			this->psy = -35;
		}
		
		this->py--;
		
		this->setCam();
		
		
	} else if(!this->playMode && !this->paused) {
		
		this->setPlaceTile();
		
		//Move player on the x axis
		if(Keyboard::isKeyPressed(Keyboard::D)) {
			this->sx++;
		}
		
		if(Keyboard::isKeyPressed(Keyboard::A)) {
			this->sx--;
		}
		
		//Move player on the y axis
		if(Keyboard::isKeyPressed(Keyboard::S)) {
			this->sy++;
		}
		
		if(Keyboard::isKeyPressed(Keyboard::W)) {
			this->sy--;
		}
		
		//Smooth the movement a bit
		this->sx *= 0.85f;
		this->px += round(sx) * 2;
		
		this->sy *= 0.85f;
		this->py += round(sy) * 2;
		
		//Collisions for getting out of the map
		if(this->px < 100) {
			this->px = 100;
		}
		if(this->px > this->gWidth * 100 - 200) {
			this->px = this->gWidth * 100 - 200;
		}
		
		if(this->py < 100) {
			this->py = 100;
		}
		if(this->py > this->gHeight * 100 - 100) {
			this->py = this->gHeight * 100 - 100;
		}
		
		//Place the tile when the left click is pressed
		if(Mouse::isButtonPressed(Mouse::Left)) {
			//Do things in multiple frames
			int x = this->mousePos().x + this->camX;
			int y = this->mousePos().y + this->camY;
			
			x = round(x / 100);
			y = round(y / 100);
			
			this->button->addButton(20.f, 960.f, 200.f, 100.f, "");
			if(this->button->state == B_IDLE) {
				this->grid[this->pos(x, y)].t = this->brush.t;
				this->grid[this->pos(x, y)].dir = this->brush.dir;
				this->grid[this->pos(x, y)].tr = this->brush.tr;
				this->refreshTile(this->pos(x, y));
			}
			
			if(!this->MousePressed) {
				//Do things once until mouse released
				
			}
			this->MousePressed = true;
		} else {
			this->MousePressed = false;
		}
		
		//Move the camera
		this->setCam();
	}
}

void Game::setCam() {
	this->camX -= (camX - (px - 860)) / 7;
	
	this->camY -= (camY - (py - 490)) / 5;
	
	if(this->camX < 100) {
		this->camX = 100;
	} else if(this->camX > this->gWidth * 100 - 2020) {
		this->camX = this->gWidth * 100 - 2020;
	}
	
	if(this->camY < 100) {
		this->camY = 100;
	} else if(this->camY > this->gHeight * 100 - 1080) {
		this->camY = this->gHeight * 100 - 1080;
	}
}

bool Game::touching() {
	bool ret = this->grid[this->pos((this->px - 0) / 100, (this->py + 1) / 100)].t != 0 && this->grid[this->pos((this->px - 0) / 100, (this->py + 1) / 100)].tr == false   ||   this->grid[this->pos((this->px - 1) / 100 + 1, (this->py - 0) / 100 + 1)].t != 0 && this->grid[this->pos((this->px - 1) / 100 + 1, (this->py - 0) / 100 + 1)].tr == false   ||   this->grid[this->pos((this->px - 1) / 100 + 1, (this->py + 1) / 100)].t != 0 && this->grid[this->pos((this->px - 1) / 100 + 1, (this->py + 1) / 100)].tr == false   ||   this->grid[this->pos((this->px - 0) / 100, (this->py - 0) / 100 + 1)].t != 0 && this->grid[this->pos((this->px - 0) / 100, (this->py - 0) / 100 + 1)].tr == false;
	return ret;
}

void Game::setPlaceTile() {
	if(Keyboard::isKeyPressed(Keyboard::T)) {
			
		if(this->Tpressed == false) {
			this->trBrush = !this->trBrush;
		}
		
		this->Tpressed = true;
	} else {
		this->Tpressed = false;
	}
	
	this->brush.tr = this->trBrush;
	
	if(this->brush.t == 8) {
		this->brush.tr = true;
	}
	
	if(Keyboard::isKeyPressed(Keyboard::Up)) {
		
		if(this->UpPressed == false) {
			brush.t++;
		}
		
		this->UpPressed = true;
	} else {
		this->UpPressed = false;
	}
	
	if(Keyboard::isKeyPressed(Keyboard::Down)) {
			
		if(this->DownPressed == false) {
			brush.t--;
		}
		
		this->DownPressed = true;
	} else {
		this->DownPressed = false;
	}
	
	if(Keyboard::isKeyPressed(Keyboard::Left)) {
			
		if(this->LeftPressed == false) {
			brush.dir += 270;
		}
		
		this->LeftPressed = true;
	} else {
		this->LeftPressed = false;
	}
	
	if(Keyboard::isKeyPressed(Keyboard::Right)) {
			
		if(this->RightPressed == false) {
			brush.dir += 90;
		}
		
		this->RightPressed = true;
	} else {
		this->RightPressed = false;
	}
	
	if(this->brush.t < 0) {
		brush.t = 0;
	}
}

void Game::render() {
	
	//Clear the window
	this->window->clear(Color(0, 255, 255, 255));
	
	//Draw everything
	switch(this->scene) {
		case 0:
			this->mmenuRender();
			break;
		case 1:
			this->settingsRender();
			break;
		case 2:
			this->gameRender();
			break;
	}
	
	//Show on-screen everything
	this->window->display();
}

void Game::mmenuRender() {
	
	this->button->addButton(0, 0, 200.f, 100.f, "Play Game");
	if(this->button->state == B_PRESSED) {
		this->scene = 2;
	}
	this->button->render(this->window);
	
	this->button->addButton(0, 100.f, 200.f, 100.f, "Settings");
	if(this->button->state == B_PRESSED) {
		this->scene = 1;
	}
	this->button->render(this->window);
	
	this->button->addButton(0, 200.f, 200.f, 100.f, "Exit Game");
	if(this->button->state == B_PRESSED) {
		this->saveCode();
		this->window->close();
	}
	this->button->render(this->window);
}

void Game::settingsRender() {
	this->button->addButton(0, 0, 300.f, 150.f, "Back To Game");
	if(this->button->state == B_PRESSED) {
		this->scene = GAME;
	}
	this->button->render(this->window);
	
	this->button->addButton(0, 150.f, 300.f, 150.f, "Reset World");
	if(this->button->state == B_PRESSED) {
		this->genLevel();
	}
	this->button->render(this->window);
	
	this->button->addButton(0, 300.f, 300.f, 150.f, "Tile Arraging");
	if(this->button->state == B_PRESSED) {
		this->TileArrange = !this->TileArrange;
	}
	this->button->render(this->window);
	
	this->button->addButton(0, 500.f, 300.f, 150.f, "Save Level");
	if(this->button->state == B_PRESSED) {
		this->saveCode();
	}
	this->button->render(this->window);
	
	this->button->addButton(0, 650.f, 300.f, 150.f, "Load Level");
	if(this->button->state == B_PRESSED) {
		this->loadCode();
	}
	this->button->render(this->window);
	
	this->button->addButton(0, 850.f, 300.f, 150.f, "Reset Player Pos");
	if(this->button->state == B_PRESSED) {
		this->kill();
	}
	this->button->render(this->window);
}

void Game::gameRender() {
	//Tile grid render
	
	for(int x = 0; x < this->gWidth; x++) {
		for(int y = 0; y < this->gHeight; y++) {
			if(this->grid[pos(x, y)].t != 0) {
				if(!(x * 100 - this->camX > 1920 || y * 100 - this->camY > 1080 || x * 100 - this->camX < -100 || y * 100 - this->camY < -100)) {
					int xpos = x;
					int ypos = y;
					if(this->currentTex != this->grid[pos(xpos, ypos)].t) {
						this->setTex(this->grid[pos(xpos, ypos)].t);
					}
					this->tile.setRotation(this->grid[pos(xpos, ypos)].dir);
					//Center the rotation
					if(this->grid[pos(x, y)].dir % 360 == 90) {
						xpos++;
					}
					if(this->grid[pos(x, y)].dir % 360 == 180) {
						xpos++;
						ypos++;
					}
					if(this->grid[pos(x, y)].dir % 360 == 270) {
						ypos++;
					}
					
					this->tile.setPosition(xpos * 100 - this->camX, ypos * 100 - this->camY);
					this->window->draw(this->tile);
				}
			}
		}
	}
	
	//Player render
	this->setTex(0);
	this->tile.setRotation(0.f);
	this->tile.setPosition(this->px - this->camX, this->py - this->camY);
	this->window->draw(this->tile);
	
	//Render the tile in the mouse
	int x = this->mousePos().x + this->camX;
	int y = this->mousePos().y + this->camY;
	x = round(x / 100);
	y = round(y / 100);
	
	this->setTex(brush.t);
	
	this->button->addButton(20.f, 960.f, 200.f, 100.f, "Play/Editor");
	if(this->button->state == B_PRESSED) {
		this->sx = 0.f;
		this->psy = 0.f;
		this->sy = 0.f;
		this->playMode = !this->playMode;
	}
	this->button->render(this->window);
	
	//Center the rotation
	this->tile.setRotation(brush.dir);
	if(brush.dir % 360 == 90) {
		x++;
	}
	if(brush.dir % 360 == 180) {
		x++;
		y++;
	}
	if(brush.dir % 360 == 270) {
		y++;
	}
	
	this->tile.setColor(Color(255, 255, 255, 127));
	this->tile.setPosition(x * 100 - this->camX, y * 100 - this->camY);
	
	if(!this->brush.t == 0) {
		if(!this->playMode) {
			this->window->draw(this->tile);
		}
	}
	
	if(this->paused) {
		
		//Pause Menu Background
		this->window->draw(this->pauseBackground);
		
		//Buttons
		string text = "Back To Game";
		this->button->addButton(640.f, 340.f, 640.f, 100.f, text);
		if(button->state == B_PRESSED) {
			this->paused = false;
		}
		this->button->render(this->window);
		
		text = "Settings";
		this->button->addButton(640.f, 490.f, 640.f, 100.f, text);
		if(button->state == B_PRESSED) {
			this->scene = 1;
			//this->paused = false;
		}
		this->button->render(this->window);
		
		text = "Main Menu";
		this->button->addButton(640.f, 640.f, 640.f, 100.f, text);
		if(button->state == B_PRESSED) {
			this->scene = 0;
			//this->window->close();
		}
		this->button->render(this->window);
	}
	
	this->tile.setColor(Color(255, 255, 255, 255));
}

int Game::pos(int x, int y) {
	return x * this->gHeight + y;
}


void Game::kill() {
	this->px = 200;
	this->py = 200;
	this->psy = 0;
	this->sx = 0.f;
}

//Private functions

void Game::initVariables() {
	this->genLevel();
	this->brush.t = 1;
	this->brush.dir = 0;
	this->brush.tr = false;
	this->paused = false;
	
	this->playMode = false;
	
	this->kill();
	
	this->TileArrange = true;
	
	this->trBrush = false;
	
	this->scene = 0;
	
	this->saveDir = "";
	this->saveT = "";
	this->saveTr = "";
	
	this->readFile.open("Assets/INFO.txt", ios::in);
	
	if(this->readFile.fail()) {
		MessageBox(0, "The program couldn't find the INFO.txt file. Be sure that is written well.", "Error!", MB_OK | MB_ICONERROR);
		this->window->close();
	}
	
	while(!readFile.eof()) {
		getline(this->readFile, this->Info);
	}
	this->resolution = this->strToInt(this->Info);
}

void Game::initWindow() {
	//Put the window size
	this->videoMode.getDesktopMode();
	this->videoMode.getDesktopMode().width;
	this->videoMode.getDesktopMode().height;
	
	//Create the window
	this->window = new RenderWindow(this->videoMode, "Game", Style::Fullscreen);
	
	//Set the Framerate Limit
	this->window->setFramerateLimit(fps);
	
	//Set the viewport to be compatible with every resolution (like if you have an 8k monitor you'll see more than a 2k one)
	this->view.reset(FloatRect(0, 0, 1920, 1080));
	this->window->setVerticalSyncEnabled(true);
	this->window->setView(this->view);
}

void Game::initGrid() {
	//Import the texture atlas
	this->tileT.loadFromFile("Assets/Textures/TexturesAtlas.png");
	
	//Set the tile size
	this->tile.setScale(100.f/resolution, 100.f/resolution);
	this->tile.setTexture(this->tileT);
	this->tile.setRotation(0);
	
	//Set the pause menu background texture
	this->pauseBackTex.loadFromFile("Assets/Textures/BackGround.png");
	this->pauseBackTex.setRepeated(true);
	
	//Set the sprite for the pause menu
	this->pauseBackground.setColor(Color(255, 255, 255, 240));
	this->pauseBackground.setScale(100.f/resolution, 100.f/resolution);
	this->pauseBackground.setTextureRect(IntRect(0, 0, 1920, 1080));
	this->pauseBackground.setTexture(this->pauseBackTex);
}

void Game::genLevel() {
	//Set some walls around the level
	for(int i = 0; i < this->gWidth; i++) {
		this->grid[pos(i, 0)].t = 4;
		this->grid[pos(i, this->gHeight - 1)].t = 1;
		
		this->grid[pos(i, 0)].dir = 0;
		this->grid[pos(i, this->gHeight - 1)].dir = 0;
	}
	
	for(int i = 1; i < this->gHeight; i++) {
		this->grid[pos(0, i)].t = 4;
		this->grid[pos(this->gWidth - 1, i)].t = 4;
		
		this->grid[pos(0, i)].dir = 0;
		this->grid[pos(this->gWidth - 1, i)].dir = 0;
	}
	
	//Fill the rest with air
	for(int x = 1; x < this->gWidth - 1; x++) {
		for(int y = 1; y < this->gHeight - 1; y++) {
			this->grid[this->pos(x, y)].t = 0;
			
			this->grid[this->pos(x, y)].dir = 0;
		}
	}

}

void Game::saveCode() {
	ofstream fileOut("Assets/Saves/world1.gmm");
	
	if(fileOut.is_open()) {
		this->serialize(fileOut);
	}
	
	fileOut.close();
}

void Game::loadCode() {
	ifstream fileIn("Assets/Saves/world1.gmm");
	
	if(fileIn.is_open()) {
		this->unserialize(fileIn);
	}

	fileIn.close();
}

void Game::setTex(int num) {
	int res = this->resolution;
	int y = num/res;
	this->tile.setTextureRect(IntRect(num*res % (res*16), y*res, res, res));
	currentTex = num;
}

int Game::strToInt(string s) {
	int l = s.length();
	int num = 0;
	if(s[0] == '1') {
		num+=1;
	}
	if(s[0] == '2') {
		num+=2;
	}
	if(s[0] == '3') {
		num+=3;
	}
	if(s[0] == '4') {
		num+=4;
	}
	if(s[0] == '5') {
		num+=5;
	}
	if(s[0] == '6') {
		num+=6;
	}
	if(s[0] == '7') {
		num+=7;
	}
	if(s[0] == '8') {
		num+=8;
	}
	if(s[0] == '9') {
		num+=9;
	}
	if(s[0] == '0') {
		num+=0;
	}
	
	for(int i = 1; i < l; i++) {
		num = num * 10;
		if(s[i] == '1') {
			num+=1;
		}
		if(s[i] == '2') {
			num+=2;
		}
		if(s[i] == '3') {
			num+=3;
		}
		if(s[i] == '4') {
			num+=4;
		}
		if(s[i] == '5') {
			num+=5;
		}
		if(s[i] == '6') {
			num+=6;
		}
		if(s[i] == '7') {
			num+=7;
		}
		if(s[i] == '8') {
			num+=8;
		}
		if(s[i] == '9') {
			num+=9;
		}
		if(s[i] == '0') {
			num+=0;
		}
	}
	return num;
}

float Game::lerp(float a, float b, float t) {
	return a + t * (b - a);
}

Vector2f Game::mousePos() {
	return Vector2f(Mouse::getPosition().x * (1920.f / this->videoMode.getDesktopMode().width), Mouse::getPosition().y * (1080.f / this->videoMode.getDesktopMode().height));
}

void Game::refreshTile(int tPos, bool o) {
	if(this->TileArrange == true) {
			
		if(!o) {
			this->refreshTile(tPos + 1, true);
			this->refreshTile(tPos - 1, true);
			this->refreshTile(tPos + this->gWidth, true);
			this->refreshTile(tPos - this->gWidth, true);
		}
		
		int tileTex = this->grid[tPos].t;
		
		int bu = this->grid[tPos - 1].t;
		int br = this->grid[tPos + this->gWidth].t;
		int bd = this->grid[tPos + 1].t;
		int bl = this->grid[tPos - this->gWidth].t;
		int bur;
		int brd;
		int bdl;
		int blu;
		
		if(bu != 0 && bu < 4) { // 4 is the max grass tile + 1
			bu = 1;
		}
		if(br != 0 && br < 4) { // 4 is the max grass tile + 1
			br = 1;
		}
		if(bd != 0 && bd < 4) { // 4 is the max grass tile + 1
			bd = 1;
		}
		if(bl != 0 && bl < 4) { // 4 is the max grass tile + 1
			bl = 1;
		}
		
		int code = bu*1000 + br*100 + bd*10 + bl*1;
		
		if(tileTex > 0 && tileTex < 4) {
			
			/*
				code works like this:
					up right down left
					
				example:
					up = 1, the rest = 0
					code would be: 1000
			*/
			
			if(bu == 1) {
				this->grid[tPos].t = 3;
				this->grid[tPos].dir = 0;
			} else {
				this->grid[tPos].t = 1;
				this->grid[tPos].dir = 0;
			}
		}
	}
}

istream& Game::unserialize (istream& in)
{
//	return in;
	//Decode
	in >> this->gWidth;
	in >> this->gHeight;
	
	for(int x = 0; x < this->gWidth; x++) {
		for(int y = 0; y < this->gHeight; y++) {
			in >> this->grid[pos(x, y)].t;
			in >> this->grid[pos(x, y)].tr;
			in >> this->grid[pos(x, y)].dir;
		}
	}
	
	char nothing;
	in >> nothing;
	
    return in;
}

ostream& Game::serialize (ostream& out)
{
//	return out;
	//Encode
	out << this->gWidth << ' ';
	out << this->gHeight << ' ';
	
	for(int x = 0; x < this->gWidth; x++) {
		for(int y = 0; y < this->gHeight; y++) {
			out << this->grid[pos(x, y)].t << ' ';
			out << this->grid[pos(x, y)].tr << ' ';
			out << this->grid[pos(x, y)].dir << ' ';
		}
	}
	
	out << ';' << '\n';
	
    return out;
}
