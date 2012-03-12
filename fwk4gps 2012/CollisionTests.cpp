/* Collision Tests Implementation - Physics Scene Component - Model Branch
 *
 * CollisionTests.cpp
 * December 4 2010
 */

//--------------------------- CollisionTests ----------------------------------
//
// A suite of functions for testing collision geometry against other collision
// geometry.
//

#include "ModelSettings.h"
#include "CollisionTests.h"
#include "iCollisionSpace.h"
#include "iCollisionGeometry.h"
#include "iCSphere.h"
#include "iCOBB.h"
#include "iPhysics.h"
#include "MathDeclarations.h"
#include <cmath>

#define WITHIN_TOLERANCE(f1, f2) (std::abs((f2) - (f1)) <= MAX_COLLISION_TOLERANCE)

/**
 * Performs a basic check on collision bounds. Returns true if a collision is
 * possible.
 */
inline bool BoundsCollide(
    const iCollisionGeometry* g1, 
    const iCollisionGeometry* g2,
    float distSquared
){
    float maxDistSquared = g1->getBoundingRadius() + g2->getBoundingRadius();
    maxDistSquared *= maxDistSquared;
    return distSquared <= maxDistSquared;
}

/**
 * ClosestPointOBB
 *
 * Given an arbitrary point in world space and an obb, finds the closest
 * feature point on that obb to that point and stores it in q in world space.
 */
void ClosestPointOBB(
    Vector p, 
    const iCollisionGeometry* obb, 
    Vector& q
){
    const COBBCollision* obbcol = 
        (const COBBCollision*)(obb->getCollisionStruct());
    const Vector& obbpos = obb->getPhysics()->worldCentreOfMass();
    const Matrix& obbrot = obb->getPhysics()->bv()->rotation();

    // Generate coordinate axes
    Vector ux(1, 0, 0), uy(0, 1, 0), uz(0, 0, 1);
    ux *= obbrot; uy *= obbrot; uz *= obbrot;

    // Make center of obb the origin
    p -= obbpos; 

    // Project point onto axes
    Vector pp(dot(ux, p), dot(uy, p), dot(uz, p));

#define CLAMP(v, min, max) if ((v) < (min)) (v) = (min); \
    else if ((v) > (max)) (v) = (max)

    // Find closest point
    CLAMP(pp.x, -obbcol->radius.x, obbcol->radius.x);
    CLAMP(pp.y, -obbcol->radius.y, obbcol->radius.y);
    CLAMP(pp.z, -obbcol->radius.z, obbcol->radius.z);

    // Put point back into world coordinate
    q = obbpos + (pp * obbrot);

#undef CLAMP
}

/**
 * CSphereBox
 *
 * Relatively simple sphere vs box collision test.
 * Normal points at box.
 */
