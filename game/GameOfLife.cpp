#include "Game.hpp"

#include <chrono>
#include <thread>


GameOfLife::GameOfLife(unsigned int h, unsigned int l) : _env(h, l)
{
	return ;
}

CELL_STATE 	getRandState()
{
	return ((double)rand()/RAND_MAX < 0.5) ? INACTIVE : ACTIVE;
}

void 	GameOfLife::randomInit()
{
	srand(time(NULL));

	for (unsigned int i = 0; i < _env.getH(); i++)
	{
		for (unsigned int j = 0; j < _env.getL(); j++)
		{
			if (getRandState() == ACTIVE)
			{
				cerr << "x = " << j <<" y = " << i << endl;
				_env.setActiveAtPosition(j, i);
			}
		}
	}
}
void 		GameOfLife::_executeOneCyle()
{
	Cell cell;
	Environment new_env(this->_env.getH(), this->_env.getL()); //remplacer par copy ctor
	unsigned int count = 0;

	for (unsigned int i = 0; i < _env.getH(); i++)
	{
		for (unsigned int j = 0; j < _env.getL(); j++)
		{
			cell = _env.getCellatPosition(j, i);
			if (cell.isActive())
			{
				new_env.setActiveAtPosition(j, i);

				if (_env.numberOfActiveNeighbours(j, i) < 2 || _env.numberOfActiveNeighbours(j, i) > 3)
					new_env.setInactiveAtPosition(j, i);
				count++;

			}
			else
			{
				if (_env.numberOfActiveNeighbours(j, i) == 3)
				{
					new_env.setInactiveAtPosition(j, i);

					new_env.setActiveAtPosition(j, i);
					count++;
				}
			}
		}
	}
	this->_env = new_env;
	this_thread::sleep_for(chrono::milliseconds(1000));
	if (count == 0)
	{
		cout << "END GAME" << endl;
		exit(0);
	}
	_env.print();
	cout << endl << endl;
}


void 	GameOfLife::step()
{
	Cell cell;
	Environment new_env(this->_env.getH(), this->_env.getL()); //remplacer par copy ctor
	unsigned int count = 0;

	for (unsigned int i = 0; i < _env.getH(); i++)
	{
		for (unsigned int j = 0; j < _env.getL(); j++)
		{
			cell = _env.getCellatPosition(j, i);
			if (cell.isActive())
			{
				new_env.setActiveAtPosition(j, i);

				if (_env.numberOfActiveNeighbours(j, i) < 2 || _env.numberOfActiveNeighbours(j, i) > 3)
					new_env.setInactiveAtPosition(j, i);
				count++;

			}
			else
			{
				if (_env.numberOfActiveNeighbours(j, i) == 3)
				{
					new_env.setInactiveAtPosition(j, i);

					new_env.setActiveAtPosition(j, i);
					count++;
				}
			}
		}
	}
	this->_env = new_env;
	this_thread::sleep_for(chrono::milliseconds(300));
	if (count == 0)
	{
		cout << "END GAME" << endl;
		exit(0);
	}
	_env.print();
	cout << endl << endl;
}

void 	GameOfLife::execute(unsigned int nbSteps)
{
	cout << "start : " << endl;
	_env.print();
	cout << endl;
	for (unsigned int i = 0; i < nbSteps; i++)
		_executeOneCyle();
}