#ifndef _CSPHERE_H_
#define _CSPHERE_H_

/* Collision Sphere Header - Physics Scene Component - Model Branch
 *
 * CSphere.h
 * November 25 2010
 */

#include "iCSphere.h"
#include "CollisionGeometry.h"

//--------------------------- CSphere -----------------------------------------
//
// Represents a spherical collision body.
//
class CSphere : public CollisionGeometry {
    CSphereCollision    _cinfo;
    float               _boundingRadius;

protected:
    CSphere();
    // Does this need to be virtual even though parent is virtual...?
    virtual ~CSphere();

public:
    friend iCollisionGeometry* CreateCSphere(iPhysics* rb, float radius);
    // initialization functions
    iCollisionGeometry* clone() const;
    // execution functions
    void getProjectionBounds(const Vector& axis, 
        float& min, 
        float& max) const;
    float getBoundingRadius() const;
    const void* getCollisionStruct() const;
    unsigned int getCollisionType() const;
};
#endif