#ifndef CELL_HPP
#define CELL_HPP

#include <iostream>
using namespace std;

typedef enum CELL_STATE {ACTIVE, INACTIVE} CELL_STATE;

# define BLACK	"\x1b[30m"
# define RED		"\x1b[31m"
# define GREEN	"\x1b[32m"
# define YELLOW	"\x1b[33m"
# define BLUE	"\x1b[34m"
# define MAGENTA	"\x1b[35m"
# define RESET	"\x1b[37m"

const char INACTIVE_CHAR = ' ';
const char ACTIVE_CHAR = '*';

class Cell {
	public:
		Cell() : _state(INACTIVE){ };
		Cell(const Cell& obj) : _state(obj.isActive() ? ACTIVE : INACTIVE){};
		Cell& operator=(const Cell& obj) { _state = obj._state; return *this; };
		~Cell() {};
	
		bool isActive() const { return (_state == ACTIVE ? true : false); };
		void setActive() { _state = ACTIVE; };
		void setInactive() { _state = INACTIVE; };
		void print() { cout << RED << (_state == ACTIVE ? ACTIVE_CHAR : INACTIVE_CHAR) << RESET; }
	private:
		CELL_STATE _state;
};

#endif