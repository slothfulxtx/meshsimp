#pragma once
#include "Vec3.h"
#include <Eigen/Dense>
#include <set>
struct Facet;
struct Vertex {
    Vec3 pos;
    Eigen::Matrix<double, 4, 4> Q;
    std::set<Facet *>facets;
};
typedef Vertex* VertexPointer;
VertexPointer newVertex(const Vec3 &vec3);