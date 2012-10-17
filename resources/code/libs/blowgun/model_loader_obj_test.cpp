#include <fstream>
#include <iostream>
#include <sstream>

#include <gtest/gtest.h>
#include "model_loader_obj.h"

using namespace blowgun;

TEST(ModelLoaderOBJTest, ParseSimpleObj)
{
    const char simpleObjCString[] =
        "v  -0.4000 -0.4000 0.0000\n"
        "v  0.4000 -0.4000 0.0000\n"
        "v  0.0000 0.4000 0.0000\n"
        "vt 0.6119 0.8867 0.0000\n"
        "vt 0.6384 0.9396 0.0000\n"
        "vt 0.5803 0.9503 0.0000\n"
        "f 1/1 2/2 3/3\n";

    const std::string simpleObjString(simpleObjCString);
    std::stringstream simpleObjStream(simpleObjString, std::ios::in);

    ModelLoaderOBJ loader;
    auto model = loader.Load(simpleObjStream);
    auto vertices_attributes = model->vertex_attributes();

    EXPECT_EQ(6, (int) vertices_attributes.size());
}

TEST(ModelLoaderOBJTest, ParseCubeObj)
{
    ModelLoaderOBJ loader;
    std::ifstream cubeObjFile("data/cube.obj", std::ios::in);
    auto model = loader.Load(cubeObjFile);
    auto vertices_attributes = model->vertex_attributes();

    EXPECT_EQ(72, (int) vertices_attributes.size());
}