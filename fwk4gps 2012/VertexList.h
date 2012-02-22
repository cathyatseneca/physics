#ifndef _VERTEX_LIST_H_
#define _VERTEX_LIST_H_

/* Graphic Implementation - Modelling Layer
 *
 * VertexList.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Graphic.h"             // for Graphic class definition

//-------------------------------- VertexList ---------------------------------
//
// The VertexList class defines the structure of a set of <T> vertices
//
template <class T = Vertex>
class VertexList : public Graphic {

    unsigned     maxNo;         // maximum number of vertices
    unsigned     no;            // number of vertices stored
    T*           vertex;        // points to the array of vertices
    iAPIGraphic* apiVertexList; // points to the API Primitive Set

    virtual ~VertexList() { apiVertexList->Delete(); delete [] vertex; }

  public:
    VertexList(PrimitiveType, int);
    VertexList& operator=(const VertexList&);
    VertexList() : vertex(0), maxNo(0), no(0) { }
    VertexList(const VertexList& src)         { vertex = 0; *this = src; }
    void*  clone() const                      { return new VertexList(*this); }
    int    add(const T& v)                    { if (no < maxNo) vertex[no++] = v; return no; }
    void   populate(unsigned i, void** pv)    { vertex[i].populate(pv); }
    Vector position(int i) const              { return vertex[i].position(); }
    void   render()                           { apiVertexList->draw(no); }
    void   suspend()                          { apiVertexList->suspend(); }
    void   release()                          { apiVertexList->release(); }
    void   Delete() const                     { delete this; }
};

// CreateVertexList creates a Vertex List object
//
template <class T>
iGraphic* CreateVertexList(PrimitiveType t, int np) {

    return new VertexList<T>(t, np);
}

// constructor allocates memory for the list and creates the Translation
//
template <class T>
VertexList<T>::VertexList(PrimitiveType t, int np) : no(0) {

    if (np <= 0) {
        maxNo  = 0;
        vertex = 0;
    }
    else {
        // Determine the number of vertices for the Primitive Type
        switch (t) {
            case POINT_LIST:     maxNo = np;     break;
            case LINE_LIST:      maxNo = 2 * np; break;
            case LINE_STRIP:     maxNo = np + 1; break;
            case TRIANGLE_LIST:  maxNo = 3 * np; break;
            case TRIANGLE_STRIP: maxNo = np + 2; break;
            case TRIANGLE_FAN:   maxNo = np + 1; break;
            default: maxNo = np;
        }
        vertex = new T[maxNo];
    }

    apiVertexList = CreateAPIVertexList(t, np, T::vertexSize(), T::vertexFormat(), (iGraphic*)this);
}

// assignment operator copies the vertex list and clone the Translation
//
template <class T>
VertexList<T>& VertexList<T>::operator=(const VertexList<T>& src) {

    if (this != &src) {
        maxNo = src.maxNo;
        no    = src.no;
        if (vertex) {
            delete [] vertex;
            vertex = 0;
        }
         vertex = new T[no];
         for (unsigned i = 0; i < no; i++)
             vertex[i] = src.vertex[i];
        if (apiVertexList)
            apiVertexList->Delete();
        apiVertexList = src.apiVertexList->clone();
    }

    return *this;
}

#endif