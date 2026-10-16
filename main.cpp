#include "game/Game.hpp"
#include "renderer/Renderer.hpp"

int main()
{
	GameOfLife	game(50, 50);
	Renderer	renderer(50, 50);

	game.randomInit();
	if (!renderer.init())
		return -1;

	while (!renderer.shouldClose())
	{
		game.step();
		renderer.update(game.getGrid());
		renderer.draw();
	}
	return 0;
}