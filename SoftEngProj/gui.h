#ifndef gui_h
#define gui_h

#include <wx/wx.h>
#include <wx/toolbar.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include "names.h"
#include "devices.h"
#include "monitor.h"

enum { 
  MY_SPINCNTRL_ID = wxID_HIGHEST + 1,
  MY_TEXTCTRL_ID,
  MY_BUTTON_ID,
}; // widget identifiers

class MyGLCanvas;

class MyFrame: public wxFrame
{
 public:
  MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size, 
	  names *names_mod = NULL, devices *devices_mod = NULL, monitor *monitor_mod = NULL, 
	  long style = wxDEFAULT_FRAME_STYLE); // constructor
 private:
  MyGLCanvas *canvas;                     // OpenGL drawing area widget to draw traces
  wxSpinCtrl *spin;                       // control widget to select the number of cycles
  names *nmz;                             // pointer to names class
  devices *dmz;                           // pointer to devices class
  monitor *mmz;                           // pointer to monitor class
  int cyclescompleted;                    // how many simulation cycles have been completed
  void runnetwork(int ncycles);           // function to run the logic network

  // ** CREATE TOOLBAR ** //

  // ** FILE MENU ** // 
  void OnNew(wxCommandEvent& event);      // event handler for NEW item
  void OnOpen(wxCommandEvent& event);     // event handler for OPEN item
  void OnSave(wxCommandEvent& event);     // event handler for SAVE item
  void OnExit(wxCommandEvent& event);     // event handler for EXIT item

  // ** EDIT MENU ** //
  void OnUndo(wxCommandEvent& event);     // event handler of UNDO item
  void OnRedo(wxCommandEvent& event);     // event handler of REDO item
  void OnCut(wxCommandEvent& event);      // event handler of CUT item
  void OnCopy(wxCommandEvent& event);     // event handler of COPY item
  void OnPaste(wxCommandEvent& event);    // event handler of PASTE item

  // ** SIMULATION MENU ** //
  void OnRun(wxCommandEvent& event);      // event handler of RUN item
  void OnPause(wxCommandEvent& event);    // event handler of PAUSE item
  void OnStop(wxCommandEvent& event);     // event handler of STOP item
  void OnTable(wxCommandEvent& event);    // event handler of TABLE item

  // ** HELP MENU ** //
  void OnAbout(wxCommandEvent& event);    // event handler for about menu item
  void OnButton(wxCommandEvent& event);   // event handler for push button


  void OnSpin(wxSpinEvent& event);        // event handler for spin control
  void OnText(wxCommandEvent& event);     // event handler for text entry field
  DECLARE_EVENT_TABLE()
};
    
class MyGLCanvas: public wxGLCanvas
{
 public:
  MyGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, monitor* monitor_mod = NULL, names* names_mod = NULL,
	     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0,
	     const wxString& name = "MyGLCanvas", const wxPalette &palette=wxNullPalette); // constructor
  void Render(wxString example_text = "", int cycles = -1); // function to draw canvas contents
 private:
  wxGLContext *context;              // OpenGL rendering context
  bool init;                         // has the OpenGL context been initialised?
  int cyclesdisplayed;               // how many simulation cycles have been displayed
  monitor *mmz;                      // pointer to monitor class, used to extract signal traces
  names *nmz;                        // pointer to names class, used to extract signal names
  void InitGL();                     // function to initialise OpenGL context
  void OnSize(wxSizeEvent& event);   // event handler for when canvas is resized
  void OnPaint(wxPaintEvent& event); // event handler for when canvas is exposed
  void OnMouse(wxMouseEvent& event); // event handler for mouse events inside canvas
  DECLARE_EVENT_TABLE()
};
    
#endif /* gui_h */
