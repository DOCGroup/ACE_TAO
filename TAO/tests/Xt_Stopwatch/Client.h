
//=============================================================================
/**
 *  @file   Client.h
 *
 *  @author Balachandran  Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================
#ifndef CLIENT_H
#define CLIENT_H

#include "testC.h"

#include "tao/XtResource/XtResource_Loader.h"
class Control;

class Client
{
public:
  /// ctor
  explicit Client (CORBA::ORB_ptr orb);

  ~Client () = default;

  Client (Client const &) = delete;
  Client &operator= (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client &&) = delete;

  /// The callbacks
  static void start_callback (Widget w,
                              XtPointer client_data,
                              XtPointer call_data);
  static void stop_callback  (Widget w,
                              XtPointer client_data,
                              XtPointer call_data);

  /// The hooks...
  void start_hook ();
  void stop_hook ();

  /// Adds the callbacks to the GUI underneath.....
  void add_callback (Control const &ctrl);

  void parse_args (int argc, ACE_TCHAR *argv[]);

private:
  /// The ORB
  CORBA::ORB_var orb_ {};

  /// The server.
  Stopwatch_var server_ {};
};

#endif /* CLIENT_H */
