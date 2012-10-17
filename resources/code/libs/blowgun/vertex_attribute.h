#ifndef BLOWGUN_VERTEX_ATTRIBUTE_H_
#define BLOWGUN_VERTEX_ATTRIBUTE_H_

#include "types.h"

namespace blowgun
{

namespace VertexAttributeFormat
{
    enum Enum
    {
        kFloat1        = 0,
        kFloat2        = 1,
        kFloat3        = 2,
        kFloat4        = 3,
        kUnsignedByte4 = 4
    };
}

union VertexAttributeData
{
    float float_1[1];
    float float_2[2];
    float float_3[3];
    float float_4[4];
    u8    u8_4[4];
};


struct VertexAttribute
{
    VertexAttributeFormat::Enum  format;
    VertexAttributeData          data;
};

}

#endif // BLOWGUN_VERTEX_ATTRIBUTE_H_
