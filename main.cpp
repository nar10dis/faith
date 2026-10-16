#include "game/Game.hpp"
#include "renderer/Renderer.hpp"

int main()
{
	GameOfLife	game(10, 10);
	Renderer	renderer(10, 10);

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