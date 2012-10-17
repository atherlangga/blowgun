#ifndef BLOWGUN_PROGRAM_H_
#define BLOWGUN_PROGRAM_H_

#include <GLES2/gl2.h>

#include <vector>
#include <string>

#include "types.h"

namespace blowgun
{

/**
 * Program that lives on the GPU, assembled from shaders.
 * 
 * User of this library should use `ProgramBuilder`
 * to build the instance of this class.
 */
class Program
{
private:
	const u32 program_handle_;
	const std::vector<u32> shader_handles_;

private:
	explicit Program(u32 program_handle, std::vector<u32> shader_handles);

	// Disallow copy-construction and assigning.
	Program(const Program &);// = delete;
	Program & operator=(const Program &);// = delete;

private:
	friend class ProgramBuilder;

public:

	/**
	 * Activate the Program.
	 *
	 * Keep in mind that according to OpenGL ES 2.0 specification,
	 * only one Program can be active at a time.
	 */
	void Use() const;

	/**
	 * Delete the Program.
	 *
	 * Delete this program from the GPU. This method will call
	 * `glDeleteProgram` API internally.
	 */
	void Delete() const;

	/**
	 * Get pairs of attribute name and its type.
	 */
	std::vector< std::pair<std::string, GLenum> > GetActiveAttribs() const;

	/**
	 * Get pairs of uniform name and its type.
	 */
	std::vector< std::pair<std::string, GLenum> > GetActiveUniforms() const;

	/**
	 * Get the symbolic location of an attribute in the Program.
	 */
	i32 GetAttribLocation(const std::string attrib_name) const;
	
	/**
	 * Get the symbolic location of an uniform in the Program.
	 */
	i32 GetUniformLocation(const std::string uniform_name) const;
};

}

#endif
