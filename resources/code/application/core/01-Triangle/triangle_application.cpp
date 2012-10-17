#include <memory>

#include <GLES2/gl2.h>

#include <blowgun/types.h>
#include <blowgun/program.h>
#include <blowgun/program_builder.h>

#include "triangle_application.h"

// Minimalist vertex shader source.
// What this shader do is just multiply the PMV
// matrix with each vertex position.
static const char * kVertexShaderSource = "\
    attribute highp vec4    a_vertex_position;\
    uniform mediump mat4    u_PMV_matrix;\
    void main(void)\
    {\
        gl_Position = u_PMV_matrix * a_vertex_position;\
    }";

// Minimalist fragment shader source.
// What this shader do is just producing white color
// for each vertex.
static const char * kFragmentShaderSource = "\
    void main (void)\
    {\
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\
    }";

// Identity matrix.
// Should be self-explanatory :).
static const float kIdentityMatrix[] =
{
    1.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  1.0f
};

// The position of each vertex that will
// produce a triangle.
static const GLfloat kVerticesPosition[] =
{
    -0.4f, -0.4f,  0.0f,
     0.4f, -0.4f,  0.0f,
     0.0f,  0.4f,  0.0f,
};

// The attribute of vertex position in the GPU Program.
static const blowgun::i32 kVertexPosition = 0;

// The GPU Program.
// This will be built using `blowgun::ProgramBuilder`.
static std::unique_ptr<blowgun::Program> program;

void
TriangleApplication::OnInitialization()
{
    // Build the GPU program.
    program = blowgun::ProgramBuilder()
        // Attach the vertex shader.
        .AddShader(GL_VERTEX_SHADER, kVertexShaderSource)
        // Attach the fragment shader.
        .AddShader(GL_FRAGMENT_SHADER, kFragmentShaderSource)
        // Bind the `a_vertex_position` to chosen constant value.
        .BindAttribute(kVertexPosition, "a_vertex_position")
        // Build the program based on the supplied parameters
        // and then get the result to `program`.
        .Build();

    // Now that `program` has been built, use it.
    program->Use();
}

void
TriangleApplication::OnDraw() const
{
    // Clear the color buffer with black.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Send the PMV matrix - in this case, an identity matrix -
    // to the Program.
    glUniformMatrix4fv(
        // Get the "slot" number in the Program
        // where `u_PMV_matrix` should be located.
        program->GetUniformLocation("u_PMV_matrix"),
        // We will only send 1 matrix data
        1,
        // The matrix data not need to be transposed.
        // Furthermore, The OpenGL ES 2 spec also says
        // this must be `GL_FALSE`. Weird, huh?
        GL_FALSE,
        // The matrix data itself.
        kIdentityMatrix);

    // Activate the vertex position attribute.
    glEnableVertexAttribArray(kVertexPosition);

    // Send the vertex position data to Program.
    glVertexAttribPointer(
        // The index of attrib.
        kVertexPosition,
        // The component size. In this case is 3
        // (x, y, and z) for position attrib.
        3,
        // The type of the position attrib data.
        // In this case, we use float for position
        // data.
        GL_FLOAT,
        // This data needn't to be normalized.
        GL_FALSE,
        // The stride of the data. In this case,
        // the data is continuous, so no stride.
        // No stride equals 0.
        0,
        // The data itself.
        kVerticesPosition);

    // Now that:
    // 1. The minimalist GPU program have been built and activated
    //    in the GPU, and
    // 2. The position for 3 vertices that will formed a triangle
    //    has been uploaded to the GPU,
    // we can draw the triangle.
    glDrawArrays(
        // Tell OpenGL that we're going to draw triangle primitive.
        // Other option for primitive includes points (`GL_POINTS`) and
        // lines (`GL_LINES`).
        GL_TRIANGLES,
        // Start the drawing from 0th element of the enabled vertex
        // attribute array(s). In this case, the only enabled vertex
        // attribute array is the vertex position.
        0,
        // Tell OpenGL to draw 3 vertices. Triangle needs 3 vertices,
        // remember?
        3);
}

void
TriangleApplication::OnDestroy()
{
    // The GPU Program has been finished working,
    // it's time to delete it. Otherwise, it may
    // cause memory leak.
    program->Delete();
}
