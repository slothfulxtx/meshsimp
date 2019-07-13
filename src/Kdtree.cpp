#include "../include/Kdtree.h"
#include <algorithm>
int KDTree::OPT = 0;

bool KDTree::checkLegal(){
    for(auto v : vertexs)
        assert(exists(root,v,0));
}
bool KDTree::exists(node* rt, Vertex* v,int dim){
    printf("%d\n",dim);
    assert(rt);
    assert(v);
    if(!rt->del&&rt->vertex==v)
        return true;
    printf("~%d\n",dim);
    OPT=dim%3;
    printf("shit!!!\n");
    if(cmp(v,rt->vertex)){
        printf("!!!!!\n");
        if(rt->l)
            return exists(rt->l,v,dim+1);
    }else{
        printf("~~~~~\n");
        if(rt->r)
            return exists(rt->r,v,dim+1);
    }
    return false;
}

void KDTree::Init(VertexPointer vs[], int n) {
    vertexs.clear();
    for(int i = 0; i < n; i++) {
        tmp[i] = vs[i];
        vertexs.insert(vs[i]);
    }
    limit_size = std::max(int(n / deltime), 10000);
    if(threshold == 0)return;
    for(int i = 0; i < 5*n; i++)Nodeptrs.push(&Nodes[i]);
    root = build(0, n - 1, 0);
//    printf("!!!!\n");
//checkLegal();
//printf("@@@\n");
}

KDTree::node *KDTree::newnode(Vertex *_v) {
    node *ret = Nodeptrs.top();
    Nodeptrs.pop();
    ret->vertex = _v;
    ret->l = ret->r =  nullptr;
    ret->del = false;
    ret->lo = ret->hi = _v->pos;
    return ret;
}
KDTree::node *KDTree::build(int l, int r, int dim) {
    if(l > r)return nullptr;
    int mid = (l + r) >> 1;
    OPT = dim;
    std::nth_element(tmp + l, tmp + mid, tmp + r + 1, cmp);
    node *ret = newnode(tmp[mid]);
    if(l < mid)ret->l = build(l, mid - 1, (dim + 1) % 3);
    if(mid < r)ret->r = build(mid + 1, r, (dim + 1) % 3);
    ret->update();
    return ret;
}
void KDTree::dfs(node *rt) {
    if(!rt->del)
        tmp[tmp_cnt++] = rt->vertex;
    Nodeptrs.push(rt);
    if(rt->l)
        dfs(rt->l);
    if(rt->r)
        dfs(rt->r);
}
KDTree::node *KDTree::del(node *rt, Vertex *v, int dim) {
    OPT = dim;
    if(!rt)return nullptr;
    //assert(rt!=nullptr);
    if(rt->vertex == v) {
        rt->del = true;
    } else if(cmp(v, rt->vertex))
        rt->l = del(rt->l, v, (dim + 1) % 3);
    else
        rt->r = del(rt->r, v, (dim + 1) % 3);
    rt->update();
    return rt;
}
void KDTree::Del(Vertex *v) {
    //assert(vertexs.find(v)!=vertexs.end());
    //printf("???\n");
    vertexs.erase(v);
    if(threshold == 0)return;
    root = del(root, v, 0);
    //printf("!!!\n");
}
KDTree::node *KDTree::ins(node *rt, Vertex *v, int dim) {
    if(rt == nullptr) {
        node *ret = newnode(v);
        return ret;
    }
    OPT = dim;
    if(cmp(v, rt->vertex)) {
        rt->l = ins(rt->l, v, (dim + 1) % 3);
        rt->update();
    } else {
        rt->r = ins(rt->r, v, (dim + 1) % 3);
        rt->update();
    }
    return rt;
}
void KDTree::Ins(Vertex *v) {
    vertexs.insert(v);
    //printf("%d %d\n",ins_size,limit_size);
    if(threshold == 0)return;
    ins_size++;
    if(ins_size == limit_size) {
        limit_size*=0.8;
        limit_size = std::max(limit_size,1000);
        ins_size = 0;
        tmp_cnt = 0;
        dfs(root);
        tmp[tmp_cnt++] = v;
        root = build(0, tmp_cnt - 1, 0);
    } else {
        root = ins(root, v, 0);
    }
}
void KDTree::search(node *rt, Vertex *v) {
    double mindis = 0;
    for(int i = 0; i < 3; i++) {
        if(v->pos[i] < rt->lo[i])
            mindis += (rt->lo[i] - v->pos[i]) * (rt->lo[i] - v->pos[i]);
        if(v->pos[i] > rt->hi[i])
            mindis += (v->pos[i] - rt->hi[i]) * (v->pos[i] - rt->hi[i]);
    }
    if(mindis > threshold * threshold)
        return;
    if(!rt->del && (rt->vertex->pos - v->pos).length() < threshold)
        Ans.push_back(rt->vertex);
    if(rt->l)
        search(rt->l, v);
    if(rt->r)
        search(rt->r, v);
}
void KDTree::Search(Vertex *v) {
    if(threshold == 0)return;
    Ans.clear();
    search(root, v);
}