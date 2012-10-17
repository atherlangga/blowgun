#ifndef BLOWGUN_VERTICES_H
#define BLOWGUN_VERTICES_H

namespace blowgun
{

namespace VerticesFormat
{
    enum Enum
    {
        kArrayOfStructures,
        kStructureOfArrays
    };
}

/**
 * A collection of vertex data and its metadata.
 */
class Vertices
{

public:

    /**
     * Upload the data to GPU.
     */
    void Upload();

private:
    /**
     * The format of the vertices, which can be one
     * of two options:
     * - Array of structures
     * - Structure of arrays
     */
    VerticesFormat::Enum format_;

    /**
     * The placement order of the data.
     */
    std::vector<std::string> order_;

    /**
     * The data itself.
     */
    void* data_;

friend class VerticesBuilder;

};

}

#endif // BLOWGUN_VERTICES_H
