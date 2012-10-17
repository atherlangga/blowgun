#include <iostream>

#include <GLES2/gl2.h>

#include <blowgun/types.h>
#include <blowgun/program.h>
#include <blowgun/program_builder.h>

#include "rainbow_triangle_application.h"

static const char * kVertexShaderSource = "\
	attribute mediump vec4	a_vertex_position;\
	attribute mediump vec3 	a_vertex_color;\
	uniform mediump mat4	u_PMV_matrix;\
	varying mediump vec3	v_vertex_color;\
	void main(void)\
	{\
		gl_Position = u_PMV_matrix * a_vertex_position;\
		v_vertex_color = a_vertex_color;\
	}";

static const char * kFragmentShaderSource = "\
	varying mediump vec3 v_vertex_color;\
	void main (void)\
	{\
		gl_FragColor = vec4(v_vertex_color, 1.0);\
	}";

static const float kIdentityMatrix[] =
{
	1.0f,  0.0f,  0.0f,  0.0f,
	0.0f,  1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
};

static const GLfloat kVerticesPosition[] =
{
	-0.4f, -0.4f,  0.0f,
	 0.4f, -0.4f,  0.0f,
	 0.0f,  0.4f,  0.0f,
};

static const GLfloat kVerticesColor[] = {
	 1.0f,  0.0f,  0.0f,
	 0.0f,  1.0f,  0.0f,
	 0.0f,  0.0f,  1.0f,
};

static const blowgun::u32 kVertexPositionAttrib = 0;
static const blowgun::u32 kVertexColorAttrib    = 1;

static std::unique_ptr<blowgun::Program> program;

void
RainbowTriangleApplication::OnInitialization()
{
	program = blowgun::ProgramBuilder()
		.AddShader(GL_VERTEX_SHADER, kVertexShaderSource)
		.AddShader(GL_FRAGMENT_SHADER, kFragmentShaderSource)
		.BindAttribute(kVertexPositionAttrib, "a_vertex_position")
		.BindAttribute(kVertexColorAttrib, "a_vertex_color")
		.Build();
	
	program->Use();
}

void
RainbowTriangleApplication::OnDraw() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	int pmv_matrix_location = program->GetUniformLocation("u_PMV_matrix");
	glUniformMatrix4fv(pmv_matrix_location, 1, GL_FALSE, kIdentityMatrix);

	glEnableVertexAttribArray(kVertexPositionAttrib);
	glVertexAttribPointer(kVertexPositionAttrib, 3, GL_FLOAT, GL_FALSE, 0, kVerticesPosition);

	glEnableVertexAttribArray(kVertexColorAttrib);
	glVertexAttribPointer(kVertexColorAttrib, 3, GL_FLOAT, GL_FALSE, 0, kVerticesColor);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void
RainbowTriangleApplication::OnDestroy()
{
	program->Delete();
}
