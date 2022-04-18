#include "Game.h"

int main() {
	
	//init game engine
	Game game(60, 15, 30);
	
	while(game.running()) {
		
		//Update
		game.update();
		
		//Render
		game.render();
		
	}
}
