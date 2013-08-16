
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_FL_SERVER_TEST_I_H
#define TAO_FL_SERVER_TEST_I_H

#include "testS.h"

#include <FL/Fl_Gl_Window.H>


class Simple_Window : public Fl_Gl_Window
{
public:
  /// Constructor
  Simple_Window (int x, int y, int w, int h,
                 const char * l = 0);

  void set_x_angle (CORBA::Long x);
  void set_y_angle (CORBA::Long y);

private:
  /// from the Fl_Gl_Window...
  virtual void draw (void);

  virtual void draw_cube (void);

  /// The angles...
  CORBA::Long x_angle_;
  CORBA::Long y_angle_;

  /// The vertices of the cube
  float boxv0[3]; float boxv1[3];
  float boxv2[3]; float boxv3[3];
  float boxv4[3]; float boxv5[3];
  float boxv6[3]; float boxv7[3];
};

/**
 * @class Simple_Server_i
 *
 * @brief Simpler Server implementation
 *
 * Implements the Simple_Server interface in test.idl
 */
class Simple_Server_i : public POA_Simple_Server
{
public:
  /// ctor
  Simple_Server_i (CORBA::ORB_ptr orb,
                   Simple_Window* window);

  // = The Simple_Server methods.
  void set_x_angle (CORBA::Long x);
  void set_y_angle (CORBA::Long y);
  void shutdown (void);

private:
  /// The ORB
  CORBA::ORB_var orb_;

  /// Display the graphics...
  Simple_Window* window_;
};

#endif /* TAO_FL_SERVER_TEST_I_H */
