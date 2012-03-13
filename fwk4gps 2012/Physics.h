#ifndef PHYSICS_H
#define PHYSICS_H
#include "iPhysics.h"
#include "Frame.h"
#include "MathDeclarations.h"
#include "iObject.h"
#include "Object.h"

class iCollisionGeometry;
class Physics:public iPhysics{
	iObject* bv_;
	Vector centreOfMass_;   //centre of mass of object relative to the centroid
							//when an object is first made a physics object this
							//value is a 0 vector.  If objects get attached
	iPhysics* parent_;

	Vector velocity_;       //velocity of this object relative to it's bv's parent
	Vector acceleration_;   
	Vector angVel_;
	Vector angAccel_;
	float mass_;           //mass of object in Kilograms
	Vector bodyForce_;     //a force that is constantly applied to the object
	Vector impulseForce_;  //a force that is applied for a brief moment in time
	Vector moment_;			//an angular force applied to the body
	Matrix momentInertia_;	//the mass moment of intertia along the body's local x,y,z axis
	Matrix invMomentInertia_; //the inverse of the mass moment of inertia matrix.

	PhysicsType physicsType_;
	iCollisionGeometry* cg_;

	void	setVelocity(Vector v)					{velocity_=v;}
	void	setAngularVelocity(Vector v)			{angVel_=v;}
	void	setAngularAcceleration(Vector v)		{angAccel_=v;}
	void	setAcceleration(Vector v)				{acceleration_=v;}

public:
    Physics(float mass, PhysicsType pt, iObject* bv);
	~Physics();

	void   scale(float x, float y, float z){bv_->scale(x,y,z);}
	void   rotatex(float rad){bv_->rotatex(rad);}
	void   rotatey(float rad){bv_->rotatey(rad);}
	void   rotatez(float rad){bv_->rotatez(rad);}
	Vector position() const{return bv_->position();}
	Matrix rotation() const{return bv_->rotation();}
	void rotate(const Matrix& M){bv_->rotate(M);}
	Vector orientation(char axis) const{return bv_->orientation(axis);}
	Matrix world() const{return bv_->world();}
	iObject* bv() const{return bv_;}
	void   rotate(const Vector& axis, float rad) { bv_->rotate(axis,rad);}
	void    translate(float x, float y, float z) {bv_->translate(x,y,z);}
	void addimpulseForce(Vector v){impulseForce_+=v;}
	float mass() const{return mass_;}

	Vector angularVelocity() const {return parent_?angVel_+parent_->angularVelocity():angVel_;}
	Vector velocity() const {return  parent_?parent_->velocity()+velocity_:velocity_;}
	Vector  acceleration() const {return acceleration_;}
	Vector angularAcceleration() const;
    Vector tanVelocityReference() const;
	Vector tanVelocity() const;
    Vector tanAccelerationReference() const	;
    Vector tanAcceleration() const;
	Vector worldCentreOfMass() const;
	Vector velocityAtPoint(const Vector& p) const;

	void setMomentInertia(const Vector& i);
	void addMoment(const Vector& m) { moment_ += m; }	
	void attachTo(iPhysics* newParent, float speed, float angularSpeed);
	void update(int delta);
	void updateKinematics(float delta);
	iPhysics* clone();
	PhysicsType physicsType() const{return physicsType_;}
	void setCollision(iCollisionGeometry* cg){cg_=cg;}
	iCollisionGeometry* collisionGeometry(){return cg_;}
	friend iPhysics* CreatePhysicsBox(float minx, float miny, float minz, 
	 float maxx, float maxy, float maxz, const Reflectivity* r, float mass,PhysicsType pt,bool hasCollision);
};
//extern "C"
//iPhysics* CreatePhysicsBox(float minx, float miny, float minz, 
//	 float maxx, float maxy, float maxz, Colour c, float mass, bool hasCollision=false);
#endif