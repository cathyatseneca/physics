#ifndef _MATH_DEFINITIONS_H_
#define _MATH_DEFINITIONS_H_

/* Definitions of the Mathematical Functions
 *
 * MathDefinitions.h
 * gam666/dps901/gam670/dps905
 * fwk4gps version 3.0
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#define _USE_MATH_DEFINES
#include <math.h>             // for sqrtf, tanf, cosf, sinf
#include "MathDeclarations.h" // for Vector, Matrix and Colour declarations
#include "GeneralConstants.h" // for MODEL_Z_AXIS

//-------------------------------- Vector -------------------------------------
//
inline Vector& Vector::operator+=(const Vector& a) {

    x += a.x;
    y += a.y;
    z += a.z;
    return *this;
}

inline Vector Vector::operator+() {

    return *this;
}

inline Vector Vector::operator-() {

    return Vector(-x, -y, -z);
}

inline Vector& Vector::operator-=(const Vector& a) {

    x -= a.x;
    y -= a.y;
    z -= a.z;
    return *this;
}

inline float Vector::length() const {

    return sqrtf(dot(*this, *this));
}

//------------------------------- Matrix --------------------------------
//

inline Vector Vector::operator*=(const Matrix& m) {

    *this = Vector(x * m.m11 + y * m.m21 + z * m.m31 + m.m41,
                   x * m.m12 + y * m.m22 + z * m.m32 + m.m42,
                   x * m.m13 + y * m.m23 + z * m.m33 + m.m43);
    return *this;
}

inline Vector Vector::operator*(const Matrix& m) {

    return Vector(x * m.m11 + y * m.m21 + z * m.m31 + m.m41,
                  x * m.m12 + y * m.m22 + z * m.m32 + m.m42,
                  x * m.m13 + y * m.m23 + z * m.m33 + m.m43);
}

inline Vector Vector::operator*(const Matrix& m) const {

    return Vector(x * m.m11 + y * m.m21 + z * m.m31 + m.m41,
                  x * m.m12 + y * m.m22 + z * m.m32 + m.m42,
                  x * m.m13 + y * m.m23 + z * m.m33 + m.m43);
}

inline Matrix& Matrix::isIdentity() {

    return *this = Matrix(1);
}

inline Matrix Matrix::transpose() const {
    return Matrix(m11, m21, m31, m41,
     m12, m22, m32, m42,
     m13, m23, m33, m43,
     m14, m24, m34, m44);
}

inline Matrix& Matrix::operator+=(const Matrix& a) {

    m11 += a.m11;
    m12 += a.m12;
    m13 += a.m13;
    m14 += a.m14;
    m21 += a.m21;
    m22 += a.m22;
    m23 += a.m23;
    m24 += a.m24;
    m31 += a.m31;
    m32 += a.m32;
    m33 += a.m33;
    m34 += a.m34;
    m41 += a.m41;
    m42 += a.m42;
    m43 += a.m43;
    m44 += a.m44;
    return *this;
}
inline Matrix Matrix::operator+(const Matrix& a) {
	Matrix rc=*this;
	rc.m11 += a.m11;
    rc.m12 += a.m12;
    rc.m13 += a.m13;
    rc.m14 += a.m14;
    rc.m21 += a.m21;
    rc.m22 += a.m22;
    rc.m23 += a.m23;
    rc.m24 += a.m24;
    rc.m31 += a.m31;
    rc.m32 += a.m32;
    rc.m33 += a.m33;
    rc.m34 += a.m34;
    rc.m41 += a.m41;
    rc.m42 += a.m42;
    rc.m43 += a.m43;
    rc.m44 += a.m44;
    return rc;
}

inline Matrix& Matrix::operator-=(const Matrix& a) {

    m11 -= a.m11;
    m12 -= a.m12;
    m13 -= a.m13;
    m14 -= a.m14;
    m21 -= a.m21;
    m22 -= a.m22;
    m23 -= a.m23;
    m24 -= a.m24;
    m31 -= a.m31;
    m32 -= a.m32;
    m33 -= a.m33;
    m34 -= a.m34;
    m41 -= a.m41;
    m42 -= a.m42;
    m43 -= a.m43;
    m44 -= a.m44;
    return *this;
}

inline Matrix operator*(const Matrix& a, const Matrix& b) {

    return Matrix(a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41,
                  a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42,
                  a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43,
                  a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44,
                  a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41,
                  a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42,
                  a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43,
                  a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44,
                  a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41,
                  a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42,
                  a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43,
                  a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44,
                  a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41,
                  a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42,
                  a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43,
                  a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44);
}

inline Matrix& Matrix::operator*=(const Matrix& a) {

    return *this = Matrix(
				m11 * a.m11 + m12 * a.m21 + m13 * a.m31 + m14 * a.m41,
				m11 * a.m12 + m12 * a.m22 + m13 * a.m32 + m14 * a.m42,
				m11 * a.m13 + m12 * a.m23 + m13 * a.m33 + m14 * a.m43,
				m11 * a.m14 + m12 * a.m24 + m13 * a.m34 + m14 * a.m44,
				m21 * a.m11 + m22 * a.m21 + m23 * a.m31 + m24 * a.m41,
				m21 * a.m12 + m22 * a.m22 + m23 * a.m32 + m24 * a.m42,
				m21 * a.m13 + m22 * a.m23 + m23 * a.m33 + m24 * a.m43,
				m21 * a.m14 + m22 * a.m24 + m23 * a.m34 + m24 * a.m44,
				m31 * a.m11 + m32 * a.m21 + m33 * a.m31 + m34 * a.m41,
				m31 * a.m12 + m32 * a.m22 + m33 * a.m32 + m34 * a.m42,
                m31 * a.m13 + m32 * a.m23 + m33 * a.m33 + m34 * a.m43,
                m31 * a.m14 + m32 * a.m24 + m33 * a.m34 + m34 * a.m44,
                m41 * a.m11 + m42 * a.m21 + m43 * a.m31 + m44 * a.m41,
                m41 * a.m12 + m42 * a.m22 + m43 * a.m32 + m44 * a.m42,
                m41 * a.m13 + m42 * a.m23 + m43 * a.m33 + m44 * a.m43,
                m41 * a.m14 + m42 * a.m24 + m43 * a.m34 + m44 * a.m44);
}

inline Vector operator*(const Matrix& a, const Vector& b) {

    return Vector(a.m11 * b.x + a.m12 * b.y + a.m13 * b.z + a.m14,
                  a.m21 * b.x + a.m22 * b.y + a.m23 * b.z + a.m24,
                  a.m31 * b.x + a.m32 * b.y + a.m33 * b.z + a.m34);
}

// scaling transformations
//
inline Matrix& scale(Matrix& m, float x, float y, float z) {

    return m = Matrix(x, 0, 0, 0,
                      0, y, 0, 0,
                      0, 0, z, 0,
                      0, 0, 0, 1);
}

inline Matrix& scale(Matrix& m, const Vector& v) {

    return scale(m, v.x, v.y, v.z);
}

inline Matrix& Matrix::scale(float sx, float sy, float sz) {

    float x = m41, y = m42, z = m43; 
	m41 = m42 = m43 = 0;
    Matrix atOrigin;
	*this *= ::scale(atOrigin, sx, sy, sz);
    m41 = x;
    m42 = y;
    m43 = z;
	return *this;
}

// direction transformations
//
// direction transforms direction v into the reference frame describes
// by the current transformation matrix
//
inline Vector Matrix::direction(const Vector& v) const {

    return v * rotation();
}

// direction returns the orientation of the ? axis in world space
//
inline Vector Matrix::direction(char axis) const {

    Vector v;

    switch(axis) {
        case 'x':
            v = Vector(1, 0, 0);
            break;
        case 'y':
            v = Vector(0, 1, 0);
            break;
        case 'z':
            v = Vector(0, 0, 1);
            break;
    }

    return direction(v);
}

// position transformations
//
// position returns the position stored in the current transformation matrix
//
inline Vector Matrix::position() const {

    return Vector(m41, m42, m43);
}

// position extracts the position vector from transformation m
//
inline Vector position(const Matrix& m) {

    return m.position(); 
}

// rotation transformations
//
// rotation extracts the rotation transformation from a homogeneous
// transformation assuming that there has not been any scaling
//
inline Matrix Matrix::rotation() const {

    return Matrix(m11, m12, m13, 0,
                  m21, m22, m23, 0,
                  m31, m32, m33, 0,
                    0,   0,   0, 1);
}

// rotation extracts the rotation transformation from transformation m 
//
inline Matrix rotation(const Matrix& m) {

    return m.rotation();
}

inline Matrix& Matrix::rotate(const Matrix &rot) {

    float x = m41, y = m42, z = m43; 
	m41 = m42 = m43 = 0;
    *this *= rot;
    m41 = x;
    m42 = y;
    m43 = z;
	return *this;
}

inline Matrix& Matrix::translate(float x, float y, float z) {

	return *this *= Matrix(1, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, 1, 0,
                           x, y, MODEL_Z_AXIS * z, 1);
}

inline Matrix& Matrix::rotatex(float rad) {

    rad    *= MODEL_Z_AXIS;
    float c = cosf(rad);
    float s = sinf(rad);
    Matrix rot(1, 0, 0, 0,
               0, c, s, 0,
               0,-s, c, 0,
               0, 0, 0, 1);
    rotate(rot);
	return *this;
}

inline Matrix& Matrix::rotatey(float rad) {

	rad    *= MODEL_Z_AXIS;
    float c = cosf(rad);
    float s = sinf(rad);
    Matrix rot(c, 0, s, 0,
               0, 1, 0, 0,
              -s, 0, c, 0,
               0, 0, 0, 1);
    rotate(rot);
	return *this;
}

inline Matrix& Matrix::rotatez(float rad) {

    rad    *= MODEL_Z_AXIS;
    float c = cosf(rad);
    float s = sinf(rad);
    Matrix rot(c, s, 0, 0,
              -s, c, 0, 0,
               0, 0, 1, 0,
               0, 0, 0, 1);
    rotate(rot);
	return *this;
}

inline Matrix& Matrix::orient(const Matrix& rot) {

    Matrix atOrigin;
	Matrix sts = *this * this->transpose();
	float sx = sqrtf(sts.m11);
	float sy = sqrtf(sts.m22);
	float sz = sqrtf(sts.m33);
    float x = m41, y = m42, z = m43; 
    isIdentity();
    *this = rot;
	// re-scaling makes the billboard disappear 
    //if (fabs(sx - 1.f) > 0.05f || fabs(sy - 1.f) > 0.05f || fabs(sz - 1.f) > 0.05f)
    //    scale(sx, sy, sz);
    m41 = x;
    m42 = y;
    m43 = z;
	return *this;
}

inline Matrix& Matrix::rotate(const Vector& axis, float rad) {

    rad    *= MODEL_Z_AXIS;
    float c = cosf(rad);
    float s = sinf(rad);
    float t = 1.f - c;
    Vector a = normal(axis);
    Matrix rot(t*a.x*a.x + c,     t*a.x*a.y + a.z*s, t*a.x*a.z - a.y*s, 0,
               t*a.x*a.y - a.z*s, t*a.y*a.y + c,     t*a.y*a.z + a.x*s, 0,
               t*a.x*a.z + a.y*s, t*a.y*a.z - a.x*s, t*a.z*a.z + c,     0,
               0,                 0,                 0,                 1);
    rotate(rot);
    return *this;
}

inline Matrix rotate(const Vector& axis, float rad) {

    rad    *= MODEL_Z_AXIS;
    float c = cosf(rad);
    float s = sinf(rad);
    float t = 1.f - c;
    Vector a = normal(axis);
    return Matrix(t*a.x*a.x + c,     t*a.x*a.y + a.z*s, t*a.x*a.z - a.y*s, 0,
                  t*a.x*a.y - a.z*s, t*a.y*a.y + c,     t*a.y*a.z + a.x*s, 0,
                  t*a.x*a.z + a.y*s, t*a.y*a.z - a.x*s, t*a.z*a.z + c,     0,
                  0,                 0,                 0,                 1);
}

// view returns the view transformation for position p, heading d 
// and up direction u
//
inline Matrix view(const Vector& p, const Vector& d, const Vector& u) {

    Vector z = MODEL_Z_AXIS * normal(d - p);
    Vector x = normal(cross(u, z));
    Vector y = cross(z, x);
    return Matrix(       x.x,        y.x,        z.x,  0,
                         x.y,        y.y,        z.y,  0,
                         x.z,        y.z,        z.z,  0,
                  -dot(x, p), -dot(y, p), -dot(z, p), 1);
}

// projection returns the projection transformation for field of view fov,
// client area aspect ratio aspect, near cutoff plane near_cp and far cutoff
// plane far_cp
//
inline Matrix projection(float fov, float aspect, float near_cp, float far_cp) {

	float sy = 1.0f / tanf(fov * 0.5f);
    float sx = sy / aspect;
	float sz = far_cp / (far_cp-near_cp);
    return Matrix(sx,  0,                 0,            0,
                   0, sy,                 0,            0,
                   0,  0, sz * MODEL_Z_AXIS, MODEL_Z_AXIS,
                   0,  0,     -near_cp * sz,            0);
}

// perspective retruns the projection transformation for an off-center view
// described by the range [minx, maxx], [miny, maxy], [near_cp, far_cp]
//
inline Matrix projection(float minx, float maxx, float miny, float maxy, 
 float near_cp, float far_cp) {

	float sy = 2 * near_cp / (maxy - miny);
    float sx = 2 * near_cp / (maxx - minx);
    float zx = (minx + maxx) / (maxx - minx);
    float zy = (miny + maxy) / (maxy - miny);
	float sz = far_cp / (far_cp-near_cp);
    return Matrix(sx,  0,                 0,            0,
                   0, sy,                 0,            0,
                  zx, zy, sz * MODEL_Z_AXIS, MODEL_Z_AXIS,
                   0,  0,     -near_cp * sz,            0);
}

// projToRhs transforms the lhs projection matrix m into rhs form
//
inline Matrix projToRhs(const Matrix& m) {

    return Matrix(m.m11, m.m12,  m.m13,                                    0,
                  m.m21, m.m22,  m.m23,                                    0,
                  m.m31, m.m32,  m.m33,                -m.m43 * MODEL_Z_AXIS,
                  m.m41, m.m42, -m.m34 * MODEL_Z_AXIS,                     0);
}

// orthographic returns the orthographic transformation
//
inline Matrix orthographic(int w, int h) {

    return Matrix(2.f/w,     0,  0, 0,
                      0, 2.f/h,  0, 0,
                      0,     0, -1, 0,
                     -1,    -1,  0, 1);
}

//function return projection of a onto b
//((a dot b)/(b dot b))*b
inline Vector projectOnTo(const Vector& a,const Vector& b){
	return Vector( (dot(a,b)/dot(b,b))*b);
}
inline Matrix orthoNormalize(const Matrix& m){
	Vector row1 =normal(Vector(m.m11,m.m12,m.m13));				//fix the vector in row 1 to unit length
	Vector pr = projectOnTo(row1,Vector(m.m21,m.m22,m.m23));	

	Vector row2 = normal(Vector(m.m21,m.m22,m.m23)-pr);	
	
	Vector row3 = normal(cross(row1,row2));
	row2=cross(row3,row1);
	return Matrix(row1.x,row1.y,row1.z,m.m14,
				row2.x,row2.y,row2.z,m.m24,
				row3.x,row3.y,row3.z,m.m34,
			    m.m41,m.m42,m.m43,m.m44);

}
inline Vector Vector::operator*(const Vector& v){
    return Vector(x*v.x, y*v.y, z*v.z);
}

#endif
