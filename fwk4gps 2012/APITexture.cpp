/* APITexture Implementation - Translation Layer
 *
 * APITexture.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIPlatformSettings.h" // for API headers
#include "APITexture.h"          // for the APITexture class definition
#include "iUtilities.h"          // for strlen()
#include "Common_Symbols.h"      // symbols common to Modelling/Translation

//-------------------------------- APITexture ---------------------------------
//
// The APITexture class implements a texture at the API level
//
iAPITexture* CreateAPITexture(const wchar_t* file, unsigned filter) {

	return new APITexture(file, filter);
}

// constructor initializes the texture identifier
//
APITexture::APITexture(const wchar_t* file, unsigned f) : filter(f) {

	if (file) {
        int len = strlen(file);
	    this->file = new wchar_t[len + 1];
	    strcpy(this->file, file, len);
    }
    else
        this->file = nullptr;

    tex = nullptr;
}

APITexture::APITexture(const APITexture& src) {

    file  = nullptr;
    tex   = nullptr;
    *this = src;    
}

iAPITexture& APITexture::operator=(const APITexture& src) {

    if (this != &src) {
        if (file)
            delete [] file;
        if (src.file) {
            int len = strlen(src.file);
            file = new wchar_t[len + 1];
            strcpy(file, src.file, len);
        }
        else
            file = nullptr;
        suspend();
        filter = src.filter;
        tex = nullptr;
    }

    return *this;
}

// setup creates the api texture from the texture file
//
void APITexture::setup(int width, int height, int key) {

    // create a texture COM object from the texture file
    //
	if (manager && file && FAILED(D3DXCreateTextureFromFileEx(d3dd, file, 
     width, height, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
     D3DX_DEFAULT, D3DX_DEFAULT, key, nullptr, nullptr, &tex))) {
		error(L"APITexture::11 Failed to create texture COM object from file");
		tex = nullptr;
	}
}

// render draws the texture directly
//
void APITexture::render(const Rectf& r, unsigned char alpha, bool back) {

	if (!tex) setup((int)(width * (r.bottomRightX - r.topLeftX)), 
	 (int)(height * (r.bottomRightY - r.topLeftY)), 0xFF000000);

	if (manager && tex) {
        float x = width * r.topLeftX, y = height * r.topLeftY;
		D3DXVECTOR3 topLeft(x, y, back ? 1.f : 0.f);
		manager->Draw(tex, nullptr, nullptr, &topLeft, 
         D3DCOLOR_RGBA(255, 255, 255, back ? '\xFF' : alpha));
	}
}

// attach attaches the api texture to sampling stage i
//
void APITexture::setFilter(unsigned filter) {

    if (tex)
		setSamplerState(0, filter);
}

// attach attaches the api texture to sampling stage i
//
void APITexture::attach() {

	if (!tex) setup(0, 0, 0);

    if (tex) {
        d3dd->SetTexture(0, tex);
		setSamplerState(0, filter);
    }
}

// setSamplerState sets the sampling state for the texture according 
// to the filtering flags
//
void APITexture::setSamplerState(int i, unsigned flags) const {

    if (flags & TEX_MIN_POINT)
        d3dd->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    else if (flags & TEX_MIN_LINEAR)
        d3dd->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    else if (flags & TEX_MIN_ANISOTROPIC)
        d3dd->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC); 
    if (flags & TEX_MAG_POINT)
        d3dd->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    else if (flags & TEX_MAG_LINEAR)
        d3dd->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

// detach detaches the api texture from sampling stage 0
//
void APITexture::detach() {

    if (tex)
        d3dd->SetTexture(0, nullptr);
}

// suspend releases the api texture
//
void APITexture::suspend() {

    // release the Interface to the texture COM object
    if (tex) {
        tex->Release();
        tex = nullptr;
    }
}

// releases suspends the api texture
//
void APITexture::release() {

    suspend();
}

// destructor releases the api texture 
//
APITexture::~APITexture() {

   release();
}
