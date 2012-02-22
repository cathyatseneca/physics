/* Graphic and Vertex Implementations - Modelling Layer
 *
 * Graphic.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include <fstream>
#include "Graphic.h"         // for Vertex and Graphic class definitions
#include "iCoordinator.h"    // for the Coordinator Interface
#include "iAPIGraphic.h"     // for the APIGraphic Interface
#include "iUtilities.h"      // for error()

#include "VertexList.h"      // for the VertexList template
#include "MathDefinitions.h" // for Vector and MODEL_Z_AXIS
#include "ModellingLayer.h"  // for ASSET_DIRECTORY
#include "Common_Symbols.h"  // symbols common to Modelling/Translation layers

//-------------------------------- Vertex -------------------------------------
//
// Vertex holds the data for a single normal vertex
//
// constructors initialize the vertex to the values received
//
Vertex::Vertex() : x(0), y(0), z(0), nx(0), ny(0), nz(0), tu(0), tv(0) {}

Vertex::Vertex(const Vector& p, const Vector& n, float ttu, float ttv) :
 x(p.x), y(p.y), z(p.z * MODEL_Z_AXIS), nx(n.x), ny(n.y), nz(n.z), tu(ttu), 
 tv(ttv) {}

// populate populates *pv with the vertex data
//
void Vertex::populate(void** pv) const {
    float* p = *(float**)pv;

    *p++ = x;
    *p++ = y;
    *p++ = z;
    *p++ = nx;
    *p++ = ny;
    *p++ = nz;
    *p++ = tu;
    *p++ = tv;
    *pv  = p;
}

// position returns the position of the vertex in local coordinates
//
Vector Vertex::position() const {
    return Vector(x, y, z);
}

//-------------------------------- LitVertex ----------------------------------
//
// LitVertex holds the data for a single lit vertex
//
LitVertex::LitVertex() : x(0), y(0), z(0), c(0) {}

LitVertex::LitVertex(const Vector& p, const Colour& x, float ttu, 
 float ttv) : x(p.x), y(p.y), z(p.z * MODEL_Z_AXIS), c(x) {}

// populate populates *pv with the vertex data
//
void  LitVertex::populate(void** pv) const {
    float* p = *(float**)pv;

    *p++ = x;
    *p++ = y;
    *p++ = z;
    *((unsigned*)p++) = COLOUR_TO_ARGB(c);
    *pv  = p;
}

// position returns the position of the vertex in local coordinates
//
Vector LitVertex::position() const {
    return Vector(x, y, z);
}

//-------------------------------- Graphic ---------------------------------
//
// The Graphic class is the base class of the Graphic hierarchy
//
// constructor adds the Graphic to the coordinator
//
Graphic::Graphic() {

    coordinator->add(this);
}

Graphic::Graphic(const Graphic& src) {

    coordinator->add(this);
    *this = src;
}

// destructor removes the Graphic from the coordinator
//
Graphic::~Graphic() {

    coordinator->remove(this);
}

//-------------------------------- Graphic Structures -------------------------
//
// prototype for add() function used by the Create...() functions
void add(VertexList<Vertex>* vertexList, const Vector& p1, 
 const Vector& p2, const Vector& p3, const Vector& p4, const Vector& n);

// CreateBox builds a triangle vertex list for a brick-like box from two
// extreme points one face at a time with all faces having the same attributes
//
iGraphic* CreateBox(float minx, float miny, float minz, float maxx, 
 float maxy, float maxz) {
    
    VertexList<Vertex>* vertexList = 
     (VertexList<Vertex>*)CreateVertexList<Vertex>(TRIANGLE_LIST, 12);

    float x = (minx + maxx) / 2;
    float y = (miny + maxy) / 2;
    float z = (minz + maxz) / 2;
    minx -= x;
    miny -= y;
    minz -= z;
    maxx -= x;
    maxy -= y;
    maxz -= z;
    // locate centroid at origin
    Vector p1 = Vector(minx, miny, minz),
           p2 = Vector(minx, maxy, minz),
           p3 = Vector(maxx, maxy, minz),
           p4 = Vector(maxx, miny, minz),
           p5 = Vector(minx, miny, maxz),
           p6 = Vector(minx, maxy, maxz),
           p7 = Vector(maxx, maxy, maxz),
           p8 = Vector(maxx, miny, maxz);
    add(vertexList, p1, p2, p3, p4, Vector(0, 0, -1)); // front
    add(vertexList, p4, p3, p7, p8, Vector(1, 0,  0)); // right
    add(vertexList, p8, p7, p6, p5, Vector(0, 0,  1)); // back
    add(vertexList, p6, p2, p1, p5, Vector(-1, 0, 0)); // left
    add(vertexList, p1, p4, p8, p5, Vector(0, -1, 0)); // bottom
    add(vertexList, p2, p6, p7, p3, Vector(0, 1,  0)); // top

    return vertexList;
}

// CreateGrid builds a grid-like line list of n by n lines in the x-z plane
//
iGraphic* CreateGrid(float min, float max, int n) {
    
    VertexList<Vertex>* vertexList = 
     (VertexList<Vertex>*)CreateVertexList<Vertex>(LINE_LIST, 2*n+2);

    float x = (min + max) / 2;
    min -= x;
    max -= x;
    float cur = min, inc = (max - min) / float(n - 1);
    for (int i = 0; i < n; i++, cur += inc) {
        // in the local x direction
        vertexList->add(Vertex(Vector(min, 0, cur), Vector(0, 1, 0)));
        vertexList->add(Vertex(Vector(max, 0, cur), Vector(0, 1, 0)));
        // in the local z direction
        vertexList->add(Vertex(Vector(cur, 0, min), Vector(0, 1, 0)));
        vertexList->add(Vertex(Vector(cur, 0, max), Vector(0, 1, 0)));
    }

    return vertexList;
}

// CreateRectangleList builds a triangle list in the x-y plane from its two 
// extreme points
//
iGraphic* CreateRectangleList(float minx, float miny, float maxx, float maxy) {
    
    VertexList<Vertex>* vertexList = 
     (VertexList<Vertex>*)CreateVertexList<Vertex>(TRIANGLE_LIST, 2);

    float x = (minx + maxx) / 2, y = (miny + maxy) / 2;
    minx -= x;
    miny -= y;
    maxx -= x;
    maxy -= y;
    // locate centroid at origin
    Vector p1 = Vector(minx, miny, 0),
           p2 = Vector(minx, maxy, 0),
           p3 = Vector(maxx, maxy, 0),
           p4 = Vector(maxx, miny, 0);
    add(vertexList, p1, p2, p3, p4, Vector(0, 0, -1)); 
    
    return vertexList;
}

// TriangleList reads a triangle list form file
//
iGraphic* TriangleList(const wchar_t* file) {
    
    iGraphic* graphic = nullptr;

	// construct filename with path
	int len = strlen(file) + strlen(ASSET_DIRECTORY) + 1;
	wchar_t* absFile = new wchar_t[len + 1];
	nameWithDir(absFile, ASSET_DIRECTORY, file, len);

    // open file for input
    std::wifstream in(absFile, std::ios::in);
    delete [] absFile;

    float x, y, z, nx, ny, nz, tu, tv, xc = 0, yc = 0, zc = 0;
    unsigned no = 0;

    // count the number of records
    while (in) { 
        in >> x >> y >> z >> nx >> ny >> nz >> tu >> tv;
        xc += x;
        yc += y;
        zc += z;
        no++;
    }
    in.clear();
    in.seekg(0);
    if (no) {
        VertexList<Vertex>* vertexList = 
         (VertexList<Vertex>*)CreateVertexList<Vertex>(TRIANGLE_LIST, no);
        xc /= no;
        yc /= no;
        zc /= no;
        for (unsigned i = 0; i < no; i++) {
            in >> x >> y >> z >> nx >> ny >> nz >> tu >> tv;
            vertexList->add(Vertex(Vector(x - xc, y - yc, (z - zc) * MODEL_Z_AXIS), 
                Vector(nx, ny, nz), tu, tv));
        }
        graphic = vertexList;
    }
    
    return graphic;
}

// TriangleList reads a triangle list from file
//
iGraphic* TriangleList(const wchar_t* file, const Colour& colour) {
    
    iGraphic* graphic = nullptr;

	// construct filename with path
	int len = strlen(file) + strlen(ASSET_DIRECTORY) + 1;
	wchar_t* absFile = new wchar_t[len + 1];
	::nameWithDir(absFile, ASSET_DIRECTORY, file, len);

    // open file for input
    std::wifstream in(absFile, std::ios::in);
    delete [] absFile;

    float x, y, z, xc = 0, yc = 0, zc = 0;
    unsigned no = 0;

    // count the number of records
    while (in) { 
        in >> x >> y >> z;
        xc += x;
        yc += y;
        zc += z;
        no++;
    }
    in.clear();
    in.seekg(0);
    if (no) {
        VertexList<LitVertex>* vertexList = 
         (VertexList<LitVertex>*)CreateVertexList<LitVertex>(TRIANGLE_LIST, no);
        xc /= no;
        yc /= no;
        zc /= no;
        for (unsigned i = 0; i < no; i++) {
            in >> x >> y >> z;
            vertexList->add(LitVertex(Vector(x - xc, y - yc, (z - zc) * MODEL_Z_AXIS), 
                colour));
        }
        graphic = vertexList;
    }
    
    return graphic;
}

void add(VertexList<Vertex>* vertexList, const Vector& p1, const Vector& p2, 
 const Vector& p3, const Vector& p4, const Vector& n) {

    vertexList->add(Vertex(p1, n, 1, 0));
    vertexList->add(Vertex(p2, n, 0, 0));
    vertexList->add(Vertex(p3, n, 0, 1));
    vertexList->add(Vertex(p1, n, 1, 0));
    vertexList->add(Vertex(p3, n, 0, 1));
    vertexList->add(Vertex(p4, n, 1, 1));
}

