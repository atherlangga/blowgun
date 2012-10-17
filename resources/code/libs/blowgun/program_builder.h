#ifndef BLOWGUN_PROGRAM_BUILDER_H_
#define BLOWGUN_PROGRAM_BUILDER_H_

#include <GLES2/gl2.h>

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "types.h"

namespace blowgun
{

class Program;

class ProgramBuilder
{
private:
	/**
	 * Shader type and source for a `Program` to use.
	 */
	std::vector< std::pair<GLenum, std::string> > shaders_;

	/**
	 * Attribute location and its name to be bound.
	 */
	std::vector< std::pair<GLuint, std::string> > attributes_;

public:
	explicit ProgramBuilder();
	virtual ~ProgramBuilder();

	/**
	 * Attach shader to the program.
	 * 
	 * Please note that this just keep the shaders information. The call
	 * to `glAttachShader` will be performed in the `Build` method.
	 */
	ProgramBuilder & AddShader(GLenum shader_type, std::string shader_source);

	/**
	 * Bind attribute to the specified handle (in this case, an u32).
	 *
	 * Please note that the real `glBindAttribute` will be performed
	 * in the `Build` method.
	 */
	ProgramBuilder & BindAttribute(u32 location, std::string name);

	/**
	 * Build the `Program`.
	 */
	std::unique_ptr<Program> Build();
};

}

#endif // BLOWGUN_PROGRAM_BUILDER_H_
