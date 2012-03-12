#ifndef _I_COLLISIONGEOMETRY_H_
#define _I_COLLISIONGEOMETRY_H_

/* Collision Geometry Interface - Physics Scene Component - Model Branch
 *
 * iCollisionGeometry.h
 * November 18 2010
 */

#include "MathDeclarations.h"
#include "MathDefinitions.h"
#include <vector>

//--------------------------- iCollisionGeometry ------------------------------
//
// Represents a collision body that can collide with other collision bodies.
//
class iPhysics;

namespace CT {
    const unsigned int SPHERE =      1;
    const unsigned int OBB =         2;
}

class iCollisionGeometry {
public:
    // functions that should be overridden by subclasses
    // initialization functions
    virtual iCollisionGeometry* clone() const               = 0;
    // execution functions
    virtual void getProjectionBounds(const Vector& axis, 
        float& min, 
        float& max) const                                   = 0;
    virtual const void* getCollisionStruct() const          = 0;
    virtual unsigned int getCollisionType() const           = 0;
    virtual float getBoundingRadius() const                 = 0;
    
    // functions that do NOT need to be overridden by subclasses
    // initialization functions
    virtual void attach(iPhysics* o)                      = 0;
    // execution functions
    virtual iPhysics* getPhysics()                             = 0;
    virtual const iPhysics* getPhysics() const                 = 0;
    // termination functions
    virtual void suspend() const                            = 0;
    virtual void release() const                            = 0;
    virtual void Delete() const                             = 0;
};

class iCBox : public iCollisionGeometry {};
#endif