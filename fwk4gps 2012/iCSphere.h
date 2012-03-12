#ifndef _I_CSPHERE_H_
#define _I_CSPHERE_H_

/* Collision Sphere Interface - Physics Scene Component - Model Branch
 *
 * iCSphere.h
 * November 25 2010
 */

#ifndef NULL
#define NULL 0
#endif

//--------------------------- iCSphere ----------------------------------------
//
// Represents a spherical collision body.
//
class iCollisionGeometry;
class iRigidBody;

struct CSphereCollision {
    float radius;
};

extern "C"
iCollisionGeometry* CreateCSphere(iRigidBody* rb=NULL, float radius=10.0f);
#endif