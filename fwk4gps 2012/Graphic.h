#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

/* Graphic and Vertex Definitions - Modelling Layer
 *
 * Graphic.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iGraphic.h"         // for the Graphic Interface
#include "MathDeclarations.h" // for Colour

//-------------------------------- LitVertex ----------------------------------
//
// The LitVertex class defines the structure for a single coloured vertex
//
class LitVertex {

    float  x; // x coordinate in the local frame
    float  y; // y coordinate in the local frame
    float  z; // z coordinate in the local frame
    Colour c; // colour
    static unsigned size;
    static unsigned format;

  public:
    static unsigned vertexSize()   { return size; }
    static unsigned vertexFormat() { return format; }
    LitVertex();
    LitVertex(const Vector&, const Colour&, float = 0, float = 0);
    void   populate(void**) const;
    Vector position() const;
};

//-------------------------------- Vertex -------------------------------------
//
// The Vertex class defines the structure for a single vertex
//
class Vertex {

    float x;   // x coordinate in the local frame
    float y;   // y coordinate in the local frame
    float z;   // z coordinate in the local frame
    float nx;  // x coordinate of normal in the local frame
    float ny;  // y coordinate of normal in the local frame
    float nz;  // z coordinate of normal in the local frame
    float tu;  // u coordinate of texture
    float tv;  // v coordinate of texture
    static unsigned size;
    static unsigned format;

  public:
    static unsigned vertexSize()   { return size; }
    static unsigned vertexFormat() { return format; }
    Vertex();
    Vertex(const Vector&, const Vector&, float = 0, float = 0);
    void   populate(void**) const;
    Vector position() const;
};

//-------------------------------- Graphic ------------------------------------
//
// The Graphic class defines the structure of the graphic representations
//
class iAPIGraphic;

class Graphic : public iGraphic {

protected:
    
    Graphic();
    Graphic(const Graphic&);
    virtual ~Graphic();
};

#endif
