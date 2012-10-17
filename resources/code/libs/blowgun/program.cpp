#include "program.h"

#include <iostream>
#include <stdexcept>

using namespace blowgun;

typedef std::pair<std::string, GLenum> AttribPair;
typedef std::pair<std::string, GLenum> UniformPair;

Program::Program(u32 program_handle, std::vector<u32> shader_handles) :
	program_handle_(program_handle), shader_handles_(shader_handles)
{
}

void
Program::Use() const
{
	glUseProgram(program_handle_);
}

void
Program::Delete() const
{
	glDeleteProgram(program_handle_);

	for (
		auto shader_handle_itr = shader_handles_.begin(); 
		shader_handle_itr != shader_handles_.end(); 
		++shader_handle_itr)
	{
		glDeleteShader(*shader_handle_itr);
	}
}

std::vector< std::pair<std::string, GLenum> >
Program::GetActiveAttribs() const
{
	// Get total attribs of this Program, this will be used as loop condition
	i32 total_attribs;
	glGetProgramiv(program_handle_, GL_ACTIVE_ATTRIBUTES, &total_attribs);

	// Get the the active attribute's strings maximum length, this will be used
	// as argument for glGetActiveAttrib
	i32 max_length;
	glGetProgramiv(program_handle_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);

	std::vector<AttribPair> result;
	for (i32 i = 0; i < total_attribs; ++i)
	{
		char * name = new char[max_length];
		GLenum type;

		glGetActiveAttrib(
			program_handle_,
			i,
			max_length,
			NULL,				// Don't care about the name length
			NULL,				// Don't care about the attribute size
			&type,
			name
		);

		result.push_back(AttribPair(std::string(name), type));
		delete name;
	}

	return result;
}

i32
Program::GetUniformLocation(const std::string uniform_name) const
{
	return glGetUniformLocation(program_handle_, uniform_name.c_str());
}

i32
Program::GetAttribLocation(const std::string attrib_name) const
{
	return glGetAttribLocation(program_handle_, attrib_name.c_str());
}
