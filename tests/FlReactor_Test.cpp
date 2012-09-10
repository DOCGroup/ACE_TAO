/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   FlReactor_Test.cpp
 *
 *  $Id$
 *
 * A simple test that ilustrates the integration of the fast-light
 * toolkit (http://fltk.easysw.org/) with ACE, it uses FL to create
 * an OpenGL window and display a polygon, it uses ACE to open an
 * acceptor. Every time there is a connection the number of polygons
 * is increased, a little widget can be used to change the number of
 * polygons too.
 *
 *
 *  @author Carlos O'Ryan <coryan@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"




#include "ace/FlReactor/FlReactor.h"
#include "ace/Event_Handler.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"

#include /**/ <FL/Fl.H>
#include /**/ <FL/Fl_Window.H>
#include /**/ <FL/Fl_Hor_Slider.H>
#include /**/ <FL/Fl_Box.H>
#include /**/ <FL/math.h>
#include /**/ <FL/gl.h>
#include /**/ <FL/Fl_Gl_Window.H>

class Test_Window : public Fl_Gl_Window
{
public:
  /// Constructor
  Test_Window (int x, int y, int w, int h,
               const char * l = 0);

  int sides (void) const;
  void sides (int s);
  void incr_sides (void);

private:
  /// from the Fl_Gl_Window...
  virtual void draw (void);

  int sides_;
};

Test_Window::Test_Window (int x, int y,
                      int w, int h,
                      const char* l)
  :  Fl_Gl_Window (x, y, w, h, l),
     sides_ (3)
{
}

int
Test_Window::sides (void) const
{
  return this->sides_;
}

void
Test_Window::sides (int s)
{
  this->sides_ = s;
  this->redraw ();
}

void
Test_Window::incr_sides (void)
{
  this->sides_++;
  if (this->sides_ > 10)
    this->sides_ = 3;
  this->redraw ();
}

void
Test_Window::draw (void)
{
  // the valid() property may be used to avoid reinitializing your
  // GL transformation for each redraw:
  if (!this->valid ())
    {
      this->valid (1);
      glLoadIdentity ();
      glViewport (0, 0, this->w (), this->h ());
    }
  // draw an amazing but slow graphic:
  glClear (GL_COLOR_BUFFER_BIT);

  glBegin (GL_POLYGON);
  int s = this->sides_;

  for (int i = 0; i != s; ++i)
    {
      double ang = i * 2 * M_PI / s;
      glColor3f (float (i) / s,
                 float (i) / s,
                 float (i) / s);
      glVertex3f (cos (ang), sin (ang), 0);
    }
  glEnd ();
}

// when you change the data, as in this callback, you must call redraw ():
void sides_cb (Fl_Widget *o, void *p)
{
  Test_Window *tw =
    reinterpret_cast<Test_Window *> (p);
  Fl_Slider *slider =
    dynamic_cast<Fl_Slider*> (o);
  tw->sides (static_cast<int> (slider->value ()));
}

class Connection_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  Connection_Handler (Test_Window *w = 0,
                      Fl_Box* box = 0);

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void *);
  //FUZZ: enble check_for_lack_ACE_OS

  virtual int handle_input (ACE_HANDLE);

private:
  Test_Window *w_;
  Fl_Box *box_;
};

class Acceptor : public ACE_Acceptor<Connection_Handler,ACE_SOCK_ACCEPTOR>
{
public:
  Acceptor (Test_Window *w = 0,
            Fl_Box *box = 0);

  virtual int make_svc_handler (Connection_Handler *&sh);

private:
  Test_Window* w_;
  Fl_Box *box_;
};

Connection_Handler::Connection_Handler (Test_Window *w,
                                        Fl_Box *box)
  :  w_ (w),
     box_ (box)
{
}

int
Connection_Handler::open (void*)
{
  if (this->box_ != 0)
    {
      ACE_INET_Addr from;

      this->peer ().get_remote_addr (from);
      const int bufsiz = 128;
      char buf[bufsiz];

      from.addr_to_string (buf, bufsiz, 0);

      static char msg[256];
      ACE_OS::sprintf (msg, "connection from <%s>\n", buf);

      this->box_->label (msg);
      this->box_->redraw ();
    }

  if (this->w_ != 0)
    {
      this->w_->incr_sides ();
    }

  return this->peer ().enable (ACE_NONBLOCK);
}

int
Connection_Handler::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];
  if (this->peer ().recv (buf, BUFSIZ) <= 0)
    return -1;
  return 0;
}

Acceptor::Acceptor (Test_Window *w, Fl_Box *box)
  :  w_ (w),
     box_ (box)
{
}

int
Acceptor::make_svc_handler (Connection_Handler *&sh)
{
  if (sh == 0)
    {
      ACE_NEW_RETURN (sh, Connection_Handler (this->w_, this->box_), -1);
      sh->reactor (this->reactor());
    }
  return 0;
}

int run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("FlReactor_Test"));

  Fl_Window window (300, 370);

  Test_Window tw (10, 75, window.w () - 20, window.h ()-90);
  window.resizable (&tw);

  Fl_Hor_Slider slider (60, 5, window.w () - 70, 30, "Sides:");
  slider.align (FL_ALIGN_LEFT);
  slider.callback (sides_cb, &tw);
  slider.value (tw.sides ());
  slider.step (1);
  slider.bounds (3, 10);

  ACE_FlReactor reactor;
  ACE_Reactor r (&reactor);

  Fl_Box *box = new Fl_Box (FL_UP_BOX, 10, 40,
                            window.w () - 20, 30,
                            "Setting up");
  box->labelfont (FL_BOLD);

  Acceptor acceptor (&tw, box);

  ACE_INET_Addr address;

  if (acceptor.open (address, &r) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open acceptor"),
                      -1);

  acceptor.acceptor ().get_local_addr (address);

  const int bufsiz = 128;
  char buf[bufsiz];

  address.addr_to_string (buf, bufsiz, 0);

  char msg[2 * bufsiz];
  ACE_OS::sprintf (msg, "Listening on <%s>\n", buf);

  box->label (msg);
  box->redraw ();

  window.end ();
  window.show (argc, argv);
  tw.show ();

  int const retval = Fl::run ();

  ACE_END_TEST;

  return retval;
}

