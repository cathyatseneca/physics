#ifndef _MATH_DECLARATIONS_H_
#define _MATH_DECLARATIONS_H_

/* Mathematical Structures
 *
 * MathDeclarations.h
 * gam666/dps901/gam670/dps905
 * fwk4gps version 3.0
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

// material reflectivity
//
#define DEFAULT_SHININESS 10

//-------------------------------- Vector -------------------------------------
//
struct Matrix;

struct Vector {
    float x;
    float y;
    float z;
    Vector() : x(0), y(0), z(0) {}
    Vector(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
    Vector  operator+();
    Vector  operator-();
    Vector& operator+=(const Vector& a);
    Vector& operator-=(const Vector& a);
	Vector operator*(const Vector& v);

    Vector  operator*(const Matrix& m);
    Vector  operator*(const Matrix& m) const;
    Vector  operator*=(const Matrix& m);
    float   length() const;
};

inline Vector operator+(const Vector& a, const Vector& b) {

    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector operator-(const Vector& a, const Vector& b) {

    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector operator*(float s, const Vector& a) {

    return Vector(s * a.x, s * a.y, s * a.z);
}

inline Vector operator*(const Vector& a, float s) {

    return Vector(s * a.x, s * a.y, s * a.z);
}

inline Vector operator/(const Vector& a, float x) {

    return Vector(x ? a.x / x : a.x, x ? a.y / x : a.y, x ? a.z / x : a.z);
}

inline Vector cross(const Vector& a, const Vector& b) {

    return Vector(a.y * b.z - a.z * b.y,
                  a.z * b.x - a.x * b.z,
                  a.x * b.y - a.y * b.x);
}

inline float dot(const Vector& a, const Vector& b) {

    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector normal(const Vector& a) {

    return a / a.length();
}

//------------------------------- Matrix --------------------------------------
//
struct Matrix {
    float m11, m12, m13, m14;
    float m21, m22, m23, m24;
    float m31, m32, m33, m34;
    float m41, m42, m43, m44;
    Matrix() : m11(0), m12(0), m13(0), m14(0),
               m21(0), m22(0), m23(0), m24(0),
               m31(0), m32(0), m33(0), m34(0),
               m41(0), m42(0), m43(0), m44(0) {}
    Matrix(float x) : m11(x), m12(0), m13(0), m14(0),
                      m21(0), m22(x), m23(0), m24(0),
                      m31(0), m32(0), m33(x), m34(0),
                      m41(0), m42(0), m43(0), m44(1) {}
    Matrix(const Vector& v) : m11(v.x), m12(0), m13(0), m14(0),
                              m21(0), m22(v.y), m23(0), m24(0),
                              m31(0), m32(0), m33(v.z), m34(0),
                              m41(0), m42(0), m43(0), m44(1) {}
    Matrix(float i11, float i12, float i13, float i14,
           float i21, float i22, float i23, float i24,
           float i31, float i32, float i33, float i34,
           float i41, float i42, float i43, float i44) :
               m11(i11), m12(i12), m13(i13), m14(i14),
               m21(i21), m22(i22), m23(i23), m24(i24),
               m31(i31), m32(i32), m33(i33), m34(i34),
               m41(i41), m42(i42), m43(i43), m44(i44) {}
    Matrix& isIdentity();
    Matrix& operator+=(const Matrix& a);
    Matrix operator+(const Matrix& a);
	Matrix& operator-=(const Matrix& a);
    Matrix& operator*=(const Matrix& m);
    Matrix  transpose() const;
	Matrix& translate(float x, float y, float z);
	Matrix& rotatex(float rad);
	Matrix& rotatey(float rad);
	Matrix& rotatez(float rad);
	Matrix& rotate(const Vector& v, float rad);
	Matrix& rotate(const Matrix& rot);
	Matrix& scale(float sx, float sy, float sz);
	Matrix& orient(const Matrix& rot);
	Vector  position() const;
	Matrix  rotation() const;
	Vector  direction(const Vector& v) const;
	Vector  direction(char c) const;
};

Matrix rotate(const Vector& axis, float rad);

//-------------------------------- Plane --------------------------------------
//
struct Plane {
    Vector n;
    float  d;
    Plane(const Vector& v, float c) : n(v), d(c) {}
    bool onPositiveSide(const Vector& v) { return dot(n, v) + d < 0; }
    Vector normal() const { return n; }
    float constant() const { return d; }
};

//-------------------------------- Colour -------------------------------------
//
struct Colour {
    float r;
    float g;
    float b;
    float a;
    Colour(float red = 0, float grn = 0, float blu = 0, float alp = 1) :
	 r(red), g(grn), b(blu), a(alp) {}
    operator const float* () const { return &r; }
};

#define COLOUR_TO_ARGB(c) ((unsigned)((c.a) * 255) << 24 | \
 (unsigned)((c.r) * 255) << 16 | (unsigned)((c.g) * 255) << 8 | \
 (unsigned)((c.b) * 255)) 

//-------------------------------- Reflectivity -------------------------------
//
struct Reflectivity {
    Colour ambient;
    Colour diffuse;
    Colour specular;
    float  power;
    Reflectivity(Colour c = Colour(), float p = 0) {
        ambient  = Colour(c.r * 0.7f, c.g * 0.7f, c.b * 0.7f, c.a);
        diffuse  = c;
        specular = Colour(1, 1, 1, c.a);
        power    = p ? p : DEFAULT_SHININESS;
    }
    bool translucent() const { 
        return ambient.a != 1 || diffuse.a != 1 || specular.a != 1; 
    }
};

#endif
