// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/FL_Server
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_FL_SERVER_TEST_I_H
#define TAO_FL_SERVER_TEST_I_H

#include "testS.h"

#if defined(ACE_HAS_FL)

#include <FL/Fl_Gl_Window.h>


class Simple_Window : public Fl_Gl_Window
{
public:
  Simple_Window (int x, int y, int w, int h,
                 const char * l = 0);
  // Constructor

  void set_x_angle (CORBA::Long x);
  void set_y_angle (CORBA::Long y);

private:
  virtual void draw (void);
  // from the Fl_Gl_Window...

  virtual void draw_cube (void);

  CORBA::Long x_angle_;
  CORBA::Long y_angle_;
  // The angles...

  float boxv0[3]; float boxv1[3];
  float boxv2[3]; float boxv3[3];
  float boxv4[3]; float boxv5[3];
  float boxv6[3]; float boxv7[3];
  // The vertices of the cube
};

class Simple_Server_i : public POA_Simple_Server
{
  // = TITLE
  //   Simpler Server implementation
  //
  // = DESCRIPTION
  //   Implements the Simple_Server interface in test.idl
  //
public:
  Simple_Server_i (CORBA::ORB_ptr orb,
                   Simple_Window* window);
  // ctor

  // = The Simple_Server methods.
  void set_x_angle (CORBA::Long x ACE_ENV_ARG_DECL_NOT_USED);
  void set_y_angle (CORBA::Long y ACE_ENV_ARG_DECL_NOT_USED);
  void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);

private:
  CORBA::ORB_var orb_;
  // The ORB

  Simple_Window* window_;
  // Display the graphics...
};

#if defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_FL */

#endif /* TAO_FL_SERVER_TEST_I_H */
