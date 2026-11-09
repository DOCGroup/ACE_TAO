
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_FL_SERVER_TEST_I_H
#define TAO_FL_SERVER_TEST_I_H

#include "testS.h"

#include <FL/Fl_Gl_Window.H>
#include <array>

class Simple_Window : public Fl_Gl_Window
{
public:
  /// Constructor
  Simple_Window (int x, int y, int w, int h,
                 char const *label = nullptr);
  Simple_Window (Simple_Window const &) = delete;
  Simple_Window (Simple_Window &&) = delete;
  Simple_Window &operator= (Simple_Window const &) = delete;
  Simple_Window &operator= (Simple_Window &&) = delete;

  void set_x_angle (CORBA::Long x);
  void set_y_angle (CORBA::Long y);

private:
  /// from the Fl_Gl_Window...
  void draw () override;

  void draw_cube ();

  /// The angles...
  CORBA::Long x_angle_ {};
  CORBA::Long y_angle_ {};

  /// The vertices of the cube
  inline static constexpr std::array<std::array<float, 3>, 8> vertices_ {{
    {{-0.5f, -0.5f, -0.5f}}, {{0.5f, -0.5f, -0.5f}},
    {{0.5f, 0.5f, -0.5f}}, {{-0.5f, 0.5f, -0.5f}},
    {{-0.5f, -0.5f, 0.5f}}, {{0.5f, -0.5f, 0.5f}},
    {{0.5f, 0.5f, 0.5f}}, {{-0.5f, 0.5f, 0.5f}}
  }};
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
  Simple_Server_i (CORBA::ORB_ptr orb, Simple_Window *window);
  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  // = The Simple_Server methods.
  void set_x_angle (CORBA::Long x) override;
  void set_y_angle (CORBA::Long y) override;
  void shutdown () override;

private:
  /// The ORB
  CORBA::ORB_var const orb_;

  /// Non-owning pointer to the window displayed by the server.
  Simple_Window * const window_;
};

#endif /* TAO_FL_SERVER_TEST_I_H */
