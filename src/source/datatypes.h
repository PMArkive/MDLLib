//
// Created by blake on 3/29/22.
//

#ifndef MDLLIB_DATATYPES_H
#define MDLLIB_DATATYPES_H
struct Vector{
    float x, y, z;
};

struct Quaternion{
    float x, y, z, w;
};

struct matrix3x4_t{
    float m_flMatVal[3][4];
};
#endif //MDLLIB_DATATYPES_H
