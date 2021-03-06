#include "gui.h"
#include <GL/glut.h>
#include <wx/toolbar.h>
#include "wx_icon.xpm"
#include <iostream>

using namespace std;

// MyGLCanvas ////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
  EVT_SIZE(MyGLCanvas::OnSize)
  EVT_PAINT(MyGLCanvas::OnPaint)
  EVT_MOUSE_EVENTS(MyGLCanvas::OnMouse)
END_EVENT_TABLE()
  
int wxglcanvas_attrib_list[5] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

MyGLCanvas::MyGLCanvas(wxWindow *parent, wxWindowID id, monitor* monitor_mod, names* names_mod, const wxPoint& pos, 
		       const wxSize& size, long style, const wxString& name, const wxPalette& palette):
  wxGLCanvas(parent, id, wxglcanvas_attrib_list, pos, size, style, name, palette)
  // Constructor - initialises private variables
{
  context = new wxGLContext(this);
  mmz = monitor_mod;
  nmz = names_mod;
  init = false;
  cyclesdisplayed = -1;
}

void MyGLCanvas::Render(wxString example_text, int cycles)
  // Draws canvas contents - the following example writes the string "example text" onto the canvas
  // and draws a signal trace. The trace is artificial if the simulator has not yet been run.
  // When the simulator is run, the number of cycles is passed as a parameter and the first monitor
  // trace is displayed.
{
  float y;
  unsigned int i;
  asignal s;

  if (cycles >= 0) cyclesdisplayed = cycles;

  SetCurrent(*context);
  if (!init) {
    InitGL();
    init = true;
  }
  glClear(GL_COLOR_BUFFER_BIT);

  if ((cyclesdisplayed >= 0) && (mmz->moncount() > 0)) { // draw the first monitor signal, get trace from monitor class

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (i=0; i<cyclesdisplayed; i++) {
      if (mmz->getsignaltrace(0, i, s)) {
	if (s==low) y = 10.0;
	if (s==high) y = 30.0;
	glVertex2f(20*i+10.0, y); 
	glVertex2f(20*i+30.0, y);
      }
    }
    glEnd();

  } else { // draw an artificial trace

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (i=0; i<5; i++) {
      if (i%2) y = 10.0;
      else y = 30.0;
      glVertex2f(20*i+10.0, y); 
      glVertex2f(20*i+30.0, y);
    }
    glEnd();
    
  }

  // Example of how to use GLUT to draw text on the canvas
  glColor3f(0.0, 0.0, 1.0);
  glRasterPos2f(10, 100);
  for (i = 0; i < example_text.Len(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, example_text[i]);

  // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
  glFlush();
  SwapBuffers();
}

void MyGLCanvas::InitGL()
  // Function to initialise the GL context
{
  int w, h;

  GetClientSize(&w, &h);
  SetCurrent(*context);
  glDrawBuffer(GL_BACK);
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glViewport(0, 0, (GLint) w, (GLint) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1); 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void MyGLCanvas::OnPaint(wxPaintEvent& event)
  // Event handler for when the canvas is exposed
{
  int w, h;
  wxString text;

  wxPaintDC dc(this); // required for correct refreshing under MS windows
  GetClientSize(&w, &h);
  text.Printf("Canvas redrawn by OnPaint event handler, canvas size is %d by %d", w, h);
  Render(text);
}

void MyGLCanvas::OnSize(wxSizeEvent& event)
  // Event handler for when the canvas is resized
{
  init = false;; // this will force the viewport and projection matrices to be reconfigured on the next paint
}

void MyGLCanvas::OnMouse(wxMouseEvent& event)
  // Event handler for mouse events inside the GL canvas
{
  wxString text;
  int w, h;;

  GetClientSize(&w, &h);
  if (event.ButtonDown()) text.Printf("Mouse button %d pressed at %d %d", event.GetButton(), event.m_x, h-event.m_y);
  if (event.ButtonUp()) text.Printf("Mouse button %d released at %d %d", event.GetButton(), event.m_x, h-event.m_y);
  if (event.Dragging()) text.Printf("Mouse dragged to %d %d", event.m_x, h-event.m_y);
  if (event.Leaving()) text.Printf("Mouse left window at %d %d", event.m_x, h-event.m_y);

  if (event.ButtonDown() || event.ButtonUp() || event.Dragging() || event.Leaving()) Render(text);
}

// MyFrame ///////////////////////////////////////////////////////////////////////////////////////


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  // FILE
  EVT_MENU(wxID_NEW, MyFrame::OnNew)
  EVT_MENU(wxID_OPEN, MyFrame::OnOpen)
  EVT_MENU(wxID_SAVE, MyFrame::OnSave)
  EVT_MENU(wxID_EXIT, MyFrame::OnExit)

  // EDIT
  EVT_MENU(wxID_CUT, MyFrame::OnCut)
  EVT_MENU(wxID_COPY, MyFrame::OnCopy)
  EVT_MENU(wxID_PASTE, MyFrame::OnPaste)
  EVT_MENU(wxID_UNDO, MyFrame::OnUndo)
  EVT_MENU(wxID_REDO, MyFrame::OnRedo)

  // SIMULATE
  EVT_MENU(wxID_FORWARD, MyFrame::OnRun)
  EVT_MENU(wxID_STATIC, MyFrame::OnPause)
  EVT_MENU(wxID_STOP, MyFrame::OnStop)
  EVT_MENU(wxID_VIEW_LIST, MyFrame::OnTable)

  // HELP
  EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)

  // OTHERS
  EVT_BUTTON(MY_BUTTON_ID, MyFrame::OnButton)
  EVT_SPINCTRL(MY_SPINCNTRL_ID, MyFrame::OnSpin)
  EVT_TEXT_ENTER(MY_TEXTCTRL_ID, MyFrame::OnText)
END_EVENT_TABLE()
  
MyFrame::MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size,
		 names *names_mod, devices *devices_mod, monitor *monitor_mod, long style):
  wxFrame(parent, wxID_ANY, title, pos, size, style)
  // Constructor - initialises pointers to names, devices and monitor classes, lays out widgets
  // using sizers
{
  SetIcon(wxIcon(wx_icon));

  nmz = names_mod;
  dmz = devices_mod;
  mmz = monitor_mod;
  if (nmz == NULL || dmz == NULL || mmz == NULL) {
    cout << "Cannot operate GUI without names, devices and monitor classes" << endl;
    exit(1);
  }

  // ** MENU ** //
  // FILE 
  wxMenu *fileMenu = new wxMenu;
  fileMenu->Append(wxID_NEW, "&New");
  fileMenu->Append(wxID_OPEN, "&Open");
  fileMenu->Append(wxID_SAVE, "&Save");
  fileMenu->Append(wxID_EXIT, "&Quit");
  
  // EDIT
  wxMenu *editMenu = new wxMenu;
  editMenu->Append(wxID_CUT, "&Cut");
  editMenu->Append(wxID_COPY, "&Copy");
  editMenu->Append(wxID_PASTE, "&Paste");
  editMenu->Append(wxID_UNDO, "&Undo");
  editMenu->Append(wxID_REDO, "&Redo");

  // SIMULATION
  wxMenu *simMenu = new wxMenu;
  simMenu->Append(wxID_FORWARD, "&Run");
  simMenu->Append(wxID_STATIC, "&Pause");
  simMenu->Append(wxID_STOP, "&Stop");
  simMenu->Append(wxID_VIEW_LIST, "&Table of States");

  // HELP
  wxMenu *helpMenu = new wxMenu;
  helpMenu->Append(wxID_ABOUT, "&About");

  // Draw the menuBar
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(fileMenu, "&File");
  menuBar->Append(editMenu, "&Edit");
  menuBar->Append(simMenu, "&Simulate");
  menuBar->Append(helpMenu, "&Help");
  SetMenuBar(menuBar);

  // ** TOOLBAR ** //
  // wxBitmap new(wxt("new.png"), wxBITMAP_TYPE_PNG);
  // wxToolBar *toolbar = CreateToolBar();
  // toolbar->AddTool(wxID_NEW, new, wxT("New"));
  // toolbar->Realize();

  Connect(wxID_NEW, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnNew));
  Centre();
  
  wxBoxSizer *topsizer = new wxBoxSizer(wxHORIZONTAL);
  canvas = new MyGLCanvas(this, wxID_ANY, monitor_mod, names_mod);
  topsizer->Add(canvas, 1, wxEXPAND | wxALL, 10);

  wxBoxSizer *button_sizer = new wxBoxSizer(wxVERTICAL);
  button_sizer->Add(new wxButton(this, MY_BUTTON_ID, "Run"), 0, wxALL, 10);
  button_sizer->Add(new wxStaticText(this, wxID_ANY, "Cycles"), 0, wxTOP|wxLEFT|wxRIGHT, 10);
  spin = new wxSpinCtrl(this, MY_SPINCNTRL_ID, wxString("10"));
  button_sizer->Add(spin, 0 , wxALL, 10);

  button_sizer->Add(new wxTextCtrl(this, MY_TEXTCTRL_ID, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 0 , wxALL, 10);
  topsizer->Add(button_sizer, 0, wxALIGN_CENTER);

  SetSizeHints(400, 400);
  SetSizer(topsizer);
}

