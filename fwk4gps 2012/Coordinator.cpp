/* Coordinator Implementation - Modelling Layer
 *
 * Coordinator.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Coordinator.h"     // for the Coordinator class definition
#include "iAPIWindow.h"      // for the API Window Interface
#include "iAPIUserInput.h"   // for the APIUserInput Interface
#include "iAPIDisplay.h"     // for the APIDisplay Interface
#include "iAPIAudio.h"       // for the APIAudio Interface
#include "iUtilities.h"      // for strcpy, sprintf, strcmp
#include "Camera.h"          // for the Camera class definition
#include "iObject.h"         // for the Object Interface
#include "iTexture.h"        // for the Texture Interface
#include "iLight.h"          // for the Light Interface
#include "iSound.h"          // for the Sound Interface
#include "iGraphic.h"        // for the Graphic Interface
#include "iText.h"           // for the Text Interface
#include "iHUD.h"            // for the HUD Interface
#include "ModellingLayer.h"  // for macros
#include "MathDefinitions.h" // for ::projection
#include "Common_Symbols.h"  // for Action and Sound enumerations
#define FPS_MAX 200


//-------------------------------- Coordinator --------------------------------
//
// The Coordinator object manages the design items of the Modelling Layer
//
iCoordinator* CoordinatorAddress() { return Coordinator::Address(); }

// constructor initializes the reference time and sets the current camera
// and HUD
//
Coordinator::Coordinator(void* hinst, int show) {

    coordinator = this;
    window      = CreateAPIWindow(hinst, show);
    userInput   = CreateAPIUserInput(AUDIO_DIRECTORY);
    display     = CreateAPIDisplay();
    audio       = CreateAPIAudio(1.0f, MIN_VOLUME, MAX_VOLUME, MIN_FREQUENCY, 
     MAX_FREQUENCY, DEFAULT_VOLUME, DEFAULT_FREQUENCY);

    // timers
    now              = 0;
    lastReset        = 0;
    lastUpdate       = 0;
    lastCameraToggle = 0;
    lastHUDToggle    = 0;
    framecount       = 0;
    fps              = 0;

    // current camera and HUD
    currentCam = 0;
    currentHUD = 0;

    // volume and frequency settings
    frequency = DEFAULT_FREQUENCY;
    volume    = DEFAULT_VOLUME;

    // pointers
    timerText  = nullptr;
    background = nullptr;

    // projection parameters are updated
    fov    = 0.9f;
    nearcp = 1.0f;
    farcp  = 1000.0f;
}

// getConfiguration retrieves the configuration selection from the user
//
bool Coordinator::getConfiguration() { 
    
    bool rc = false;

    if (userInput->getConfiguration()) {
        release();
        userInput->configure();         
        window->configure();
        if (window->setup() && userInput->setup() && display->setup() && 
         audio->setup()) {
            Matrix p = ::projection(fov, window->aspectRatio(), nearcp, farcp);
            display->setProjection(&p);
            rc = true;
        }
    }

    // reset timers
    now = window->time();
    lastUpdate = now;
    lastReset  = now;

    return rc;
}

// reset resets the configuration
//
void Coordinator::reset() {

    if (getConfiguration()) {
        // reset the sound files
	    for (unsigned i = 0; i < sound.size(); i++) {
            if (sound[i]->relFileName() && 
    	     strcmp(soundFile((ModelSound)i), sound[i]->relFileName()))
		     sound[i]->change(soundFile((ModelSound)i));
        }
    }
}

// processMessages processes the next message in the message queue 
// returns false if queue is empty
//
int Coordinator::run() {

	int  rc = 0;
    bool keepgoing;

    // configure and initialize the application
    if (keepgoing = getConfiguration()) {
        initialize();
        now = window->time();
        lastUpdate = now;
        lastReset  = now;
    }

	while (keepgoing) {
        // process all system messages as the first priority
		if (window->processMessages(rc, keepgoing)) 
            ; // intentional
        else if (!active)
            window->wait();
        else {
            // opportunity to render a frame if no messages
            now = window->time();
            // render only if sufficient time has elapsed since the last frame
	        if (now - lastUpdate >= UNITS_PER_SEC / FPS_MAX) {
                // render the frame
                render();
                // update the reference time
                lastUpdate = now;
            }
        }
	}

    return rc;
}

// setAmbientLight sets the colour of the background lighting
//
void Coordinator::setAmbientLight(float r, float g, float b) {

    ambient = Colour(r, g, b); 
}

// setProjection sets the angle and clipping planes for the projection matrix
//
void Coordinator::setProjection(float angle, float n, float f) {

    fov    = angle;
    nearcp = n;
    farcp  = f;
}

// pressed returns the on/off status of Action a
//
bool Coordinator::pressed(Action a) const { return userInput->pressed(a); }

// ptrPressed returns the on/off status of Action a
//
bool Coordinator::ptrPressed() const { return userInput->ptrPressed(); }

// ctrPressed returns the on/off status of Action a
//
bool Coordinator::ctrPressed() const { return userInput->ctrPressed(); }

// change returns the change caused by Action a 
//
int Coordinator::change(Action a) const { return userInput->change(a); }

// soundFile returns the address of the soundFile associated with ModelSound s
//
const wchar_t* Coordinator::soundFile(ModelSound s) const { 
    
    return userInput->soundFile(s); 
}

// adjustVolume adjusts the volume of the audio system by factor *
// incVolume - positive factor increases the volume, negative factor
// decreases the volume
//
void Coordinator::adjustVolume(int factor) {

    if (factor > 0)
        volume += STEP_VOLUME;
    else if (factor < 0)
        volume -= STEP_VOLUME;
    if (volume > MAX_VOLUME)
        volume = MAX_VOLUME;
    else if (volume < MIN_VOLUME)
        volume = MIN_VOLUME;
}

// adjustFrequency adjusts the frequency of the audio system by factor *
// incVolume - positive factor increases the volume, negative factor
// decreases the volume
//
void Coordinator::adjustFrequency(int factor) {

    if (factor < 0) {
        frequency = frequency + STEP_FREQUENCY;
        frequency = frequency < MIN_FREQUENCY ? MIN_FREQUENCY : frequency;
    }
    else if (factor > 0) {
        frequency = frequency + STEP_FREQUENCY;
        frequency = frequency > MAX_FREQUENCY ? MAX_FREQUENCY : frequency;
    }
}

// update updates the current Camera and HUD objects, the lights and the sounds
//
void Coordinator::update() {

	// toggle and update the current camera
	//if (camera.size() && userInput->pressed(CAMERA_SELECT) && 
 //       now - lastCameraToggle > KEY_LATENCY) {
 //       lastCameraToggle = now;
 //       currentCam++;
 //       if (currentCam == camera.size())
 //           currentCam = 0;
 //   }
    if (camera.size() && camera[currentCam])
       camera[currentCam]->update();

	//// toggle and update the current hud
	//if (hud.size() && userInput->pressed(HUD_SELECT) &&
 //       now - lastHUDToggle > KEY_LATENCY) {
 //       lastHUDToggle = now;
 //       currentHUD++;
 //       if (currentHUD == hud.size())
 //           currentHUD = 0;
 //   }
 //   if (hud.size() && hud[currentHUD] && userInput->pressed(HUD_DISPLAY))
 //       hud[currentHUD]->toggle();
 //   if (hud.size() && hud[currentHUD]) 
 //       hud[currentHUD]->update();

 //   // update the volume and the frequency
	//if (now - lastUpdate > KEY_LATENCY) {

	//	if (userInput->pressed(AUD_VOLUME_DEC))
	//		adjustVolume(-1);
	//	if (userInput->pressed(AUD_VOLUME_INC))
	//		adjustVolume(1);

 //       if (userInput->pressed(AUD_FREQ_DEC))
 //           adjustFrequency(-1);
 //       else if (userInput->pressed(AUD_FREQ_INC))
 //           adjustFrequency(1);
	//}	

    // update the sound sources
    for (unsigned i = 0; i < sound.size(); i++)
		if (sound[i]) 
			sound[i]->update();

	// update the light sources
    for (unsigned i = 0; i < light.size(); i++)
		if (light[i])
			light[i]->update();
}

// renders draws a complete frame
//
void Coordinator::render() {

	// adjust framecount and fps
    if (now - lastReset <= unitsPerSec) 
		framecount++;
	else {
        // recalculate the frame rate
        fps        = framecount * unitsPerSec / (now - lastReset);
		framecount = 0;
		lastReset  = now;
        if (timerText) {
            wchar_t str[MAX_DESC + 1];
            sprintf(str, fps, L" fps");
	        timerText->set(str);
        }
	}
    // update the user input devices
    userInput->update();
    Coordinator::update();
    // update the model
    update();
    // update the audio
    audio->setVolume(volume);
    audio->setFrequencyRatio(frequency);
    audio->update(Camera::getView());

    // start rendering
    display->beginDrawFrame(Camera::getView());
    if (background) { 
        Rectf fullScreen(0, 0, 1, 1);
        display->beginDrawHUD(0);
        background->render(fullScreen, true);
        display->endDrawHUD();
    }
    display->setAmbientLight(ambient.r, ambient.g, ambient.b);
    render(OPAQUE_OBJECT);
    display->set(ALPHA_BLEND, true);
    render(TRANSLUCENT_OBJECT);
    display->set(ALPHA_BLEND, false);
    display->beginDrawHUD(HUD_ALPHA);
    render(ALL_HUDS);
    display->endDrawHUD();
    display->endDrawFrame();
    render(ALL_SOUNDS);
}

// render draws the coordinator elements for the specified Category
//
void Coordinator::render(Category category) {

    switch (category) {
        case ALL_OBJECTS:
            // draw all objects
            for (unsigned i = 0; i < object.size(); i++) {
		        if (object[i])
                    render(object[i]);
            }
            break;
        case ALL_HUDS:
            // draw all huds
            for (unsigned i = 0; i < hud.size(); i++)
                if (hud[i] && hud[i]->isOn())
                    hud[i]->render();
            for (unsigned i = 0; i < text.size(); i++)
                if (text[i] && text[i]->getHUD() && text[i]->getHUD()->isOn())
                    text[i]->render();
            break;
        case ALL_SOUNDS:
            // render all sounds
            for (unsigned i = 0; i < sound.size(); i++)
		        if (sound[i]) 
			        sound[i]->render();
            break;
        default:
            // draw objects that only belong to category
            for (unsigned i = 0; i < object.size(); i++) {
		        if (object[i] && object[i]->belongsTo(category))
                    render(object[i]);
            }
    }
}

// render draws a single object (*object) 
//
void Coordinator::render(iObject* object) {

    display->setWorld(&object->world());
    iTexture* texture = object->getTexture();
    if (texture) texture->attach();
    const void* reflectivity = object->getReflectivity();
    if (reflectivity)
        display->setReflectivity(reflectivity);
    else
        display->set(LIGHTING, false);
    object->render();
    if (!reflectivity)
        display->set(LIGHTING, true);
    if (texture) texture->detach();
}

// resize resizes the window and the user interface
//
void Coordinator::resize() {

    if (active && window->getWindowMode()) {
        window->resize();
        Matrix p = ::projection(fov, window->aspectRatio(), nearcp, farcp);
        display->setProjection(&p);
    }
}

// suspend suspends all of the design items
//
void Coordinator::suspend() {

	for (unsigned i = 0; i < texture.size(); i++)
		if (texture[i])
			texture[i]->suspend();

	for (unsigned i = 0; i < light.size(); i++)
		if (light[i])
			light[i]->suspend();

    for (unsigned i = 0; i < sound.size(); i++)
		if (sound[i]) 
			sound[i]->suspend();

    for (unsigned i = 0; i < graphic.size(); i++)
		if (graphic[i]) 
			graphic[i]->suspend();

    for (unsigned i = 0; i < text.size(); i++)
        if (text[i])
			text[i]->suspend();

    display->suspend();
    userInput->suspend();
    audio->suspend();
    active = false;
}

// restore restores all of the design items and initializes the timers
//
void Coordinator::restore() {

    now = window->time();
    userInput->restore();
    display->restore();
    Matrix p = ::projection(fov, window->aspectRatio(), nearcp, farcp);
    display->setProjection(&p);
    audio->restore();

    for (unsigned i = 0; i < camera.size(); i++)
        if (camera[i])
			camera[i]->restore();
	for (unsigned i = 0; i < light.size(); i++)
		if (light[i])
			light[i]->restore();
	for (unsigned i = 0; i < sound.size(); i++)
		if (sound[i]) 
			sound[i]->restore();
    for (unsigned i = 0; i < hud.size(); i++)
        if (hud[i])
			hud[i]->restore();
    for (unsigned i = 0; i < text.size(); i++)
        if (text[i])
			text[i]->restore();

    lastCameraToggle = now;
    lastHUDToggle    = now;
    lastUpdate       = now;
    active           = true;
}

// release releases the design items
//
void Coordinator::release() {

	for (unsigned i = 0; i < texture.size(); i++)
		if (texture[i])
			texture[i]->release();

    for (unsigned i = 0; i < graphic.size(); i++)
		if (graphic[i]) 
			graphic[i]->release();

    for (unsigned i = 0; i < text.size(); i++)
        if (text[i])
			text[i]->release();

    for (unsigned i = 0; i < sound.size(); i++)
        if (sound[i])
			sound[i]->release();

    display->release();
    userInput->release();
    audio->release();
    window->release();
}

// destructor deletes all of the coordinator elements
//
Coordinator::~Coordinator() {

    for (unsigned i = 0; i < object.size(); i++)
        if (object[i]) 
            object[i]->Delete();

    for (unsigned i = 0; i < texture.size(); i++)
        if (texture[i]) 
            texture[i]->Delete();

    for (unsigned i = 0; i < light.size(); i++)
        if (light[i])
            light[i]->Delete();

    for (unsigned i = 0; i < camera.size(); i++)
        if (camera[i])
            camera[i]->Delete();

    for (unsigned i = 0; i < sound.size(); i++)
        if (sound[i])
            sound[i]->Delete();

    for (unsigned i = 0; i < graphic.size(); i++)
        if (graphic[i])
            graphic[i]->Delete();

    for (unsigned i = 0; i < text.size(); i++)
        if (text[i]) 
			text[i]->Delete();

    display->Delete();
    userInput->Delete();
    audio->Delete();
	window->Delete();

    coordinator = nullptr;
}

