#include <SDL2/SDL.h>

#include "game.hpp"

int main() {
	Game game{};

	game.loop();

	return EXIT_SUCCESS;
}
