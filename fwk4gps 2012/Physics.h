#ifndef PHYSICS_H
#define PHYSICS_H
#include "iPhysics.h"
#include "Frame.h"
#include "MathDeclarations.h"
#include "iObject.h"
#include "Object.h"

class Physics:public iPhysics{
	iObject* bv_;
	iPhysics* parent_;

	Vector velocity_;       //velocity of this object relative to it's bv's parent
	Vector acceleration_;   

	Vector angVel_;
	Vector angAccel_;
	void	setVelocity(Vector v)					{velocity_=v;}
	void	setAcceleration(Vector v)				{acceleration_=v;}
	void	setAngularVelocity(Vector v)			{angVel_=v;}
	void	setAngularAcceleration(Vector v)		{angAccel_=v;}

public:
	Physics(iObject* bv);
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
	Vector velocity() const {return  parent_?parent_->velocity()+velocity_:velocity_;}
	Vector  acceleration() const {return acceleration_;}

	Vector angularVelocity() const {return parent_?angVel_+parent_->angularVelocity():angVel_;}
	Vector angularAcceleration() const;

	void attachTo(iPhysics* newParent, float speed, float angularSpeed);
	void update(int delta);
	iPhysics* clone();

	friend iPhysics* CreatePhysicsBox(float minx, float miny, float minz, 
	 float maxx, float maxy, float maxz, bool hasCollision);
};
#endif