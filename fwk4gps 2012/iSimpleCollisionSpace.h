#ifndef _I_SIMPLECOLLISIONSPACE_H_
#define _I_SIMPLECOLLISIONSPACE_H_

/* Simple Collision Space Interface - Physics Scene Component - Model Branch
 *
 * iSimpleCollisionSpace.h
 * November 17 2010
 */

//--------------------------- iSimpleCollisionSpace ---------------------------
//
// A simple collision space. Performance is N^2 rigid bodies.
//

class iCollisionSpace;

extern "C"
iCollisionSpace* CreateSimpleCollisionSpace(size_t samplesPerSecond=60);
#endif
