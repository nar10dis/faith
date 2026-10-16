#include "Renderer.hpp"
#include <vector>

Renderer::Renderer(unsigned int w, unsigned int h) : _window(NULL),
	_fragmentShader(GL_FRAGMENT_SHADER, "renderer/shaders/fragment.glsl"),
	_vertexShader(GL_VERTEX_SHADER, "renderer/shaders/vertex.glsl"),
	_shaderProgram(0), _VAO(0), _VBO(0), _EBO(0), _w(w), _h(h), _gridTexture(0)
{
	return ;
}

Renderer::~Renderer()
{
	glDeleteProgram(this->_fragmentShader.getId());
	glDeleteProgram(this->_vertexShader.getId());
	glDeleteTextures(1, &this->_gridTexture);
	glfwTerminate();
}

bool 	Renderer::init()
{
	if (this->_initWindow() == false)
		return false;
	if (this->_initGlad() == false)
		return false;
	this->_initShader();
	if (this->_shaderProgramInit() == false)
		return false;
	this->_initQuad();
	return true;
}

void 	Renderer::update(const Cell* const* grid)
{
	static std::vector<uint8_t> pixels;
	this->_gridToPixels(grid, pixels);
}

bool 	Renderer::shouldClose()
{
	if (glfwGetKey(this->_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->_window, true);
	return glfwWindowShouldClose(this->_window);
}

void 	Renderer::pollEvents()
{
	glfwPollEvents();
}

bool 	Renderer::_initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->_window = glfwCreateWindow(800, 600, "first", NULL, NULL);

	if (!this->_window)
	{
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(this->_window);
	return true ;
}

void	framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
	return ;
}

bool 	Renderer::_initGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return false;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(this->_window, framebuffer_size_callback);

	return true;
}

void 	Renderer::_initShader()
{
	this->_fragmentShader.readFile();
	this->_fragmentShader.init();
	this->_vertexShader.readFile();
	this->_vertexShader.init();
}

void 	Renderer::_initQuad()
{
	//Point in -1 1 3d axe
	float vertices[] = {
		1.0f, 1.0f, 0.0f,//top right
		1.0f, -1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f, //bottom left
		-1.0f, 1.0f, 0.0f //top left
	};

	unsigned int indexes[] = {
		0, 1, 2, // first triangle
		0, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &this->_VAO);
	glBindVertexArray(this->_VAO);

	glGenBuffers(1, &this->_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &this->_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);  

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Initialize texture
	glGenTextures(1, &this->_gridTexture);
	glBindTexture(GL_TEXTURE_2D, this->_gridTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _w, _h, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindVertexArray(0);
}

bool 	Renderer::_shaderProgramInit()
{

	GLint success;
	GLchar infoLog[512];

	//on recup l'ID de notre Shader Program
	this->_shaderProgram = glCreateProgram();
	//on link les shaders entre eux
	glAttachShader(this->_shaderProgram, this->_vertexShader.getId());
	glAttachShader(this->_shaderProgram, this->_fragmentShader.getId());
	glLinkProgram(this->_shaderProgram);
	//error handling
	glGetProgramiv(this->_shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::SHADER::LINK_FAILED\n" <<
			infoLog << std::endl;
		return (false);
	}
	glUseProgram(this->_shaderProgram);

	// glDeleteShader(shader->vertexShader);
	// glDeleteShader(shader->fragmentShader);

	//how opengl should intepret the data
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//
	// glEnableVertexAttribArray(0);

	return (true);
}

void 	Renderer::_gridToPixels(const Cell* const *grid, std::vector<uint8_t>& pixels)
{
	pixels.clear();
	pixels.resize(_w * _h);  // GL_RED format (1 byte per pixel)

	for (unsigned int y = 0; y < _h; y++)
	{
		for (unsigned int x = 0; x < _w; x++)
		{
			unsigned int idx = y * _w + x;
			pixels[idx] = grid[y][x].isActive() ? 255 : 0;
		}
	}

	// Update texture with pixel data
	glBindTexture(GL_TEXTURE_2D, this->_gridTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _w, _h, GL_RED, GL_UNSIGNED_BYTE, pixels.data());
}

void 	Renderer::_setUniform()
{
	float timeValue = glfwGetTime();
	int loc = glGetUniformLocation(this->_shaderProgram, "u_time");
	glUniform1f(loc, timeValue);

	int width;
	int height;
	glfwGetFramebufferSize(this->_window, &width, &height);
	loc = glGetUniformLocation(this->_shaderProgram, "u_resolution");
	glUniform2f(loc, (float)width, (float)height);

	// Set texture uniform
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->_gridTexture);
	loc = glGetUniformLocation(this->_shaderProgram, "u_grid");
	glUniform1i(loc, 0);
}

void 	Renderer::draw()
{
	// processInput(this->_window);

	// glClearColor(0.2f, 0.3f, abs(sin(glfwGetTime() * 2)), 1.0f);
	glClearColor(0.8f, 0.7f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(this->_shaderProgram);

	this->_setUniform();

	glBindVertexArray(this->_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	this->pollEvents();
	glfwSwapBuffers(this->_window);
}