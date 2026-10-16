#include "Game.hpp"
#include "Environment.hpp"

#include <limits>

Environment::Environment(unsigned int h, unsigned int l)
{
	_h = h;
	_l = l;

	this->_CellGrid = new CellLine[h];
	for (unsigned int i = 0; i < h; i++)
		_CellGrid[i] = new Cell[l];

}

Environment::Environment(const Environment& env)
{
	if (this == &env)
		return ;
	
}

Environment& Environment::operator=(const Environment& env)
{
	if (this == &env)
		return *this;

	for (unsigned int i = 0; i < this->_h; i++)
		delete[] this->_CellGrid[i];
	delete[] this->_CellGrid;

	this->_h = env.getH();
	this->_l = env.getL();

	this->_CellGrid = new CellLine[this->_h];
	for (unsigned int i = 0; i < this->_h; i++)
	{
		this->_CellGrid[i] = new Cell[this->_l];
		for (unsigned int j = 0; j < this->_l; j++)
			this->_CellGrid[i][j] = env.getCellatPosition(j, i);
	}

	return *this;
}

Environment::~Environment()
{
	for (unsigned int i = 0; i < _h; i++)
		delete [] _CellGrid[i];
}

bool 	Environment::isActiveAtPosition(unsigned int x, unsigned int y)
{
	if (x > _l - 1|| y > _h - 1 || x  == numeric_limits<unsigned int>::max() || y  == numeric_limits<unsigned int>::max() )
		return (false);
	return _CellGrid[y][x].isActive(); 
}

short Environment::numberOfActiveNeighbours(unsigned int x, unsigned int y)
{
	short 	count = 0;

	if (isActiveAtPosition(x + 1, y))
		count++;
	if  (isActiveAtPosition(x, y + 1))
		count++;
	if (isActiveAtPosition(x - 1, y))
		count++;
	if (isActiveAtPosition(x, y - 1))
		count++;
	if (isActiveAtPosition(x + 1, y + 1))
		count++;
	if  (isActiveAtPosition(x - 1, y + 1))
		count++;
	if (isActiveAtPosition(x - 1, y - 1))
		count++;
	if  (isActiveAtPosition(x + 1, y - 1))
		count++;
	
	// cerr << "count = " << count << "for x = " << x << "and y = " << y << endl;
	return count;
}

void Environment::print()
{
	for (unsigned int i = 0; i < _h; i++)
	{
		cout << BLUE << string(_l * 2, '-');
		cout << endl << '|' << RESET;
		for (unsigned int j = 0; j < _l; j++)
		{
			getCellatPosition(j, i).print();
			cout << BLUE << '|' << RESET;
		}
		cout << endl;
	}
	cout << BLUE << string(_l * 2, '-') << RESET;

}