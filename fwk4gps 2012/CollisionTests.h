#ifndef _COLLISIONTESTS_H_
#define _COLLISIONTESTS_H_

/* Collision Tests Header - Physics Scene Component - Model Branch
 *
 * CollisionTests.h
 * December 4 2010
 */

//--------------------------- CollisionTests ----------------------------------
//
// A suite of functions for testing collision geometry against other collision
// geometry.
//
class iCollisionGeometry;
struct CollisionContact;
struct Vector;

void ClosestPointOBB(Vector p, const iCollisionGeometry* obb, Vector& q);
bool CSphereBox(iCollisionGeometry* s, iCollisionGeometry* b, CollisionContact* contact);
bool CSphereSphere(iCollisionGeometry* s1, iCollisionGeometry* s2, CollisionContact* contact);
bool CBoxBox(iCollisionGeometry* b1, iCollisionGeometry* b2, CollisionContact* contact);

#endif