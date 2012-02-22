#ifndef _API_DISPLAY_H_
#define _API_DISPLAY_H_

/* APIDisplay Definition - Translation Layer
 *
 * APIDisplay.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIPlatformSettings.h" // for graphics api headers
#include "APIBase.h"             // for the APIBase class definiton
#include "iAPIDisplay.h"         // for the APIDisplay Interface
#include "MathDeclarations.h"    // for Colour
#include "GeneralConstants.h"    // for MAX_DESC

//-------------------------------- APIDisplaySet ------------------------------
//
// The APIDisplaySet class holds the information for the API Displays 
//
class APIDisplaySet : public iAPIDisplaySet, public APIBase {

    // available configuration dimensions
    int     nAdapters;     // number of available adapters
    int     nModes;        // number of available resolution modes
    int     nPixelFmts;    // number of pixel formats
    int     (*modeDim)[2]; // points to list of mode widths, heights
    wchar_t (*modeDes)[MAX_DESC + 1]; // points to list of mode descriptions
    wchar_t (*adptDes)[MAX_DESC + 1]; // points to list of adapter descriptions

    APIDisplaySet(const APIDisplaySet& d);            // prevents copying
	APIDisplaySet& operator=(const APIDisplaySet& d); // prevents assignments
    virtual ~APIDisplaySet();

  public:
    APIDisplaySet();
	bool interrogate();
    int  noAdapters() const                  { return nAdapters; }
    int  noModes() const                     { return nModes; }
    int  noPixelFormats() const              { return nPixelFmts; }
    const wchar_t* adapterDesc(int id) const { return adptDes[id]; }
    const wchar_t* modeDesc(int id, int ir, int ip) const;
    int  getWidth(int id, int ir, int ip) const;
    int  getHeight(int id, int ir, int ip) const;
	void Delete()                            { delete this; }
};

//-------------------------------- APIDisplay ---------------------------------
//
// The APIDisplay class manages the API connectivity for the Graphics Card 
//
class APIDisplay : public iAPIDisplay, public APIBase {

    // selected configuration
    int      displayId;        // APIDisplay adapter identifier
    int      mode;             // resolution mode identifier
    int      pixel;            // pixel format identifier
    float    red, green, blue; // global ambient light
    unsigned maxLights;        // max no of lights supported by selected adapter

    D3DPRESENT_PARAMETERS d3dpp; // parameters for creating/restoring D3D
                                 // APIDisplay device

    void setupProjection();      // sets up the projection matrix
    void setupLighting();        // sets up the lighting
	void setupBlending();        // sets up the alpha blending

	APIDisplay(const APIDisplay& d);            // prevents copying
	APIDisplay& operator=(const APIDisplay& d); // prevents assignments
    virtual ~APIDisplay();

  public:
    APIDisplay();
	// configuration
    void configure(int, int, int);
    void setProjection(void*);
    void setAmbientLight(float, float, float);
    bool setup();
	// execution
    void beginDrawFrame(const void*);
    void setWorld(const void*);
    void setReflectivity(const void*);
    void set(RenderState, bool);
    void beginDrawHUD(unsigned flags);
    void endDrawHUD();
    void endDrawFrame();
	// termination
    void suspend();
    bool restore();
    void release();
	void Delete() { delete this; }
};

#endif