#ifndef _COBB_H_
#define _COBB_H_

/* Collision OBB Header - Physics Scene Component - Model Branch
 *
 * COBB.h
 * December 4 2010
 */

#ifndef NULL
#define NULL 0
#endif

#include <vector>
#include "CollisionGeometry.h"
#include "iCOBB.h"
#include "MathDeclarations.h"

//---------------------------- COBB -------------------------------------------
//
// Represents an oriented bounding box collision body.
//
class COBB : public CollisionGeometry {
    COBBCollision       _cinfo;
    std::vector<Vector> _cVerts;
    float               _boundingRadius;

protected:
    COBB();
    // Does this need to be virtual even though parent is virtual...?
    virtual ~COBB();

public:
    friend iCollisionGeometry* CreateCOBB(
        iPhysics* rb,
        const Vector& radius);
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