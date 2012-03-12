#ifndef _I_COBB_H_
#define _I_COBB_H_

/* Collision OBB Interface - Physics Scene Component - Model Branch
 *
 * iCOBB.h
 * December 4 2010
 */

#ifndef NULL
#define NULL 0
#endif

#include "MathDeclarations.h"

//--------------------------- iCOBB -------------------------------------------
//
// Represents an oriented bounding box collision body.
//
class iCollisionGeometry;
class iPhysics;

struct COBBCollision {
    Vector radius;
};

extern "C"
iCollisionGeometry* CreateCOBB(
    iPhysics* rb=NULL, 
    const Vector& radius=Vector(10, 10, 10)
);
#endif