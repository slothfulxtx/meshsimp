#include "../include/Vec3.h"
Vec3 max(const Vec3 &a, const Vec3 &b) {
    Vec3 ret;
    ret.x = a.x > b.x ? a.x : b.x;
    ret.y = a.y > b.y ? a.y : b.y;
    ret.z = a.z > b.z ? a.z : b.z;
    return ret;
}
Vec3 min(const Vec3 &a, const Vec3 &b) {
    Vec3 ret;
    ret.x = a.x > b.x ? b.x : a.x;
    ret.y = a.y > b.y ? b.y : a.y;
    ret.z = a.z > b.z ? b.z : a.z;
    return ret;
}