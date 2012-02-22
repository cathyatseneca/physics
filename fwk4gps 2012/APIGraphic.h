#ifndef _API_GRAPHIC_H_
#define _API_GRAPHIC_H_

/* APIGraphic Definition - Translation Layer
 *
 * APIGraphic.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIPlatformSettings.h" // for API headers
#include "APIBase.h"             // for the APIBase class definition
#include "iAPIGraphic.h"         // for the APIGraphic Interface

//-------------------------------- APIGraphic ---------------------------------
//
// The APIVertexList class implements the vertex list at the API level
//
struct IDirect3DVertexBuffer9;

class APIVertexList : public iAPIGraphic, public APIBase {

    unsigned                 nPrimitives; // number of primitives
    D3DPRIMITIVETYPE         type;        // primitive type
    IDirect3DVertexBuffer9*  vb;          // points to the vertex buffer
    iGraphic*                vertexList;  // points to model vertex list
    unsigned                 vertexSize;  // size of a single vertex
    unsigned                 vertexFrmt;  // format of a vertex
    unsigned                 nVertices;   // number of vertices

    virtual ~APIVertexList();
    void setup(unsigned);

public:
    APIVertexList(PrimitiveType t, unsigned np, unsigned, unsigned, iGraphic*);
    APIVertexList& operator=(const APIVertexList&);
    APIVertexList(const APIVertexList& src); 
    iAPIGraphic* clone() const { return new APIVertexList(*this); }
    void draw(unsigned);
    void suspend();
    void release();
    void Delete() const        { delete this; }
};

#endif
