#include "model_loading_application.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <GLES2/gl2.h>

#include <blowgun/matrix.h>
#include <blowgun/program.h>
#include <blowgun/program_builder.h>
#include <blowgun/vertex_attribute.h>
#include <blowgun/model_loader_obj.h>
#include <blowgun/texture.h>
#include <blowgun/texture_builder.h>
#include <blowgun/image_loader_tga.h>

// Minimalist vertex shader.
// Only takes a PMV matrix and vertex position
// as its input.
static const char * kVertexShaderSource = "\
    attribute mediump vec4  a_vertex_position;\
    attribute mediump vec2  a_vertex_texture;\
    varying mediump vec2    v_vertex_texture;\
    uniform mediump mat4    u_PMV_matrix;\
    \
    void main(void)\
    {\
        v_vertex_texture = a_vertex_texture;\
        gl_Position = u_PMV_matrix * a_vertex_position;\
    }";

// Minimalist fragment shader.
static const char * kFragmentShaderSource = "\
    varying mediump vec2        v_vertex_texture;\
    uniform mediump sampler2D   u_texture;\
    \
    void main (void)\
    {\
        gl_FragColor = texture2D(u_texture, v_vertex_texture);\
    }";

// Projection Model-View matrix
static std::shared_ptr<blowgun::Matrix> pmv_matrix;

// Model to draw.
static std::shared_ptr<blowgun::Model> model;

// Texture for model.
static std::unique_ptr<blowgun::Texture> texture;

// GPU program built from vertex shader and
// fragment shader above.
static std::unique_ptr<blowgun::Program> program;

// Predefined position for "a_vertex_position" in
// vertex shader to bind for.
static const blowgun::i32 kVertexPositionAttrib = 0;

// Predefined position for "a_vertex_texture" in
// vertex shader to bind for.
static const blowgun::i32 kVertexTextureAttrib = 1;

void
ModelLoadingApplication::OnInitialization()
{
    // Create perspective matrix.
    blowgun::Matrix persp = blowgun::Matrix::CreateIdentity().
        Perspective(60, 800.0f/600.0f, 1.0f, 20.0f);

    // Create translation matrix.
    blowgun::Matrix trans = blowgun::Matrix::CreateIdentity().
        Translate(0.0f, 0.0f, -15.0f);

    // In this application, PMV is a result of translation multiplied by
    // perspective.
    pmv_matrix = std::make_shared<blowgun::Matrix>(trans.Multiply(persp));

    // Build the GPU program.
    program = blowgun::ProgramBuilder().
        AddShader(GL_VERTEX_SHADER, kVertexShaderSource).
        AddShader(GL_FRAGMENT_SHADER, kFragmentShaderSource).
        BindAttribute(kVertexPositionAttrib, "a_vertex_position").
        BindAttribute(kVertexTextureAttrib, "a_vertex_texture").
        Build();

    // Activate the program.
    program->Use();

    // Build and load the model.
    blowgun::ModelLoaderOBJ loader;
    std::ifstream cubeObjFile("data/cube.obj", std::ios::in);
    model = loader.Load(cubeObjFile);

    // Open the texture file.
    // Notice that `std::ios::binary` need to be explicitly requested,
    // otherwise it will fail silently on Win32. Trust me, I just spent
    // too much times chased the bug.
    std::ifstream image_file("data/bricks_color_map.tga",
        std::ios::in | std::ios::binary);
    if (!image_file.is_open())
        throw new std::runtime_error("File not opened.");

    // Load the texture.
    blowgun::ImageLoaderTGA imageLoaderTGA;
    std::shared_ptr<blowgun::Image> image = imageLoaderTGA.Load(image_file);

    // Build and setup texture.
    texture = blowgun::TextureBuilder().
        SetTarget(GL_TEXTURE_2D).
        SetLevelOfDetail(0).
        SetFormat(GL_RGB).
        SetWidth(image->width).
        SetHeight(image->height).
        SetType(GL_UNSIGNED_BYTE).
        SetData(image->data).
        Build();

    // Specify "zooming" filter.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void
ModelLoadingApplication::OnDraw() const
{
    // Clear out the color and buffer.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Set up PMV matrix.
    int pmv_matrix_location = program->GetUniformLocation("u_PMV_matrix");
    glUniformMatrix4fv(pmv_matrix_location, 1, GL_FALSE, pmv_matrix->values());

    // Set up the texture.
    glActiveTexture(GL_TEXTURE0);
    texture->Bind();
    int texture_location = program->GetUniformLocation("u_texture");
    glUniform1i(texture_location, 0);

    const std::vector<blowgun::VertexAttribute> vertex_attributes =
        model->vertex_attributes();

    // Send vertex position information to GPU program.
    glEnableVertexAttribArray(kVertexPositionAttrib);
    glVertexAttribPointer(
        kVertexPositionAttrib,
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(blowgun::VertexAttribute),
        &vertex_attributes.at(0).data.float_3[0]);

    // Send vertex texture coordinate information to GPU program.
    glEnableVertexAttribArray(kVertexTextureAttrib);
    glVertexAttribPointer(
        kVertexTextureAttrib,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(blowgun::VertexAttribute),
        &vertex_attributes.at(1).data.float_2[0]);

    glDrawArrays(GL_TRIANGLES, 0, vertex_attributes.size() / 2);
}

void
ModelLoadingApplication::OnDestroy()
{
    program->Delete();
    texture->Delete();
}
