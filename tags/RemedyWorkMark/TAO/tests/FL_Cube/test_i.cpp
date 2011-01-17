// $Id$

#include "test_i.h"
#include <GL/gl.h>

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb,
                                  Simple_Window *window)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
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
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

// ****************************************************************

Simple_Window::Simple_Window (int x, int y,
                              int w, int h,
                              const char* l)
  :  Fl_Gl_Window (x, y, w, h, l),
     x_angle_ (0),
     y_angle_ (0)
{
    boxv0[0] = -0.5; boxv0[1] = -0.5; boxv0[2] = -0.5;
    boxv1[0] =  0.5; boxv1[1] = -0.5; boxv1[2] = -0.5;
    boxv2[0] =  0.5; boxv2[1] =  0.5; boxv2[2] = -0.5;
    boxv3[0] = -0.5; boxv3[1] =  0.5; boxv3[2] = -0.5;
    boxv4[0] = -0.5; boxv4[1] = -0.5; boxv4[2] =  0.5;
    boxv5[0] =  0.5; boxv5[1] = -0.5; boxv5[2] =  0.5;
    boxv6[0] =  0.5; boxv6[1] =  0.5; boxv6[2] =  0.5;
    boxv7[0] = -0.5; boxv7[1] =  0.5; boxv7[2] =  0.5;
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
Simple_Window::draw (void)
{
  // Based on the CubeView example in the FL toolkit.

  if (!this->valid())
    {
      glLoadIdentity();
      glViewport(0, 0, this->w(), this->h());
      glOrtho(-2,2,-2,2,-20000,10000);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glRotatef(this->x_angle_, 0, 1, 0);
    glRotatef(this->y_angle_, 1, 0, 0);

    this->draw_cube ();

    glPopMatrix ();
}

void
Simple_Window::draw_cube (void)
{
  const float ALPHA = 0.5;

    glBegin(GL_QUADS);
      glColor4f(0.0, 0.0, 1.0, ALPHA);
      glVertex3fv(boxv0);
      glVertex3fv(boxv1);
      glVertex3fv(boxv2);
      glVertex3fv(boxv3);

      glColor4f(1.0, 1.0, 0.0, ALPHA);
      glVertex3fv(boxv0);
      glVertex3fv(boxv4);
      glVertex3fv(boxv5);
      glVertex3fv(boxv1);

      glColor4f(0.0, 1.0, 1.0, ALPHA);
      glVertex3fv(boxv2);
      glVertex3fv(boxv6);
      glVertex3fv(boxv7);
      glVertex3fv(boxv3);

      glColor4f(1.0, 0.0, 0.0, ALPHA);
      glVertex3fv(boxv4);
      glVertex3fv(boxv5);
      glVertex3fv(boxv6);
      glVertex3fv(boxv7);

      glColor4f(1.0, 0.0, 1.0, ALPHA);
      glVertex3fv(boxv0);
      glVertex3fv(boxv3);
      glVertex3fv(boxv7);
      glVertex3fv(boxv4);

      glColor4f(0.0, 1.0, 0.0, ALPHA);
      glVertex3fv(boxv1);
      glVertex3fv(boxv5);
      glVertex3fv(boxv6);
      glVertex3fv(boxv2);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
      glVertex3fv(boxv0);
      glVertex3fv(boxv1);

      glVertex3fv(boxv1);
      glVertex3fv(boxv2);

      glVertex3fv(boxv2);
      glVertex3fv(boxv3);

      glVertex3fv(boxv3);
      glVertex3fv(boxv0);

      glVertex3fv(boxv4);
      glVertex3fv(boxv5);

      glVertex3fv(boxv5);
      glVertex3fv(boxv6);

      glVertex3fv(boxv6);
      glVertex3fv(boxv7);
      glVertex3fv(boxv7);
      glVertex3fv(boxv4);

      glVertex3fv(boxv0);
      glVertex3fv(boxv4);

      glVertex3fv(boxv1);
      glVertex3fv(boxv5);

      glVertex3fv(boxv2);
      glVertex3fv(boxv6);

      glVertex3fv(boxv3);
      glVertex3fv(boxv7);
    glEnd();
}

