#pragma once
#include <Eigen/Dense>
struct Vec3 {
    double x, y, z;
    constexpr static double eps = 1e-7;
    double &operator[] (const int &index) {
        return index == 0 ? x : (index == 1 ? y : z);
    }
    Vec3(): x(0), y(0), z(0) {}
    Vec3(const double &_x, const double &_y, const double &_z): x(_x), y(_y), z(_z) {}
    double at(const int &index)const {
        return index == 0 ? x : (index == 1 ? y : z);
    }
    bool operator==(const Vec3 &that)const {
        return (fabs(this->x - that.x) < eps) && (fabs(this->y - that.y) < eps) && (fabs(this->z - that.z) < eps);
    }
    bool operator!=(const Vec3 &that)const {
        return (fabs(this->x - that.x) >= eps) || (fabs(this->y - that.y) >= eps) || (fabs(this->z - that.z) >= eps);
    }
    Vec3 operator*(const Vec3 &that)const {
        return Vec3(this->y * that.z - this->z * that.y, this->z * that.x - this->x * that.z, this->x * that.y - this->y * that.x);
    }
    Vec3 operator-(const Vec3 &that)const {
        return Vec3(this->x - that.x, this->y - that.y, this->z - that.z);
    }
    Vec3 operator+(const Vec3 &that)const {
        return Vec3(this->x + that.x, this->y + that.y, this->z + that.z);
    }
    double operator^(const Vec3 &that)const {
        return this->x * that.x + this->y * that.y + this->z * that.z;
    }
    Vec3 operator/(const double &k)const {
        return Vec3(this->x / k, this->y / k, this->z / k);
    }
    Vec3 operator*(const double &k)const {
        return Vec3(this->x * k, this->y * k, this->z * k);
    }
    double length()const {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }
    void normalize() {
        double l = this->length();
        if(l < eps) {
            this->x = this->y = this->z = 0.0;
        } else {
            this->x /= l;
            this->y /= l;
            this->z /= l;
        }
    }
    Eigen::Vector4d toVector4()const {
        return Eigen::Vector4d(x, y, z, 1);
    }
};
Vec3 max(const Vec3 &a, const Vec3 &b);
Vec3 min(const Vec3 &a, const Vec3 &b);