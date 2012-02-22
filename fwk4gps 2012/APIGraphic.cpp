/* APIGraphic Implementation - Translation Layer
 *
 * APIGraphic.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIGraphic.h"     // for the APIGraphic class definition
#include "iGraphic.h"       // for the Graphic Interface
#include "APIVertex.h"      // for Vertex static variables
#include "Common_Symbols.h" // symbols common to Modelling/Translation layer

//-------------------------------- APIVertexList ------------------------------
//
// The APIVertexList class hierarchy implements the Vertex List at the API
// level
//
// CreateAPIVertexList creates the APIVertexList object on dynamic memory
//
iAPIGraphic* CreateAPIVertexList(PrimitiveType t, unsigned n, unsigned s, 
 unsigned f, iGraphic* v) {

    return new APIVertexList(t, n, s, f, v);
}

// constructor initializes instance variables and converts to the API types
//
APIVertexList::APIVertexList(PrimitiveType t, unsigned np, unsigned s,
 unsigned f, iGraphic* v) : nPrimitives(np), vertexList(v), vb(nullptr), 
 vertexSize(s), vertexFrmt(f), nVertices(0) {

    // convert to Direct3D type
    switch (t) {
        case POINT_LIST    : type = D3DPT_POINTLIST;     break;
        case LINE_LIST     : type = D3DPT_LINELIST;      break;
        case LINE_STRIP    : type = D3DPT_LINESTRIP;     break;
        case TRIANGLE_LIST : type = D3DPT_TRIANGLELIST;  break;
        case TRIANGLE_STRIP: type = D3DPT_TRIANGLESTRIP; break;
        case TRIANGLE_FAN  : type = D3DPT_TRIANGLEFAN;   break;
        default            : type = D3DPT_POINTLIST;
    }
}

APIVertexList::APIVertexList(const APIVertexList& src) {
    
    vb    = nullptr;
    *this = src;
}

APIVertexList& APIVertexList::operator=(const APIVertexList& src) {

    if (this != &src) {
        vertexList  = src.vertexList;
        vertexSize  = src.vertexSize;
        vertexFrmt  = src.vertexFrmt;
        nPrimitives = src.nPrimitives;
        type        = src.type;
        if (vb) {
            vb->Release();
            vb = nullptr;
        }
    }

    return *this;
}

// setup creates the vertex buffer and populates it
//
void APIVertexList::setup(unsigned n) {

    nVertices = n;

	// create the vertex buffer
    if (FAILED(d3dd->CreateVertexBuffer(vertexSize * n, 0, vertexFrmt, 
     D3DPOOL_DEFAULT, &vb, nullptr))) {
        error(L"APIVertexList::10 Couldn\'t create the vertex buffer");
        vb = nullptr;
    }
    // populate the newly created vertex buffer
    else {
        void* pv;
        if (SUCCEEDED(vb->Lock(0, vertexSize * n, &pv, 0)))
            for (unsigned i = 0; i < n; i++)
                vertexList->populate(i, &pv);
        vb->Unlock();
    }
}

// draw draws the stream of vertices
//
void APIVertexList::draw(unsigned n) {

    if (!vb) setup(n);

    if (vb) {
        d3dd->SetFVF(vertexFrmt);
        d3dd->SetStreamSource(0, vb, 0, vertexSize);
        d3dd->DrawPrimitive(type, 0, nPrimitives);
    }
}

// suspend releases the interface to the vertex buffer
//
void APIVertexList::suspend() {

    if (vb) {
        vb->Release();
        vb = nullptr;
    }
}

// release suspends the vertex buffer
//
void APIVertexList::release() {

    suspend();
}

// destructor releases the vertex buffer
//
APIVertexList::~APIVertexList() {

    release();
}