bool CSphereBox(
    iCollisionGeometry* s, 
    iCollisionGeometry* b, 
    CollisionContact* contact
){
    const CSphereCollision* scol =
        (const CSphereCollision*)(s->getCollisionStruct());
    const COBBCollision* obbcol = 
        (const COBBCollision*)(b->getCollisionStruct());
    const iPhysics* srbd = s->getPhysics();
    const iPhysics* brbd = b->getPhysics();
    const Vector& spos = srbd->worldCentreOfMass();
    const Vector& bpos = brbd->worldCentreOfMass();
    const Matrix& brot = brbd->rotation();

    // First and foremost, test bounds
    Vector diff = spos - bpos;
    bool bColliding = BoundsCollide(s, b, dot(diff, diff));

    // Perform tests if collision is possible
    if (bColliding) {

        // ########################### SAT Test ###################################
        // Perform SAT test on Box vs. Sphere

        // Box has 23 separating axes, attempt to separate by them
        const Vector bRotRad = obbcol->radius * brot;
#define VECTOR_MACRO(fpx, fpy, fpz) \
    normal(spos - (((Vector(fpx, fpy, fpz) * bRotRad) + bpos)))

        const Vector box_axes[23] = {

            // Add face axes
            Vector(1, 0, 0) * brot,
            Vector(0, 1, 0) * brot,
            Vector(0, 0, 1) * brot,

            // Add edge axes
            VECTOR_MACRO(1, 0, 1),
            VECTOR_MACRO(1, 0, -1),
            VECTOR_MACRO(-1, 0, 1),
            VECTOR_MACRO(-1, 0, -1),
            VECTOR_MACRO(0, 1, 1),
            VECTOR_MACRO(0, 1, -1),
            VECTOR_MACRO(0, -1, 1),
            VECTOR_MACRO(0, -1, -1),
            VECTOR_MACRO(1, 1, 0),
            VECTOR_MACRO(1, -1, 0), 
            VECTOR_MACRO(-1, 1, 0),
            VECTOR_MACRO(-1, -1, 0),

            // Add vertex axes
            VECTOR_MACRO(1, 1, 1),
            VECTOR_MACRO(1, 1, -1),
            VECTOR_MACRO(1, -1, 1),
            VECTOR_MACRO(1, -1, -1),
            VECTOR_MACRO(-1, 1, 1),
            VECTOR_MACRO(-1, 1, -1),
            VECTOR_MACRO(-1, -1, 1),
            VECTOR_MACRO(-1, -1, -1),
        };
#undef VECTOR_MACRO

        // Perform projection tests on box vs sphere
        float bMin, bMax, sMin, sMax;
        float tempDepth = 0xFFFF;
        float depth = 0;
        const Vector* minAxis = NULL;
        for (size_t i = 0; bColliding && i < 23; i++) {
            b->getProjectionBounds(box_axes[i], bMin, bMax);
            s->getProjectionBounds(box_axes[i], sMin, sMax);

            // Test for separating plane
            if (bMax <= sMin || sMax <= bMin) {
                // We have separation
                bColliding = false;

            // Keep track of penetration depth for later analysis
            } else if (bMax > sMin && sMax > bMax) {
                // Penetration: Box -> Sphere
                tempDepth = bMax - sMin;
            } else {
                // Penetration: Sphere -> Box (sMax > bMin && bMax > sMax)
                tempDepth = sMax - bMin;
            }

            // Update minimum axis
            if ((bColliding && i == 0) || tempDepth < depth) {
                depth = tempDepth;
                minAxis = &(box_axes[i]);
            }
        }

        // If no separating plane found and penetration depth is within collision
        // tolerance, test velocities
        if (bColliding) {
            float penDist = scol->radius - depth;

            // Calculate normal
            Vector n = *minAxis;

            // If normal is not facing box, make it face box
            if (dot(n, bpos - spos) < 0) {
                n *= -1;
            }

            Vector contactPoint = spos + penDist * n;

            // If collision depth is within tolerance then test velocities
            if (depth <= MAX_COLLISION_TOLERANCE) {
                Vector rVelocity = srbd->velocityAtPoint(contactPoint) -
                    brbd->velocityAtPoint(contactPoint);
                bColliding = dot(rVelocity, spos - bpos) > 0;
            }

            // If still colliding, update contact
            if (bColliding) {
                // We have a collision! This fails for spos == bpos
                // Update contact
                contact->g1 = s;
                contact->g2 = b;

                // Update normal
                contact->normal = n;

                // Calculate depth
                // Optimize later
                contact->depth = depth;
                contact->pos = contactPoint;
            }
        }
    }

    return bColliding;
#undef EQ_VECT
}

/**
 * CSphereSphere
 *
 * Relatively simple sphere vs sphere collision test.
 * Normal points at s2.
 */
bool CSphereSphere(
    iCollisionGeometry* s1, 
    iCollisionGeometry* s2, 
    CollisionContact* contact
){
    bool bColliding = true;

    if (!(s1 && s1->getPhysics() && s2 && s2->getPhysics() && contact)) {
        bColliding = false;
    } else {
        const iPhysics* s1rbd = s1->getPhysics();
        const iPhysics* s2rbd = s2->getPhysics();

		Vector s1com = s1rbd->worldCentreOfMass();
		Vector s2com = s2rbd->worldCentreOfMass();

        const CSphereCollision* s1col = (const CSphereCollision*)
            (s1->getCollisionStruct());
        const CSphereCollision* s2col = (const CSphereCollision*)
            (s2->getCollisionStruct());

        Vector dir = s2com - s1com;
        float distSq = dot(dir, dir);
        float maxColDistSq = s1col->radius + s2col->radius;
        maxColDistSq *= maxColDistSq;

        // Do we have a collision?
        if (distSq > maxColDistSq) bColliding = false;
        else {
            // Ensure that bodies are not simply resting
            float depth = sqrt(maxColDistSq) - sqrt(distSq);
            Vector n = normal(dir);
            Vector contactPoint = s1com + 
                    ((s1col->radius - depth) * n);

            if (depth <= MAX_COLLISION_TOLERANCE) {
				Vector rVelocity = s1rbd->velocityAtPoint(contactPoint) -
                    s2rbd->velocityAtPoint(contactPoint);
                bColliding = dot(rVelocity, -n) > 0;
            }

            if (bColliding) {
                // Update contact
                contact->g1 = s1;
                contact->g2 = s2;

                contact->depth = depth;

                // For now we will negate the normal, seems to be required...
                contact->normal = n;
                contact->pos = contactPoint;
            }
        }
    }

    return bColliding;
}

/**
 * CBoxBox
 * Simple Box vs Box SAT collision tests.
 * Normal points at b2.
 */
