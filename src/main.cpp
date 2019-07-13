#include "../include/Facet.h"
#include "../include/Heap.h"
#include "../include/Vertex.h"
#include "../include/Kdtree.h"
#include "../include/Parameters.h"
#include "../include/Vec3.h"
#include <iostream>
bool Force = false;
KDTree kdtree;
Heap heap;
Vertex tmp;
Facet tmp2;
Facet tmpf[100];
int tmpf_cnt;

Vertex *compute_best_vertex(Vertex *A, Vertex *B) {
    Eigen::Matrix<double, 4, 4> Q = A->Q + B->Q;
    Eigen::Matrix<double, 4, 4> Q2 = Q;
    Q2(3, 0) = Q2(3, 1) = Q2(3, 2) = 0;
    Q2(3, 3) = 1;
    Vertex *C = &tmp;
    if(fabs(Q2.determinant()) > 1e-7) {
        Eigen::Matrix<double, 4, 4> m = Q2.inverse();
        Eigen::Vector4d v(0, 0, 0, 1);
        v = m * v;
        C->pos = {v(0), v(1), v(2)};
    } else {
        Eigen::Vector4d v1 = A->pos.toVector4();
        Eigen::Vector4d v2 = B->pos.toVector4();
        double a = ((v2 - v1).transpose() * Q * (v2 - v1))(0, 0);
        double b = ((v2 - v1).transpose() * Q * v1 + v1.transpose() * Q * (v2 - v1))(0, 0);
        double c = (v1.transpose() * Q * v1)(0, 0);
        // a*k^2+b*k+c
        if(fabs(a) < 1e-7) {
            if(fabs(b) < 1e-7)
                C->pos = (A->pos + B->pos) / 2;
            else if(b > 0)
                C->pos = A->pos;
            else
                C->pos = B->pos;
        } else {
            double k = -b / (2 * a);
            if(a > 0) {
                if(k < 0)
                    C->pos = A->pos;
                else if(k > 1)
                    C->pos = B->pos;
                else
                    C->pos = A->pos + (B->pos - A->pos) * k;
            } else {
                C->pos = k > 0.5 ? A->pos : B->pos;
            }
        }
    }
    if(Force)return C;
    tmpf_cnt = 0;
    //printf("****\n");
    for(auto facet : A->facets) {
        bool exist = true;
        for(int i = 0; i < 3; i++)
            if(facet->vertexs[i] == B) {
                exist = false;
                break;
            }
        if(exist) {
            for(int i = 0; i < 3; i++)
                if(facet->vertexs[i] == A) {
                    for(int j = 0; j < 3; j++)
                        if(j != i)
                            tmpf[tmpf_cnt].vertexs[j] = facet->vertexs[j];
                        else
                            tmpf[tmpf_cnt].vertexs[j] = C;
                    if((tmpf[tmpf_cnt].normal()^facet->normal()) < 0) {
                        C = nullptr;
                    }
                    break;
                }
            if(!C)break;
            tmpf_cnt++;
        }
    }
    if(!C)return nullptr;
    for(auto facet : B->facets) {
        bool exist = true;
        for(int i = 0; i < 3; i++)
            if(facet->vertexs[i] == A) {
                exist = false;
                break;
            }
        if(exist) {
            for(int i = 0; i < 3; i++)
                if(facet->vertexs[i] == B) {
                    for(int j = 0; j < 3; j++)
                        if(j != i)
                            tmpf[tmpf_cnt].vertexs[j] = facet->vertexs[j];
                        else
                            tmpf[tmpf_cnt].vertexs[j] = C;
                    if((tmpf[tmpf_cnt].normal()^facet->normal()) < 0) {
                        C = nullptr;
                    }
                    break;
                }
            if(!C)break;
            tmpf_cnt++;
        }
    }
    if(!C)return nullptr;

    Vertex *a, *b, *c, *d;

    for(int i = 0; i < tmpf_cnt && C; i++)
        for(int j = 0; j < 3 && C; j++)if(tmpf[i].vertexs[j] != C) {
                a = tmpf[i].vertexs[(j + 1) % 3];
                b = tmpf[i].vertexs[(j + 2) % 3];
                for(auto facet : tmpf[i].vertexs[j]->facets) {
                    for(int k = 0; k < 3; k++)if(facet->vertexs[k] == tmpf[i].vertexs[j]) {
                            c = facet->vertexs[(k + 1) % 3];
                            d = facet->vertexs[(k + 2) % 3];
                            break;
                        }
                    if(tmpf[i].intersect(c, d))
                        C = nullptr;
                    else if(facet->intersect(a, b))
                        C = nullptr;
                }
            }

    /*for(int i = 0; i < tmpf_cnt && C; i++)
        for(int j = i + 1; j < tmpf_cnt && C; j++) {
            for(int k = 0; k < 3; k++)if(tmpf[i].vertexs[k] == C) {
                    a = tmpf[i].vertexs[(k + 1) % 3];
                    b = tmpf[i].vertexs[(k + 2) % 3];
                    break;
                }
            for(int k = 0; k < 3; k++)if(tmpf[j].vertexs[k] == C) {
                    c = tmpf[j].vertexs[(k + 1) % 3];
                    d = tmpf[j].vertexs[(k + 2) % 3];
                    break;
                }
            if(tmpf[i].intersect(c, d))
                C = nullptr;
            else if(tmpf[j].intersect(a, b))
                C = nullptr;
        }*/

    return C ? newVertex(C->pos) : nullptr;
}
int cntVertex = 0,cntFacet = 0;
VertexPointer tmpVertex[MaxN+10];
FacetPointer tmpFacet[MaxF+10];
int main(int argc, char **argv) {
    if(argc < 4)
        return 1;
    double simp_rate = atof(argv[3]);
    char *inFileName = argv[1];
    char *outFileName = argv[2];
    if(argc==5)
        Force = true;
    printf("Force = %d\n",Force);
    char opt[10], buffer[1000];

    freopen(inFileName, "r", stdin);
    //int n = 0;
    while(scanf("%s", opt) != EOF) {
        if(strlen(opt) != 1) {
            std::cin.getline(buffer, 1000);
            continue;
        }
        double x, y, z;
        int a, b, c;
        switch(opt[0]) {
        case 'v':
            scanf("%lf%lf%lf", &x, &y, &z);
            tmpVertex[cntVertex++] = newVertex({x,y,z});
            break;
        case 'f':
            scanf("%d%d%d", &a, &b, &c);
            tmpFacet[cntFacet++] = newFacet(tmpVertex[a-1],tmpVertex[b-1],tmpVertex[c-1]);
            break;
        default:
            std::cin.getline(buffer, 1000);
            break;
        }
    }
    fclose(stdin);
    for(int i=0;i<cntFacet;i++){
        tmpFacet[i]->join();
        tmpFacet[i]->contributeQ();
    }

    std::map<Vertex *, Facet *>nbs;
    Vertex *v;
    for(int i = 0; i < cntVertex; i++) {
        v = tmpVertex[i];
        nbs.clear();
        for(auto facet : v->facets)
            for(int j = 0; j < 3; j++)if(facet->vertexs[j] == v) {
                    Vertex *v2 = facet->vertexs[(j + 1) % 3];
                    Vertex *v3 = facet->vertexs[(j + 2) % 3];
                    if(nbs.find(v2) == nbs.end())
                        nbs[v2] = facet;
                    else
                        nbs.erase(v2);
                    if(nbs.find(v3) == nbs.end())
                        nbs[v3] = facet;
                    else
                        nbs.erase(v3);
                }
        for(auto iter = nbs.begin(); iter != nbs.end(); iter++) {
            Vec3 s = (iter->second->normal() * (iter->first->pos - v->pos));
            s.normalize();
            double tmp[4] = {s.x, s.y, s.z, -(s ^ v->pos)};
            Eigen::Matrix<double, 4, 4> Q;
            for(int i = 0; i < 4; i++)
                for(int j = 0; j < 4; j++)
                    Q(i, j) = tmp[i] * tmp[j];
            v->Q += Q;
        }
    }
    std::set<Vertex *> neighbours;
    kdtree.Init(tmpVertex, cntVertex);
    for(int i = 0; i < cntVertex; i++) {
        v = tmpVertex[i];
        neighbours.clear();
        neighbours.insert(v);
        kdtree.Search(v);
        for(auto nd : kdtree.Ans) {
            if(v < nd && neighbours.find(nd) == neighbours.end()) {
                heap.push({v, nd});
                neighbours.insert(nd);
            }
        }
        for(auto facet : v->facets)
            for(int i = 0; i < 3; i++)if(facet->vertexs[i] == v) {
                    Vertex *v2 = facet->vertexs[(i + 1) % 3];
                    Vertex *v3 = facet->vertexs[(i + 2) % 3];
                    //assert(v2 && v3 && (v2 != v3));
                    if(v < v2 && neighbours.find(v2) == neighbours.end()) {
                        heap.push({v, v2});
                        neighbours.insert(v2);
                    }
                    if(v < v3 && neighbours.find(v3) == neighbours.end()) {
                        heap.push({v, v3});
                        neighbours.insert(v3);
                    }
                    break;
                }
    }
    int T = int(cntVertex - cntVertex * simp_rate);
    //assert(T > 0);
    std::vector<Facet *> vf;
    int failcnt = 0;
    while(T--) {
        //if(T%5000==0)
        if(T%1000==0)
            printf("%d nodes to delete\n", T);
        if(heap.empty())break;
        VertexPair vp = heap.top();
        heap.pop();
        //kdtree.checkLegal();
        Vertex *best = compute_best_vertex(vp.A, vp.B);
        if(best != nullptr) {
            heap.delVs.insert(vp.A);
            heap.delVs.insert(vp.B);
            //printf("-->>>>\n");
            kdtree.Del(vp.A);
            kdtree.Del(vp.B);
            vf.clear();
            //printf("-->>>\n");
            for(auto facet : vp.A->facets) {
                bool Flag = true;
                for(int i = 0; i < 3 && Flag; i++)
                    if(facet->vertexs[i] == vp.B) {
                        Flag = false;
                        vf.push_back(facet);
                    }
                if(!Flag)continue;
                for(int i = 0; i < 3; i++)
                    if(facet->vertexs[i] == vp.A) {
                        facet->vertexs[i] = best;
                        (best->facets).insert(facet);
                        break;
                    }
            }
            //printf("-->>\n");
            for(auto facet : vp.B->facets) {
                bool Flag = true;
                for(int i = 0; i < 3 && Flag; i++)
                    if(facet->vertexs[i] == vp.A) {
                        Flag = false;
                    }
                if(!Flag)continue;
                for(int i = 0; i < 3; i++)
                    if(facet->vertexs[i] == vp.B) {
                        facet->vertexs[i] = best;
                        (best->facets).insert(facet);
                        break;
                    }
            }
            for(auto facet : vf) {
                facet->remove();
            }
            //printf("&&^^^^\n");
            best->Q = vp.A->Q + vp.B->Q;
            neighbours.clear();
            neighbours.insert(best);
            kdtree.Search(best);
            //printf("-->%d\n", T);
            for(auto nd : kdtree.Ans) {
                if(neighbours.find(nd) == neighbours.end()) {
                    heap.push({best, nd});
                    neighbours.insert(nd);
                }
            }
            for(auto facet : best->facets) {
                //assert(facet);
                for(int i = 0; i < 3; i++)
                    if(facet->vertexs[i] == best) {
                        //assert(best);
                        Vertex *v2 = facet->vertexs[(i + 1) % 3];
                        //assert(v2);
                        Vertex *v3 = facet->vertexs[(i + 2) % 3];
                        //assert(v3);
                        //assert(best && v2 && v3);
                        //printf("!!!\n");
                        if(neighbours.find(v2) == neighbours.end()) {
                            heap.push({best, v2});
                            neighbours.insert(v2);
                        }

                        if(neighbours.find(v3) == neighbours.end()) {
                            heap.push({best, v3});
                            neighbours.insert(v3);
                        }
                        //printf("!!!2\n");
                        break;
                    }
            }

            //printf("&&&&&^^^^\n");
            kdtree.Ins(best);
            //printf("&&&&&&^^^^\n");
        } else {
            //printf("Fail!!!!!!!!!!\n");
            T++;
            failcnt++;
        }
        /*if(T<25){
            char name[20];
            sprintf(name,"%d.obj",T);
            freopen(name, "w", stdout);
            std::map<Vertex *, int> ID;
            std::set<Facet *>facets;
            int cnt = 0;
            for(auto v : kdtree.vertexs) {
                printf("v %lf %lf %lf\n", v->pos[0], v->pos[1], v->pos[2]);
                ID[v] = ++cnt;
                for(auto facet : v->facets)
                    facets.insert(facet);
            }
            for(auto facet : facets) {
                printf("f %d %d %d\n", int(ID[facet->vertexs[0]]), int(ID[facet->vertexs[1]]), int(ID[facet->vertexs[2]]));
            }
            fclose(stdout);
        }*/

    }
    //printf("fail = %d\n", failcnt);
    freopen(outFileName, "w", stdout);
    /*for(auto v : kdtree.vertexs)
        for(auto f : v->facets)
            assert(kdtree.vertexs.find(f->vertexs[0]) != kdtree.vertexs.end() && kdtree.vertexs.find(f->vertexs[1]) != kdtree.vertexs.end() && kdtree.vertexs.find(f->vertexs[2]) != kdtree.vertexs.end());
    */
    std::map<Vertex *, int> ID;
    std::set<Facet *>facets;
    int cnt = 0;
    for(auto v : kdtree.vertexs) {
        printf("v %lf %lf %lf\n", v->pos[0], v->pos[1], v->pos[2]);
        ID[v] = ++cnt;
        for(auto facet : v->facets)
            facets.insert(facet);
    }
    for(auto facet : facets) {
        printf("f %d %d %d\n", int(ID[facet->vertexs[0]]), int(ID[facet->vertexs[1]]), int(ID[facet->vertexs[2]]));
    }
    fclose(stdout);
    return 0;
}
