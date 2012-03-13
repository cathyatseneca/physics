#ifndef _SIMPLECOLLISIONSPACE_H_
#define _SIMPLECOLLISIONSPACE_H_

/* Simple Collision Space Header - Physics Scene Component - Model Branch
 *
 * SimpleCollisionSpace.h
 * November 17 2010
 */

//--------------------------- SimpleCollisionSpace ----------------------------
//
// A simple collision space. Performance is N^2 rigid bodies.
//

#include "iCollisionSpace.h"
#include <vector>
#include <list>

class iCSphere;
class iCBox;

class SimpleCollisionSpace : public iCollisionSpace {

    std::list<iCollisionGeometry*> _geomList;
    std::vector<CollisionContact> _contactList;

    size_t _samplesPerSecond;

    // Deny users native construction
    SimpleCollisionSpace();

    bool _collide(
        iCollisionGeometry* g1, 
        iCollisionGeometry* g2, 
        CollisionContact* contact
    ) const;

public:
    friend iCollisionSpace* CreateSimpleCollisionSpace(size_t samplesPerSecond);
    virtual ~SimpleCollisionSpace();
    // execution functions
    bool add(iCollisionGeometry* o);
    void remove(iCollisionGeometry* o);
    const std::list<iCollisionGeometry*>& getAttachedGeometry();
    void populateContactList(float delta);
    const CollisionContact* getContactList(int idx=0) const;
    size_t getNumContacts() const;
    // termination functions
    void suspend() const;
    void release() const;
    void Delete() const;
};
#endif
