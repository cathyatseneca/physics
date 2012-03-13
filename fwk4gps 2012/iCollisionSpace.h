#ifndef _I_COLLISIONSPACE_H_
#define _I_COLLISIONSPACE_H_

/* Collision Space Interface - Physics Scene Component - Model Branch
 *
 * iCollisionSpace.h
 * November 17 2010
 */

#include "MathDeclarations.h"
#include <list>

//--------------------------- iCollisionSpace ---------------------------------
//
// iCollisionSpace represents a space of objects that may or may not collide
// with each other. Collision spaces may also eliminate unnecessary collisions.
//

class iCollisionSpace;
class iCollisionGeometry;

struct CollisionContact{
    Vector pos;         // contact position
    Vector normal;      // normal vector
    float depth;        // penetration depth
    iCollisionGeometry *g1, *g2; // colliding geoms
};

class iCollisionSpace {
public:
    // execution functions
    virtual bool add(iCollisionGeometry* o)             = 0;
    virtual void remove(iCollisionGeometry* o)          = 0;
    virtual const std::list<iCollisionGeometry*>& getAttachedGeometry() = 0;
    virtual void populateContactList(float delta)       = 0;
    virtual const CollisionContact* getContactList(int idx) const  = 0;
    virtual size_t getNumContacts() const               = 0;
    // termination functions
    virtual void suspend() const                        = 0;
    virtual void release() const                        = 0;
    virtual void Delete() const                         = 0;
};
#endif