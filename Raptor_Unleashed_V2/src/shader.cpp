#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "App.h"

Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RenderID(0) {
	ShaderProgramSource source = ParseShader(filepath);
	m_RenderID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
	GLCALL(glDeleteProgram(m_RenderID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	return {
		ss[0].str(), ss[1].str()
	};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	GLCALL(unsigned int program = glCreateProgram());
	std::cout << program << std::endl;
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCALL(glAttachShader(program, vs));
	GLCALL(glAttachShader(program, fs));
	GLCALL(glLinkProgram(program));
	GLCALL(glValidateProgram(program));
	 
	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));

	return program;
}

void Shader::Bind() const {
	GLCALL(glUseProgram(m_RenderID));
}

void Shader::UnBind() const {
	GLCALL(glUseProgram(0));
}

int Shader::GetUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}
	GLCALL(int location = glGetUniformLocation(m_RenderID, name.c_str()));
	if (location == -1) {
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}
	m_UniformLocationCache[name] = location;

	return location;
}


void Shader::SetUniform1i(const std::string& name, int v) {
	GLCALL(glUniform1i(GetUniformLocation(name), v));
}

void Shader::SetUniform1f(const std::string& name, float v) {
	GLCALL(glUniform1f(GetUniformLocation(name), v));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
	GLCALL(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	GLCALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformf(const std::string& name, std::vector<float> values) {
	int location = GetUniformLocation(name);
	int nValues = values.size();

	switch (nValues) {
	case 0:
		return;
		break;
	case 1:
		GLCALL(glUniform1f(location, values[0]));
		break;
	case 2:
		GLCALL(glUniform2f(location, values[0], values[1]));
		break;
	case 3:
		GLCALL(glUniform3f(location, values[0], values[1], values[2]));
		break;
	case 4:
		GLCALL(glUniform4f(location, values[0], values[1], values[2], values[3]));
		break;
	}
}