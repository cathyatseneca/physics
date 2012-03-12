#ifndef _COLLISIONGEOMETRY_H_
#define _COLLISIONGEOMETRY_H_

/* Collision Geometry Header - Physics Scene Component - Model Branch
 *
 * CollisionGeometry.h
 * November 25 2010
 */

#include "iCollisionGeometry.h"
#include "iPhysics.h"
#include "MathDeclarations.h"
#include "MathDefinitions.h"
#include <vector>

//--------------------------- CollisionGeometry -------------------------------
//
// Represents a collision body that can collide with other collision bodies.
//

class iCollisionSpace;

class CollisionGeometry : public iCollisionGeometry {
    iPhysics* _rb;

    static iCollisionSpace* _collisionSpace;

protected:
    CollisionGeometry();
    virtual ~CollisionGeometry();

public:
    static void setGlobalCollisionSpace(iCollisionSpace* cs) { _collisionSpace = cs; }
    // initialization functions
    void attach(iPhysics* o);
    // execution functions
    iPhysics* getPhysics();
    const iPhysics* getPhysics() const;
    // termination functions
    virtual void suspend() const;
    virtual void release() const;
    virtual void Delete() const;
};
#endif