bool CBoxBox(
    iCollisionGeometry* b1, 
    iCollisionGeometry* b2, 
    CollisionContact* contact
){
    const COBBCollision* b1col =
        (const COBBCollision*)(b1->getCollisionStruct());
    const COBBCollision* b2col = 
        (const COBBCollision*)(b2->getCollisionStruct());
    const iPhysics* b1rbd = b1->getPhysics();
    const iPhysics* b2rbd = b2->getPhysics();
    const Vector& b1pos = b1rbd->worldCentreOfMass();
    const Vector& b2pos = b2rbd->worldCentreOfMass();
    const Matrix& b1rot = b1rbd->rotation();
    const Matrix& b2rot = b2rbd->rotation();

    // First and foremost, test bounds
    Vector diff = b1pos - b2pos;
    bool bColliding = BoundsCollide(b1, b2, dot(diff, diff));

    // Perform tests if collision is possible
    if (bColliding) {

        // ########################### SAT Test ###################################
        // Added axes from both boxes then test

        // Box has 23 separating axes, attempt to separate by them
#define VECTOR_MACRO(orad,op,tp,fpx, fpy, fpz) \
        (tp - (((Vector(fpx, fpy, fpz) * orad) + op))).normalize()

        const Vector b1RotRad = b1col->radius * b1rot;
        const Vector b2RotRad = b2col->radius * b2rot;

        const Vector box_axes[15] = {

            // Add box 1 face axes
            Vector(1, 0, 0) * b1rot,
            Vector(0, 1, 0) * b1rot,
            Vector(0, 0, 1) * b1rot,

            // Add box 2 face axes
            Vector(1, 0, 0) * b2rot,
            Vector(0, 1, 0) * b2rot,
            Vector(0, 0, 1) * b2rot,

            // Add edge cross products
            cross(Vector(1, 0, 0) * b1RotRad, Vector(1, 0, 0) * b2RotRad),
            cross(Vector(1, 0, 0) * b1RotRad, Vector(0, 1, 0) * b2RotRad),
            cross(Vector(1, 0, 0) * b1RotRad, Vector(0, 0, 1) * b2RotRad),

            cross(Vector(0, 1, 0) * b1RotRad, Vector(1, 0, 0) * b2RotRad),
            cross(Vector(0, 1, 0) * b1RotRad, Vector(0, 1, 0) * b2RotRad),
            cross(Vector(0, 1, 0) * b1RotRad, Vector(0, 0, 1) * b2RotRad),

            cross(Vector(0, 0, 1) * b1RotRad, Vector(1, 0, 0) * b2RotRad),
            cross(Vector(0, 0, 1) * b1RotRad, Vector(0, 1, 0) * b2RotRad),
            cross(Vector(0, 0, 1) * b1RotRad, Vector(0, 0, 1) * b2RotRad),
        };
#undef VECTOR_MACRO

        // Perform projection tests on box vs sphere
        float b1Min, b1Max, b2Min, b2Max;
        float tempDepth = 0xFFFF;
        float depth = 0;
        float tempContactDist = 0;
        float contactDist = 0;
        const Vector* minAxis = NULL;
        for (size_t i = 0; bColliding && i < 15; i++) {
            b1->getProjectionBounds(box_axes[i], b1Min, b1Max);
            b2->getProjectionBounds(box_axes[i], b2Min, b2Max);

            // Test for separating plane
            if (b2Max <= b1Min || b1Max <= b2Min) {
                // We have separation
                bColliding = false;

            // Keep track of penetration depth for later analysis
            } else if (b2Max > b1Min && b1Max > b2Max) {
                // Penetration: Box 2 -> Box 1
                tempDepth = b2Max - b1Min;

                // Find contact distance from centroid of box 1
                float b1Centroid = (b1Min + b1Max) / 2.0f;
                tempContactDist = std::abs(b2Max - b1Centroid);
            } else {
                // Penetration: Box 1 -> Box 2 (b1Max > b2Min && b2Max > b1Max)
                tempDepth = b1Max - b2Min;

                // Find contact distance from centroid of box 1
                float b1Centroid = (b1Min + b1Max) / 2.0f;
                tempContactDist = std::abs(b2Min - b1Centroid);
            }

            // Update minimum axis
            if ((bColliding && i == 0) || tempDepth < depth) {
                depth = tempDepth;
                contactDist = tempContactDist;
                minAxis = &(box_axes[i]);
            }
        }

        // If no separating plane found and penetration depth is within collision
        // tolerance, test velocities
        if (bColliding) {
        
            // Calculate normal
            Vector n = *minAxis;

            // If normal is not facing box 2, make it face box 2
            if (dot(n, b2pos - b1pos) < 0) {
                n *= -1;
            }

            Vector contactPoint = b1pos + contactDist * n;

            // If collision depth is within tolerance then test velocities
            if (depth <= MAX_COLLISION_TOLERANCE) {
				Vector rVelocity = b1rbd->velocityAtPoint(contactPoint) -
					b2rbd->velocityAtPoint(contactPoint);
                bColliding = dot(rVelocity, b1pos - b2pos) > 0;
            }

            // If still colliding, update contact
            if (bColliding) {
                // We have a collision! This fails for spos == bpos
                // Update contact
                contact->g1 = b1;
                contact->g2 = b2;

                // Update normal
                contact->normal = n;

                // Calculate depth
                // Optimize later
                contact->depth = depth;
                contact->pos = contactPoint;
            }
        }
    }

    return bColliding;
#undef EQ_VECT
}
