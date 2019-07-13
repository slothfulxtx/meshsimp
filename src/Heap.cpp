#include "../include/Heap.h"
VertexPair::VertexPair(Vertex *_A, Vertex *_B): A(_A), B(_B) {
    //assert(A && B);
    Eigen::Matrix<double, 4, 4> Q = A->Q + B->Q;
    Eigen::Matrix<double, 4, 4> Q2 = Q;
    Q2(3, 0) = Q2(3, 1) = Q2(3, 2) = 0;
    Q2(3, 3) = 1;
    if(fabs(Q2.determinant()) > 1e-7) {
        Eigen::Matrix<double, 4, 4> m = Q2.inverse();
        Eigen::Vector4d v(0, 0, 0, 1);
        v = m * v;
        cost = (v.transpose() * Q * v)(0, 0);
    } else {
        Eigen::Vector4d v1 = A->pos.toVector4();
        Eigen::Vector4d v2 = B->pos.toVector4();
        double a = ((v2 - v1).transpose() * Q * (v2 - v1))(0, 0);
        double b = ((v2 - v1).transpose() * Q * v1 + v1.transpose() * Q * (v2 - v1))(0, 0);
        double c = (v1.transpose() * Q * v1)(0, 0);
        // a*k^2+b*k+c
        if(fabs(a) < 1e-7) {
            if(fabs(b) < 1e-7)
                cost = c;
            else if(b > 0)
                cost = (v1.transpose() * Q * v1)(0, 0);
            else
                cost = (v2.transpose() * Q * v2)(0, 0);
        } else {
            if(a > 0) {
                double k = -b / (2 * a);
                if(k < 0)
                    cost = (v1.transpose() * Q * v1)(0, 0);
                else if(k > 1)
                    cost = (v2.transpose() * Q * v2)(0, 0);
                else
                    cost = ((v1 + (v2 - v1) * k).transpose() * Q * (v1 + (v2 - v1) * k))(0, 0);
            } else {
                cost = std::min((v1.transpose() * Q * v1)(0, 0), (v2.transpose() * Q * v2)(0, 0));
            }
        }
    }
}