// ** FILE MENU ** //

void MyFrame::OnNew(wxCommandEvent &event)
{

}

void MyFrame::OnOpen(wxCommandEvent &event)
{

}

void MyFrame::OnSave(wxCommandEvent &event)
{

}

void MyFrame::OnExit(wxCommandEvent &event)
  // Event handler for the exit menu item
{
  Close(true);
}

// ** EDIT MENU ** //

void MyFrame::OnCut(wxCommandEvent &event)
{

}

void MyFrame::OnCopy(wxCommandEvent &event)
{

}

void MyFrame::OnPaste(wxCommandEvent &event)
{

}

void MyFrame::OnUndo(wxCommandEvent &event)
{

}

void MyFrame::OnRedo(wxCommandEvent &event)
{

}

// ** SIMULATE MENU ** //
void MyFrame::OnRun(wxCommandEvent &event)
{

}

void MyFrame::OnPause(wxCommandEvent &event)
{

}

void MyFrame::OnStop(wxCommandEvent &event)
{

}

void MyFrame::OnTable(wxCommandEvent &event)
{

}

// ** HELP MENU ** //

void MyFrame::OnAbout(wxCommandEvent &event)
  // Event handler for the about menu item
{
  wxMessageDialog about(this, "E. Dakin, K. Kyriakopolous and Q. Ha\nEaster 2015\nUsing NYAYA language.", "Logic Simulator LYCEON", wxICON_INFORMATION | wxOK);
  about.ShowModal();
}

void MyFrame::OnButton(wxCommandEvent &event)
  // Event handler for the push button
{
  int n, ncycles;

  cyclescompleted = 0;
  mmz->resetmonitor ();
  runnetwork(spin->GetValue());
  canvas->Render("Run button pressed", cyclescompleted);
}

void MyFrame::OnSpin(wxSpinEvent &event)
  // Event handler for the spin control
{
  wxString text;

  text.Printf("New spinctrl value %d", event.GetPosition());
  canvas->Render(text);
}

void MyFrame::OnText(wxCommandEvent &event)
  // Event handler for the text entry field
{
  wxString text;

  text.Printf("New text entered %s", event.GetString().c_str());
  canvas->Render(text);
}

void MyFrame::runnetwork(int ncycles)
  // Function to run the network, derived from corresponding function in userint.cc
{
  bool ok = true;
  int n = ncycles;

  while ((n > 0) && ok) {
    dmz->executedevices (ok);
    if (ok) {
      n--;
      mmz->recordsignals ();
    } else
      cout << "Error: network is oscillating" << endl;
  }
  if (ok) cyclescompleted = cyclescompleted + ncycles;
  else cyclescompleted = 0;
}
