#ifndef _COORDINATOR_H_
#define _COORDINATOR_H_

/* Coordinator Definition - Modelling Layer
 *
 * Coordinator.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include <vector>
#include "iCoordinator.h"     // for the Coordinator Interface
#include "MathDeclarations.h" // for Matrix

//-------------------------------- Templates for add and remove ---------------
//
// add adds a pointer to *o in vector v
//
template <class T>
void add(std::vector<T>& v, T o) {
    unsigned i;
    bool rc = false;
    
    for (i = 0; i < v.size() && !rc; i++)
        if (!v[i]) {
            v[i] = o;
            rc = true;
        }
    if (!rc)
        v.push_back(o);
}

// remove removes the pointer to *o from vector v
//
template <class T>
bool remove(std::vector<T>& v, T o) {
    bool rc = false;

    for (unsigned i = 0; i < v.size(); i++)
        if (v[i] == o) {
            v[i] = 0;
            rc = true;
        }
    while (v.size() && !v[v.size() - 1])
        v.pop_back();

    return rc;
}

//-------------------------------- Coordinator --------------------------------
//
// The Coordinator class coordinates all design elements in the Modelling Layer 
//
enum Category;
class iAPIWindow;
class iAPIUserInput;
class iAPIDisplay;
class iAPIAudio;

class Coordinator : public iCoordinator {

    iAPIWindow*            window;           // points to the window object
    iAPIUserInput*         userInput;        // points to the user input object
    iAPIDisplay*           display;          // points to the display object
    iAPIAudio*             audio;            // points to the audio object

    std::vector<iObject*>  object;           // points to objects
	std::vector<iTexture*> texture;          // points to textures
    std::vector<iLight*>   light;            // points to light sources
    std::vector<iCamera*>  camera;           // points to cameras
    std::vector<iSound*>   sound;            // points to sound sources
    std::vector<iGraphic*> graphic;          // points to graphics
    std::vector<iText*>    text;             // points to text items
    std::vector<iHUD*>     hud;              // points to huds

    unsigned               framecount;       // no of frames since 'lastReset'
    unsigned               fps;              // frame rate per sec
    unsigned               lastReset;        // last time framecount reset to 0

    unsigned               currentCam;       // index - current camera
    unsigned               lastCameraToggle; // time of most recent cam toggle
    unsigned               currentHUD;       // index - current HUD
    unsigned               lastHUDToggle;    // time of most recent hud toggle

    iTexture*              background;       // points to background texture
    iText*                 timerText;        // points to timer's text object

    // display device
    float                  nearcp;           // near clipping plane
    float                  farcp;            // far clipping plane
    float                  fov;              // field of view in radians
    Colour                 ambient;          // background lighting

    Coordinator(const Coordinator& s);            // prevents copying
    Coordinator& operator=(const Coordinator& s); // prevents assignment
    bool getConfiguration();
    void adjustVolume(int);
    void adjustFrequency(int);
	void update();
    void render();
    void render(iObject*);
    void render(Category category);

  protected:
	// configuration
    void initialize()                 { }
    void setProjection(float, float, float);
    void setAmbientLight(float, float, float);
    void setTimerText(void* text)     { timerText = (iText*)text; }
    void setBackground(void* texture) { background = (iTexture*)texture; }
 	// execution
    bool pressed(Action a) const;
    bool ptrPressed() const;
    bool ctrPressed() const;
    int  change(Action a) const;
    const wchar_t* soundFile (ModelSound s) const;
    virtual ~Coordinator();

  public:
    static iCoordinator* Address() { return coordinator; }
    Coordinator(void*, int);
	// initialization
    void  add(iObject* o)  { ::add(object, o); }
    void  add(iTexture* t) { ::add(texture, t); }
    void  add(iLight* l)   { ::add(light, l); }
    void  add(iCamera* c)  { ::add(camera, c); }
    void  add(iSound* s)   { ::add(sound, s); }
    void  add(iGraphic* g) { ::add(graphic, g); }
    void  add(iText* t)    { ::add(text, t); }
    void  add(iHUD* h)     { ::add(hud, h); }
    void  reset();
	// execution
    int   run();
    void  resize();
    // termination
    void  suspend();
	void  restore();
    void  release();
    void  remove(iObject* o)  { ::remove(object, o); }
    void  remove(iTexture* t) { ::remove(texture, t); }
    void  remove(iLight* l)   { ::remove(light, l); }
    void  remove(iCamera* c)  { ::remove(camera, c); }
    void  remove(iSound* s)   { ::remove(sound, s); }
    void  remove(iGraphic* g) { ::remove(graphic, g); }
    void  remove(iText* t)    { ::remove(text, t); }
    void  remove(iHUD* h)     { ::remove(hud, h); }
};

#endif
