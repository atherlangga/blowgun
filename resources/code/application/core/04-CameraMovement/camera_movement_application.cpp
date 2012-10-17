#include "camera_movement_application.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

#include <GLES2/gl2.h>

#include <blowgun/matrix.h>
#include <blowgun/program.h>
#include <blowgun/program_builder.h>
#include <blowgun/model.h>
#include <blowgun/model_loader_obj.h>
#include <blowgun/texture.h>
#include <blowgun/texture_builder.h>
#include <blowgun/image_loader_tga.h>

namespace
{
    static std::shared_ptr<blowgun::Matrix> pmv_matrix;

    static std::shared_ptr<blowgun::Matrix>
    CreatePMVMatrix()
    {
        blowgun::Matrix persp = blowgun::Matrix::CreateIdentity()
            .Perspective(60, 800.0f/600.0f, 1.0f, 20.0f);

        blowgun::Matrix trans = blowgun::Matrix::CreateIdentity()
            .Translate(0.0f, 0.0f, -15.0f);

        return std::make_shared<blowgun::Matrix>(trans.Multiply(persp));
    }

    static std::unique_ptr<blowgun::Program> program;
    static const blowgun::i32 kVertexPositionAttrib = 0;
    static const blowgun::i32 kVertexTextureAttrib  = 1;

    static std::unique_ptr<blowgun::Program>
    CreateProgram()
    {
        std::ifstream vertex_shader_file("data/shader01.vs");
        if (!vertex_shader_file.is_open())
            throw new std::runtime_error("Can't open vertex shader file!");

        std::ifstream fragment_shader_file("data/shader01.fs");
        if (!fragment_shader_file.is_open())
            throw new std::runtime_error("Can't open fragment shader file!");

        std::string vertex_shader(
            (std::istreambuf_iterator<char>(vertex_shader_file)),
            std::istreambuf_iterator<char>());

        std::string fragment_shader(
            (std::istreambuf_iterator<char>(fragment_shader_file)),
            std::istreambuf_iterator<char>());

        return blowgun::ProgramBuilder()
            .AddShader(GL_VERTEX_SHADER, vertex_shader)
            .AddShader(GL_FRAGMENT_SHADER, fragment_shader)
            .BindAttribute(kVertexPositionAttrib, "a_vertex_position")
            .BindAttribute(kVertexTextureAttrib, "a_vertex_texture")
            .Build();
    }

    static std::shared_ptr<blowgun::Model> model;

    static std::shared_ptr<blowgun::Model>
    CreateModel()
    {
        // Build and load the model
        blowgun::ModelLoaderOBJ loader;
        std::ifstream cubeObjFile("data/cube.obj", std::ios::in);
        return loader.Load(cubeObjFile);
    }

    static std::unique_ptr<blowgun::Texture> texture;

    static std::unique_ptr<blowgun::Texture>
    CreateTexture()
    {
        std::ifstream image_file("data/bricks_color_map.tga",
            std::ios::binary | std::ios::in);
        if (!image_file.is_open())
            throw new std::runtime_error("Can't open texture file!");

        blowgun::ImageLoaderTGA imageLoaderTGA;
        std::shared_ptr<blowgun::Image> image = imageLoaderTGA.Load(image_file);

        return blowgun::TextureBuilder()
            .SetTarget(GL_TEXTURE_2D)
            .SetLevelOfDetail(0)
            .SetFormat(GL_RGB)
            .SetWidth(image->width)
            .SetHeight(image->height)
            .SetType(GL_UNSIGNED_BYTE)
            .SetData(image->data)
            .AddParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR)
            .AddParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
            .Build();
    }
}

void
CameraMovementApplication::OnInitialization()
{
    pmv_matrix = CreatePMVMatrix();
    program = CreateProgram();
    model = CreateModel();
    texture = CreateTexture();
}

void
CameraMovementApplication::OnDraw() const
{
    // Clear out the color and buffer.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    program->Use();

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
CameraMovementApplication::OnUpdate()
{
    blowgun::Matrix *current_pmv = pmv_matrix.get();
    pmv_matrix = std::make_shared<blowgun::Matrix>((*current_pmv)
        .Rotate(0.1f, 1.0f, 0.0f, 0.0f)
        .Rotate(0.1f, 0.0f, 1.0f, 0.0f)
        .Rotate(0.1f, 0.0f, 0.0f, 1.0f)
    );
}

void
CameraMovementApplication::OnDestroy()
{
    program->Delete();
    texture->Delete();
}
