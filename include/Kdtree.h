#pragma once
#include "Vec3.h"
#include "Vertex.h"
#include "Parameters.h"
#include <vector>
#include <set>
#include <stack>
struct KDTree {
    struct node {
        VertexPointer vertex=nullptr;
        Vec3 lo, hi;
        bool del=false;
        node *l=nullptr, *r=nullptr;

        void update() {
            if(!del)
                lo = hi = vertex->pos;
            else
                lo = Vec3(1e10, 1e10, 1e10), hi = Vec3(-1e10, -1e10, -1e10);
            if(l != nullptr) {
                lo = min(lo, l->lo);
                hi = max(hi, l->hi);
            }
            if(r != nullptr) {
                lo = min(lo, r->lo);
                hi = max(hi, r->hi);
            }
        }
    } Nodes[2 * MaxN + 10];
    std::stack<node *> Nodeptrs;
    node *root = nullptr;
    VertexPointer tmp[MaxN + 10];
    int tmp_cnt = 0;
    int limit_size = 0;
    int ins_size = 0;
    static int OPT;
    std::vector<VertexPointer> Ans;
    std::set<VertexPointer> vertexs;
    static bool cmp(const VertexPointer &a, const VertexPointer &b) {
        return (a->pos.at(OPT) < b->pos.at(OPT)) or ((a->pos.at(OPT) == b->pos.at(OPT)) and (a < b));
        //printf(")\n");
        ///return r;
    }
    void Init(VertexPointer vs[], int n);
    node *newnode(Vertex *_v);
    node *build(int l, int r, int dim);
    void dfs(node *rt);
    node *del(node *rt, Vertex *v, int dim);
    void Del(Vertex *v);
    node *ins(node *rt, Vertex *v, int dim);
    void Ins(Vertex *v);
    void search(node *rt, Vertex *v);
    void Search(Vertex *v);
    bool checkLegal();
    bool exists(node *rt,Vertex *v,int dim);
};
