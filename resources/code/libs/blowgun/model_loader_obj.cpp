#include "model_loader_obj.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <tuple>

#include "types.h"

using namespace blowgun;

ModelLoaderOBJ::ModelLoaderOBJ()
{
}

std::shared_ptr<Model>
ModelLoaderOBJ::Load(std::istream & stream)
{
    typedef struct V3_
    {
        float x;
        float y;
        float z;
    } V3;

    typedef struct V2_
    {
        float u;
        float v;
    } V2;

    std::vector<VertexAttribute> vertex_attributes;
    std::vector<std::tuple<u32, u32>> vertex_mappings;

    std::vector<V3> vertices_position;
    std::vector<V2> vertices_tex_coord;

    std::string line;
    while (std::getline(stream, line))
    {
        // Ignore empty line
        if (line.empty())
            continue;

        // Determine what kind of info the line contains of.
        std::string attr;
        std::stringstream ss(line.c_str());
        ss >> attr;
        const char* attr_cstr = attr.c_str();

        switch (attr_cstr[0])
        {
        case '#' :
            // The line that starts with '#' is comment line. So, do nothing.
            break;
        case 'v' :
            {
                switch (attr_cstr[1])
                {
                case '\0' :
                    // Found 'v' at this point. So, it's vertex position
                    // information.
                    // Sample:
                    // v  0.4000 -0.4000 0.0000
                    {
                        V3 v3;
                        ss >> v3.x >> v3.y >> v3.z;
                        vertices_position.push_back(v3);
                    }
                    break;
                case 't' :
                    // Found 'vt' at this point. So, it's vertex texture
                    // coordinate information.
                    // Sample:
                    // vt 0.6119 0.8867 0.0000
                    {
                        V2 v2;
                        ss >> v2.u >> v2.v;
                        vertices_tex_coord.push_back(v2);
                    }
                default:
                    break;
                }
            }
            break;
        case 'f' :
            {
                // Found 'f'. It means "face" information is found.
                // So, we're gonna construct complete set of vertex
                // attributes. By the time execution goes to this
                // point, all vertex attributes has had to be parsed.
                // Sample:
                // f 1/1 2/2 3/3
                std::string face_entry;
                while (ss >> face_entry)
                {
                    int indices[3];
                    std::tuple<u32, u32> vertex_mapping;

                    // Although the specified format is `%d/%d/%d` below,
                    // `%d/%d` will also be read successfully.
                    int element_count = std::sscanf(
                        face_entry.c_str(),
                        "%d/%d/%d",
                        &indices[0],
                        &indices[1],
                        &indices[2]);

                    if (element_count == 2)
                    {
                        // If the element count is 2, then it means the
                        // face contains position and texcoord.
                        vertex_mapping = std::make_tuple(indices[0], indices[1]);
                    }
                    else if (element_count == 3)
                    {
                        // If the element count is 3, then it means the
                        // face contains position, normal, and texcoord.
                        // For now, we only need position and texcoord,
                        // which means we need to take first and third
                        // indices.
                        vertex_mapping = std::make_tuple(indices[0], indices[2]);
                    }

                    vertex_mappings.push_back(vertex_mapping);
                }
            }
        default :
            break;
        }
    }

    for (auto vertex_mapping : vertex_mappings)
    {
        u32 position_index = std::get<0>(vertex_mapping);
        u32 texture_index  = std::get<1>(vertex_mapping);

        ///
        // Process vertex position information.
        ///

        VertexAttributeData va_position_data;

        va_position_data.float_3[0] = vertices_position[position_index - 1].x;
        va_position_data.float_3[1] = vertices_position[position_index - 1].y;
        va_position_data.float_3[2] = vertices_position[position_index - 1].z;

        VertexAttribute va_position;
        va_position.format = VertexAttributeFormat::kFloat3;
        va_position.data   = va_position_data;

        vertex_attributes.push_back(va_position);

        ///
        // Process vertex texture coordinate information.
        ///

        VertexAttributeData va_tex_coord_data;

        va_tex_coord_data.float_2[0] = vertices_tex_coord[texture_index - 1].u;
        va_tex_coord_data.float_2[1] = vertices_tex_coord[texture_index - 1].v;

        VertexAttribute va_texture;
        va_texture.format = VertexAttributeFormat::kFloat2;
        va_texture.data   = va_tex_coord_data;

        vertex_attributes.push_back(va_texture);
    }

    return std::make_shared<Model>(vertex_attributes);
}
