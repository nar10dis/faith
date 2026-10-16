#pragma once
#include "Game.hpp"

class GameOfLife
{
	private:
		Environment _env;

		void 		_executeOneCyle();
	public:
		GameOfLife(unsigned int h, unsigned int l);
		
		void 	randomInit(); //init env;
		void 	execute(unsigned int nbSteps);
		void 	step();
		const Cell* const* getGrid() const { return _env.getGrid(); }

};