/* Texture Implementation - Modelling Layer
 *
 * Texture.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Texture.h"         // for Texture class definition
#include "iCoordinator.h"    // for the Coordinator Interface
#include "iAPITexture.h"     // for the APITexture Interface
#include "iUtilities.h"      // for error(), nameWithDir()

#include "ModellingLayer.h"  // for TEXTURE_DIRECTORY, TEXTURE_ALPHA

//-------------------------------- Texture ------------------------------------
//
// The Texture class defines the structure for a texture in the Modelling Layer
//
// CreateTexture creates a Texture object
//
iTexture* CreateTexture(const wchar_t* file, unsigned filter) {

	return new Texture(file, filter);
}

iTexture* Clone(const iTexture* src) {

    return (iTexture*)src->clone();
}

// constructor adds a pointer to the texture to the coordinator, creates the full
// name of the texture file, stores the name and creates the texture's api
// representation
//
Texture::Texture(const wchar_t* file, unsigned filter) {

	coordinator->add(this);

	wchar_t* fileWithPath = nullptr;
    if (file) {
	    // add the directory to create the relative filename
	    int len = strlen(file) + strlen(TEXTURE_DIRECTORY) + 1;
	    fileWithPath = new wchar_t[len + 1];
	    ::nameWithDir(fileWithPath, TEXTURE_DIRECTORY, file, len);
    }

	// apiTexture on the graphics device
    apiTexture = CreateAPITexture(fileWithPath, filter);

    if (fileWithPath) delete [] fileWithPath;
}

// copy constructor initializes the instance pointers and calls the
// assignment operator
//
Texture::Texture(const Texture& src) {

	coordinator->add(this);
	
	apiTexture = nullptr;
	*this      = src;
}

// assignment operator discards the old data and copies new data
// from src - does not copy the APITexture from src but creates
// a new APITexture instead
//
Texture& Texture::operator=(const Texture& src) {

	if (this != &src) {
		if (apiTexture)
			apiTexture->Delete();
        if (src.apiTexture)
            apiTexture = src.apiTexture->clone();
        else
            apiTexture = nullptr;
	}

	return *this;
}

// attach attaches the apiTexture to the pipeline
//
void Texture::attach() const {

	if (apiTexture)  
		apiTexture->attach();
}

// setFilter sets the filtering on the apiTexture
//
void Texture::setFilter(unsigned flags) const {

	if (apiTexture)  
		apiTexture->setFilter(flags);
}

// detach detaches the apiTexture from the pipeline
//
void Texture::detach() {

	if (apiTexture)
		apiTexture->detach();
}

// render draws the apiTexture 
//
void Texture::render(const Rectf& rect, bool back) {

	if (apiTexture)
		apiTexture->render(rect, TEXTURE_ALPHA, back);
}

// suspend suspends the apiTexture 
//
void Texture::suspend() {

	if (apiTexture)
		apiTexture->suspend();
}

// release releases the apiTexture 
//
void Texture::release() {

	if (apiTexture)
		apiTexture->release();
}

// destructor deletes the graphics device representation and removes the 
// pointer to the texture from the coordinator
//
Texture::~Texture() {

	apiTexture->Delete();
    coordinator->remove(this);
}
