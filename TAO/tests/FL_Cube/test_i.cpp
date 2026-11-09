#include "test_i.h"
#include <GL/gl.h>

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb,
                                  Simple_Window *window)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    window_ (window)
{
}

void
Simple_Server_i::set_x_angle (CORBA::Long x)
{
  this->window_->set_x_angle (x);
}

void
Simple_Server_i::set_y_angle (CORBA::Long y)
{
  this->window_->set_y_angle (y);
}

void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}

// ****************************************************************

Simple_Window::Simple_Window (int x, int y,
                              int w, int h,
                              char const *label)
  : Fl_Gl_Window (x, y, w, h, label)
{
}

void
Simple_Window::set_x_angle (CORBA::Long x)
{
  this->x_angle_ = x;
  this->redraw ();
}

void
Simple_Window::set_y_angle (CORBA::Long y)
{
  this->y_angle_ = y;
  this->redraw ();
}

void
Simple_Window::draw ()
{
  // Based on the CubeView example in the FL toolkit.

  if (!this->valid ())
    {
      glLoadIdentity ();
      glViewport (0, 0, this->w (), this->h ());
      glOrtho (-2, 2, -2, 2, -20000, 10000);
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix ();

  glRotatef (this->x_angle_, 0, 1, 0);
  glRotatef (this->y_angle_, 1, 0, 0);

  this->draw_cube ();

  glPopMatrix ();
}

void
Simple_Window::draw_cube ()
{
  constexpr float alpha {0.5f};

  glBegin (GL_QUADS);
  glColor4f (0.0f, 0.0f, 1.0f, alpha);
  glVertex3fv (this->vertices_[0].data ());
  glVertex3fv (this->vertices_[1].data ());
  glVertex3fv (this->vertices_[2].data ());
  glVertex3fv (this->vertices_[3].data ());

  glColor4f (1.0f, 1.0f, 0.0f, alpha);
  glVertex3fv (this->vertices_[0].data ());
  glVertex3fv (this->vertices_[4].data ());
  glVertex3fv (this->vertices_[5].data ());
  glVertex3fv (this->vertices_[1].data ());

  glColor4f (0.0f, 1.0f, 1.0f, alpha);
  glVertex3fv (this->vertices_[2].data ());
  glVertex3fv (this->vertices_[6].data ());
  glVertex3fv (this->vertices_[7].data ());
  glVertex3fv (this->vertices_[3].data ());

  glColor4f (1.0f, 0.0f, 0.0f, alpha);
  glVertex3fv (this->vertices_[4].data ());
  glVertex3fv (this->vertices_[5].data ());
  glVertex3fv (this->vertices_[6].data ());
  glVertex3fv (this->vertices_[7].data ());

  glColor4f (1.0f, 0.0f, 1.0f, alpha);
  glVertex3fv (this->vertices_[0].data ());
  glVertex3fv (this->vertices_[3].data ());
  glVertex3fv (this->vertices_[7].data ());
  glVertex3fv (this->vertices_[4].data ());

  glColor4f (0.0f, 1.0f, 0.0f, alpha);
  glVertex3fv (this->vertices_[1].data ());
  glVertex3fv (this->vertices_[5].data ());
  glVertex3fv (this->vertices_[6].data ());
  glVertex3fv (this->vertices_[2].data ());
  glEnd ();

  glColor3f (1.0f, 1.0f, 1.0f);
  glBegin (GL_LINES);
  glVertex3fv (this->vertices_[0].data ());
  glVertex3fv (this->vertices_[1].data ());

  glVertex3fv (this->vertices_[1].data ());
  glVertex3fv (this->vertices_[2].data ());

  glVertex3fv (this->vertices_[2].data ());
  glVertex3fv (this->vertices_[3].data ());

  glVertex3fv (this->vertices_[3].data ());
  glVertex3fv (this->vertices_[0].data ());

  glVertex3fv (this->vertices_[4].data ());
  glVertex3fv (this->vertices_[5].data ());

  glVertex3fv (this->vertices_[5].data ());
  glVertex3fv (this->vertices_[6].data ());

  glVertex3fv (this->vertices_[6].data ());
  glVertex3fv (this->vertices_[7].data ());
  glVertex3fv (this->vertices_[7].data ());
  glVertex3fv (this->vertices_[4].data ());

  glVertex3fv (this->vertices_[0].data ());
  glVertex3fv (this->vertices_[4].data ());

  glVertex3fv (this->vertices_[1].data ());
  glVertex3fv (this->vertices_[5].data ());

  glVertex3fv (this->vertices_[2].data ());
  glVertex3fv (this->vertices_[6].data ());

  glVertex3fv (this->vertices_[3].data ());
  glVertex3fv (this->vertices_[7].data ());
  glEnd ();
}

