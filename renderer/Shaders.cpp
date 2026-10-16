#include "Renderer.hpp"
#include "Shader.hpp"

// 1 = neon, 2 = organique, 3 = crt (make STYLE=N)
#ifndef CELL_STYLE
# define CELL_STYLE 1
#endif

Shader::Shader(int type, std::string path) : _type(type), _path(path)
{
	return ;
}

void 	Shader::readFile()
{
	std::ostringstream	str;
	std::ifstream 		file(this->_path, std::ios::in);
	std::string 		line;

	if (!file)
	{
		std::cerr << "Error loading file -> " << this->_path << std::endl;
		exit(1);
	}
	while (getline(file, line))
		str << line << std::endl;
	this->_code = str.str();

	// injecte le style choisi a la compilation juste apres la ligne #version
	size_t eol = this->_code.find('\n');
	if (this->_code.compare(0, 8, "#version") == 0 && eol != std::string::npos)
		this->_code.insert(eol + 1, "#define CELL_STYLE " + std::to_string(CELL_STYLE) + "\n");
	std::cout << this->_code << std::endl;
}

bool	Shader::init()
{
	int 	success;
	char	infoLog[512];


	//creating the shader
	this->_id = glCreateShader(this->_type);
	//attach the shader source code
	const char *src = this->_code.c_str();
	glShaderSource(this->_id, 1, &src, NULL);
	//compiling
	glCompileShader(this->_id);
	//error handling
	glGetShaderiv(this->_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(this->_id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << this->_path << "::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
			return false;
	}
	return true;
}