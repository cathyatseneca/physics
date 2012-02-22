#ifndef _I_API_DISPLAY_H_
#define _I_API_DISPLAY_H_

/* APIDisplaySet and APIDisplay Interfaces - Translation Layer
 *
 * iAPIDisplay.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPIDisplaySet -----------------------------
//
// iAPIDisplay is the Interface to the APIDisplay object
//
class iAPIDisplaySet {
  public:
	virtual bool  interrogate()                                        = 0;
    virtual int   noAdapters() const                                   = 0;
    virtual int   noModes() const                                      = 0;
    virtual int   noPixelFormats() const                               = 0;
    virtual const wchar_t* adapterDesc(int id) const                   = 0;
    virtual const wchar_t* modeDesc(int id, int ir, int ip) const      = 0;
    virtual int   getWidth(int, int, int) const                        = 0;
    virtual int   getHeight(int, int, int) const                       = 0;
	virtual void  Delete()                                             = 0;
};

iAPIDisplaySet* CreateAPIDisplaySet();

//-------------------------------- iAPIDisplay --------------------------------
//
// iAPIDisplay is the Interface to the APIDisplay object
//
enum RenderState;

class iAPIDisplay {
  public:
	// configuration 
    virtual void configure(int, int, int)                             = 0;
    virtual void setProjection(void*)                                 = 0;
    virtual void setAmbientLight(float, float, float)                 = 0;
    virtual bool setup()                                              = 0;
	// execution
    virtual void beginDrawFrame(const void*)                          = 0;
    virtual void setWorld(const void*)                                = 0;
    virtual void setReflectivity(const void*)                         = 0;
    virtual void set(RenderState, bool)                               = 0;
    virtual void beginDrawHUD(unsigned flags)                         = 0;
    virtual void endDrawHUD()                                         = 0;
    virtual void endDrawFrame()                                       = 0;
	// termination
    virtual void suspend()                                            = 0;
    virtual bool restore()                                            = 0;
    virtual void release()                                            = 0;
	virtual void Delete()                                             = 0;
};

iAPIDisplay* CreateAPIDisplay();

#endif