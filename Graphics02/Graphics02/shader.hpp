#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL\glew.h>

namespace shader {

	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

	GLuint LoadShaderFromCode(const char * vertex_shader_code, const char * fragment_shader_code);
}
#endif
