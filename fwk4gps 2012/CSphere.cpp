/* Collision Sphere Implementation - Physics Scene Component - Model Branch
 *
 * CSphere.cpp
 * November 25 2010
 */

#include "CSphere.h"
#include "COBB.h"
#include "iPhysics.h"

/**
 * Basic ctor.
 */
CSphere::CSphere() : _cinfo(){
    _cinfo.radius = 0;
}

/**
 * Basic dtor.
 */
CSphere::~CSphere(){
}

/**
 * CreateCSphere
 *
 * Creates a new collision sphere.
 */
iCollisionGeometry* CreateCSphere(iPhysics* rb, float radius){
    CSphere* sphere = new CSphere();
    sphere->_cinfo.radius = radius;
    sphere->_boundingRadius = radius;
    sphere->attach(rb);
    return sphere;
}

/**
 * clone
 *
 * Returns a clone of this object and the attached rb.
 */
iCollisionGeometry* CSphere::clone() const{
    // This is ugly but...
    iPhysics* rb = const_cast<iPhysics*>(getPhysics());
    if (rb) { rb = rb->clone(); }
    return CreateCSphere(rb, _cinfo.radius);
}

/**
 * getProjectionBounds
 *
 * Projects this sphere on a given axis.
 */
void CSphere::getProjectionBounds(const Vector& axis,
    float& min,
    float& max) const{

    // For any of this to work, we need to have an associated rb
    const iPhysics* rb = getPhysics();
    if (rb) {
        // Project our sphere onto our axis
        const Vector& pos = rb->worldCentreOfMass();
        float mid = dot(axis, pos) / dot(axis, axis);
        min = mid - _cinfo.radius;
        max = mid + _cinfo.radius;
    }
}

/**
 * getBoundingRadius
 *
 * Returns the radius.
 */
float CSphere::getBoundingRadius() const {
    return _boundingRadius;
}

/**
 * getCollisionInformation
 * 
 * Returns the locally contained CSphereCollision struct.
 */
const void* CSphere::getCollisionStruct() const {
    return &_cinfo;
}

/**
 * getCollisionType
 *
 * Sphere collision geometry.
 */
unsigned int CSphere::getCollisionType() const{
    return CT::SPHERE;
}
