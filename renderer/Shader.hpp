#pragma once

#include "Renderer.hpp"

class Shader {
	private :
		unsigned int _id;

		std::string 	_code;

		unsigned int _program;

		//#define for glsl
		int 		_type;
		std::string 	_path;

	public:
		Shader(int type, std::string path);

		unsigned int 	getId() { return this->_id; };
		void 	readFile();
		bool 	init();
};
