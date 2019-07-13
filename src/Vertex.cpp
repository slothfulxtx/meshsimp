#include "../include/Vertex.h"
#include "../include/Parameters.h"
Vertex vertex_pond[2 * MaxN + 10];
int vertex_pond_cnt = 0;
VertexPointer newVertex(const Vec3 &vec3) {
    VertexPointer ret = &vertex_pond[vertex_pond_cnt++];
    ret->pos = vec3;
    return ret;
}