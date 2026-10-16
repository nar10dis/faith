#pragma once

#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

using namespace std;

# define BLACK	"\x1b[30m"
# define RED		"\x1b[31m"
# define GREEN	"\x1b[32m"
# define YELLOW	"\x1b[33m"
# define BLUE	"\x1b[34m"
# define MAGENTA	"\x1b[35m"
# define RESET	"\x1b[37m"

#include "Cell.hpp"
#include "Environment.hpp"
#include "GameOfLife.hpp"
