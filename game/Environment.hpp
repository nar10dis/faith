#pragma once

#include "Game.hpp"
using namespace std;


class Environment {
	private:
		typedef Cell *CellLine;

		CellLine *_CellGrid;
		unsigned int _h;
		unsigned int _l;

	public:

		Environment(unsigned int h, unsigned int l);
		Environment(const Environment& env);
		Environment& operator=(const Environment &env);
		~Environment();

		const Cell* const* getGrid() const { return _CellGrid; };
		unsigned int	getH() const { return _h; };
		unsigned int	getL() const { return _l; };
		void 			setH(unsigned int h) { _h = h; };
		void 			setL(unsigned int l) { _l = l; };

		Cell 	getCellatPosition(unsigned int x, unsigned int y) const { return _CellGrid[y][x]; };
		void 	setActiveAtPosition(unsigned int x, unsigned int y) { _CellGrid[y][x].setActive(); };
		void 	setInactiveAtPosition(unsigned int x, unsigned int y) { _CellGrid[y][x].setInactive(); };
		bool 	isActiveAtPosition(unsigned int x, unsigned int y);
		short 	numberOfActiveNeighbours(unsigned int x, unsigned int y);
		void 	print();
};