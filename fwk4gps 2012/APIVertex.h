#ifndef _API_VERTEX_H_
#define _API_VERTEX_H_

/* APIVertex Data - Translation Layer
 *
 * APIVertex.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIPlatformSettings.h" // for the D3D constants
#include "Graphic.h"             // for Vertex, Coloured Vertex defs

unsigned Vertex::size = 32;
unsigned Vertex::format = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 \
                             | D3DFVF_TEXCOORDSIZE2(0);
unsigned LitVertex::size = 16;
unsigned LitVertex::format = D3DFVF_XYZ | D3DFVF_DIFFUSE;

#endif
