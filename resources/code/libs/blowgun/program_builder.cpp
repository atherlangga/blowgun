#include "program_builder.h"

#include <GLES2/gl2.h>

#include <iostream>
#include <stdexcept>

#include "program.h"

// Helper method to create and compile shader
static GLuint CreateShader(GLenum type, std::string source);

using namespace blowgun;

typedef std::pair<GLenum, std::string> ShaderPair;
typedef std::pair<GLuint, std::string> AttributePair;

ProgramBuilder::ProgramBuilder() :
	shaders_(), attributes_()
{
}

ProgramBuilder::~ProgramBuilder()
{
}

ProgramBuilder &
ProgramBuilder::AddShader(
	GLenum shader_type, std::string shader_source)
{
	shaders_.push_back(ShaderPair(shader_type, shader_source));
	return * this;
}

ProgramBuilder &
ProgramBuilder::BindAttribute(u32 location, std::string name)
{
	attributes_.push_back(AttributePair(location, name));
	return * this;
}

std::unique_ptr<Program>
ProgramBuilder::Build()
{
	std::vector<u32> shader_handles;

	// Create the program and get its handle 
	GLuint program_handle = glCreateProgram();
	
	// Create and attach shaders 
	for (auto i = shaders_.begin(); i != shaders_.end(); ++i)
	{
		u32 shader_handle = CreateShader((* i).first, (* i).second);
		glAttachShader(program_handle, shader_handle);

		// Keep the shader handle (id) to be used in Program.
		shader_handles.push_back(shader_handle);
	}

	// According to the specification, we have to bind
	// all attributes before we can link the program. Let's do it then.
	for (auto i = attributes_.begin(); i != attributes_.end(); ++i)
	{
		glBindAttribLocation(program_handle, (* i).first, (* i).second.c_str());
	}
	
	// Link the program 
	glLinkProgram(program_handle);

	// Check whether linking succeeded 
	GLint program_is_linked;
	glGetProgramiv(program_handle, GL_LINK_STATUS, &program_is_linked);
	
	if (!program_is_linked)
	{
		GLint info_log_length;
		glGetProgramiv(program_handle, GL_INFO_LOG_LENGTH, &info_log_length);

		char * info_log_ca = new char[info_log_length];
		glGetProgramInfoLog(program_handle, info_log_length, NULL, info_log_ca);

		std::string info_log(info_log_ca);
		std::cout << info_log << std::endl;
		delete[] info_log_ca;

		throw std::runtime_error("Failed linking program.");
	}

	return std::unique_ptr<Program>(new Program(program_handle, shader_handles));
}

static u32
CreateShader(GLenum type, std::string source)
{
	const char * shader_source_ca = source.c_str();

	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, &shader_source_ca, NULL);
	glCompileShader(handle);

	// Check whether the compile succeeded.
	GLint shader_is_compiled;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &shader_is_compiled);

	if (!shader_is_compiled)
	{
		GLint info_log_length;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &info_log_length);

		char * info_log_ca = new char[info_log_length];
		glGetShaderInfoLog(handle, info_log_length, NULL, info_log_ca);

		// TODO: Revisit this!
		std::string info_log(info_log_ca);

		std::cout << info_log << std::endl;
		delete[] info_log_ca;

		throw std::runtime_error("Failed compiling shader.");
	}

	return handle;
}
