#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include "unistd.h"
#include "time.h"
#include "sstream"
#include "fstream"
#include <ios>
#include <math.h>
#include "Shader.hpp"
#include "../game/Cell.hpp"
#include "Shader.hpp"

class Renderer 
{
	private:
		GLFWwindow*	_window;
		Shader 		_fragmentShader;
		Shader 		_vertexShader;

		unsigned int _shaderProgram;

		//enregistre la config
		unsigned int _VAO;
		//enregistre les vertices
		unsigned int _VBO;
		//enregistre les indexes
		unsigned int _EBO;

		unsigned int _w;
		unsigned int _h;
		GLuint		_gridTexture;

		bool 	_initWindow();
		bool 	_initGlad();
		void 	_initShader();
		void 	_initQuad();
		bool 	_shaderProgramInit();
		void 	_gridToPixels(const Cell* const *grid, std::vector<uint8_t>& pixels);
		void 	_setUniform();

	public:
		Renderer(unsigned int w, unsigned int h);
		~Renderer();

		bool 	init();
		void 	update(const Cell* const* grid);

		void 	setUniform();
		void 	draw();

		bool 	shouldClose();
		void 	pollEvents();

		GLFWwindow*	getWindow(){ return this->_window ;};


};


void	framebuffer_size_callback(GLFWwindow* window, int width, int height);
void	processInput(GLFWwindow *window);

#endif