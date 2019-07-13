#pragma once
#include "Vec3.h"
#include "Vertex.h"
struct Facet {
    VertexPointer vertexs[3];
    struct Line {
        double l0, l1, l2, l3, l4, l5;
        Line() {}
        Line(Vertex *a, Vertex *b):
            l0(a->pos.x * b->pos.y - a->pos.y * b->pos.x),
            l1(a->pos.x * b->pos.z - a->pos.z * b->pos.x),
            l2(a->pos.x - b->pos.x),
            l3(a->pos.y * b->pos.z - a->pos.z * b->pos.y),
            l4(a->pos.z - b->pos.z),
            l5(b->pos.y - a->pos.y) {
        }
        double sideOp(const Line &b)const {
            return l0 * b.l4 + l1 * b.l5 + l2 * b.l3 + l3 * b.l2 + l4 * b.l0 + l5 * b.l1;
        }
    };
    constexpr static double eps = 1e-6;
    bool exist() {
        return (vertexs[0]->pos != vertexs[1]->pos) && (vertexs[1]->pos != vertexs[2]->pos) && (vertexs[2]->pos != vertexs[0]->pos);
    }

    Vec3 normal() {
        Vec3 n = (vertexs[1]->pos - vertexs[0]->pos) * (vertexs[2]->pos - vertexs[0]->pos);
        n.normalize();
        return n;
    }
    void compute(double &a, double &b, double &c, double &d) {
        Vec3 n = normal();
        a = n.x;
        b = n.y;
        c = n.z;
        d = -(n ^ vertexs[0]->pos);
    }
    void join() {
        vertexs[0]->facets.insert(this);
        vertexs[1]->facets.insert(this);
        vertexs[2]->facets.insert(this);
    }
    void remove() {
        vertexs[0]->facets.erase(this);
        vertexs[1]->facets.erase(this);
        vertexs[2]->facets.erase(this);
    }
    void contributeQ();
    bool intersect(Vertex *A, Vertex *B);
};
typedef Facet *FacetPointer;
FacetPointer newFacet(VertexPointer &A, VertexPointer &B, VertexPointer &C);