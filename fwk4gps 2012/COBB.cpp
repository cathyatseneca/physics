/* Collision OBB ImplwmwnrRION - Physics Scene Component - Model Branch
 *
 * COBB.cpp
 * December 4 2010
 */

#include "COBB.h"
#include "iCollisionGeometry.h"
#include "iPhysics.h"
#include "MathDeclarations.h"
#include <cmath>

//---------------------------- COBB -------------------------------------------
//
// Represents an oriented bounding box collision body.
//

/**
 * Basic ctor.
 */
COBB::COBB() : _cinfo(), _cVerts(){
    _cinfo.radius = Vector();
}

/**
 * Basic dtor.
 */
COBB::~COBB(){
}

/**
 * CreateCSphere
 *
 * Creates a new collision sphere.
 */
iCollisionGeometry* CreateCOBB(
        iPhysics* rb,
        const Vector& radius){
    COBB* cobb = new COBB();
    cobb->_cinfo.radius = radius;
    COBBCollision& cinfo = cobb->_cinfo;

    // Populate vertex cache
    std::vector<Vector>& cverts = cobb->_cVerts;
cverts.push_back(Vector(cinfo.radius.x, cinfo.radius.y, cinfo.radius.z));
cverts.push_back(Vector(cinfo.radius.x, cinfo.radius.y, -cinfo.radius.z));
cverts.push_back(Vector(cinfo.radius.x, -cinfo.radius.y, cinfo.radius.z));
cverts.push_back(Vector(cinfo.radius.x, -cinfo.radius.y, -cinfo.radius.z));
cverts.push_back(Vector(-cinfo.radius.x, cinfo.radius.y, cinfo.radius.z));
cverts.push_back(Vector(-cinfo.radius.x, cinfo.radius.y, -cinfo.radius.z));
cverts.push_back(Vector(-cinfo.radius.x, -cinfo.radius.y, cinfo.radius.z));
cverts.push_back(Vector(-cinfo.radius.x, -cinfo.radius.y, -cinfo.radius.z));
    cobb->attach(rb);

    float md = std::max(std::max(radius.x, radius.y), radius.z);
    cobb->_boundingRadius = std::sqrtf(3.0f * (md*md));

    return cobb;
}

/**
 * clone
 *
 * Returns a clone of this object and the attached rb.
 */
iCollisionGeometry* COBB::clone() const{
    // This is ugly but...
    iPhysics* rb = const_cast<iPhysics*>(getPhysics());
    if (rb) { rb = rb->clone(); }
    return CreateCOBB(rb, _cinfo.radius);
}

/**
 * getProjectionBounds
 *
 * Projects this OBB on a given axis.
 */
void COBB::getProjectionBounds(const Vector& axis,
    float& min,
    float& max) const{

    // For any of this to work, we need to have an associated rb
    const iPhysics* rb = getPhysics();
    if (rb) {
        // Use vertex cache, if possible
        std::vector<Vector> locCVerts;
        const std::vector<Vector>* cVerts = NULL;
        if (_cVerts.size() == 8) {
            cVerts = &_cVerts;
        } else {
            cVerts = &locCVerts;

            // Fill temporary vertex cache
locCVerts.push_back(Vector(_cinfo.radius.x, _cinfo.radius.y, _cinfo.radius.z));
locCVerts.push_back(Vector(_cinfo.radius.x, _cinfo.radius.y, -_cinfo.radius.z));
locCVerts.push_back(Vector(_cinfo.radius.x, -_cinfo.radius.y, _cinfo.radius.z));
locCVerts.push_back(Vector(_cinfo.radius.x, -_cinfo.radius.y, -_cinfo.radius.z));
locCVerts.push_back(Vector(-_cinfo.radius.x, _cinfo.radius.y, _cinfo.radius.z));
locCVerts.push_back(Vector(-_cinfo.radius.x, _cinfo.radius.y, -_cinfo.radius.z));
locCVerts.push_back(Vector(-_cinfo.radius.x, -_cinfo.radius.y, _cinfo.radius.z));
locCVerts.push_back(Vector(-_cinfo.radius.x, -_cinfo.radius.y, -_cinfo.radius.z));
        }

        // Project every vertex in sequence on this axis
        const Vector& pos = rb->worldCentreOfMass();
        const Matrix& rot = rb->rotation();
        for (int i = 0; i < 8; i++) {
            float proj = dot(axis, ((*cVerts)[i] * rot) + pos) / dot(axis, axis);
            if (i == 0) {
                min = proj;
                max = proj;
            } else {
                if (proj < min) min = proj;
                else if (proj > max) max = proj;
            }
        }
    }
}

/**
 * getBoundingRadius
 *
 * Returns the radius.
 */
float COBB::getBoundingRadius() const {
    return _boundingRadius;
}

/**
 * getCollisionInformation
 * 
 * Returns the locally contained COBBCollision struct.
 */
const void* COBB::getCollisionStruct() const {
    return &_cinfo;
}

/**
 * getCollisionType
 *
 * Sphere collision geometry.
 */
unsigned int COBB::getCollisionType() const{
    return CT::OBB;
}
