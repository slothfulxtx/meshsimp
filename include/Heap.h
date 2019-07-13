#pragma once
#include "Vec3.h"
#include <Eigen/Dense>
#include "Vertex.h"
#include <algorithm>
#include <set>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/priority_queue.hpp>
struct VertexPair {
    Vertex *A, *B;
    double cost;
    VertexPair(Vertex *_A, Vertex *_B);
};

struct cmp {
    bool operator()(const VertexPair &A, const VertexPair &B)const {
        return (A.cost > B.cost) || (A.cost == B.cost && A.A > B.A) || (A.cost == B.cost && A.A == B.A && A.B > B.B);
    }
};

struct Heap {
    __gnu_pbds::priority_queue<VertexPair, cmp, __gnu_pbds::pairing_heap_tag> heap;
    std::set<Vertex *> delVs;
    void clear_top() {
        while(!heap.empty() && (delVs.find(heap.top().A) != delVs.end() || delVs.find(heap.top().B) != delVs.end()))
            heap.pop();
    }
    void push(VertexPair vp) {
        heap.push(vp);
    }
    VertexPair top() {
        clear_top();
        return heap.top();
    }
    void pop() {
        clear_top();
        heap.pop();
    }
    bool empty(){
        clear_top();
        return heap.empty();
    }
};