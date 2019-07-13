#include "../include/Facet.h"
#include <Eigen/Dense>
#include "../include/Parameters.h"
void Facet::contributeQ() {
    double t[4];
    compute(t[0], t[1], t[2], t[3]);
    Eigen::Matrix<double, 4, 4> tmp;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            tmp(i, j) = t[i] * t[j];
    for(size_t i = 0; i < 3; i++)
        vertexs[i]->Q += tmp;
}
bool Facet::intersect(Vertex *A, Vertex *B) {
    double a, b, c, d;
    compute(a, b, c, d);
    Vec3 tmp = Vec3(a, b, c);
    if(((A->pos ^ tmp) + d >= 0 && (B->pos ^ tmp) >= 0) || ((A->pos ^ tmp) + d <= 0 && (B->pos ^ tmp) + d <= 0))
        return false;
    Line e1(vertexs[1], vertexs[0]), e2(vertexs[2], vertexs[1]), e3(vertexs[0], vertexs[2]), l(A, B);
    double s1 = l.sideOp(e1), s2 = l.sideOp(e2), s3 = l.sideOp(e3);
    return (s1 > eps && s2 > eps && s3 > eps) || (s1 < -eps && s2 < -eps && s3 < -eps);
}

Facet facet_pond[MaxF + 10];
int facet_pond_cnt = 0;
FacetPointer newFacet(VertexPointer &A, VertexPointer &B, VertexPointer &C) {
    FacetPointer ret = &facet_pond[facet_pond_cnt++];
    ret->vertexs[0] = A;
    ret->vertexs[1] = B;
    ret->vertexs[2] = C;
    return ret;
}