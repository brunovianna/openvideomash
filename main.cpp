#include "ofMain.h"
#include "testApp.h"


#define MWM_HINTS_DECORATIONS (1L << 1)

typedef struct
{
  int flags;
  int functions;
  int decorations;
  int input_mode;
  int status;
} MwmHints;

void noborders(Display *disp, Window win)
{
  MwmHints mwmhints;
  Atom MOTIF_WM_HINTS;
  mwmhints.flags = MWM_HINTS_DECORATIONS;
  mwmhints.decorations = 0; /* MWM_DECOR_BORDER */
  MOTIF_WM_HINTS = XInternAtom(disp, "_MOTIF_WM_HINTS", False);
  XChangeProperty(disp, win, MOTIF_WM_HINTS, MOTIF_WM_HINTS, 32, PropModeReplace, (unsigned char*) &mwmhints, sizeof(mwmhints)/sizeof(long));
}


//========================================================================
int main( ){

	//ofSetupOpenGL(1920,1080, OF_WINDOW);			// <-------- setup the GL context


    ofSetupOpenGL(1280,800,OF_WINDOW);


    ofSleepMillis(1000);
    Display *display;
    display = glXGetCurrentDisplay();
    Window focus_window = None;
    int focus_state;
    XGetInputFocus(display, &focus_window, &focus_state);
    noborders(display,focus_window);
    XMoveWindow(display,focus_window,1280,800);
    ofSetWindowPosition(0,0);


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());


}
