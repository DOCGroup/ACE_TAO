// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Xt_Stopwatch/Stopwatch_client
//
// = FILENAME
//   Client.h
//
// = AUTHOR
//   Balachandran  Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef CLIENT_H
#define CLIENT_H

#include "testC.h"


#include "tao/XtResource/XtResource_Loader.h"
class Control;

class Client
{
public:

  Client (CORBA::ORB_ptr orb);
  // ctor

  ~Client (void);
  //Dtor..

  static void start_callback (Widget w,
                              XtPointer client_data,
                              XtPointer call_data);
  static void stop_callback  (Widget w,
                              XtPointer client_data,
                              XtPointer call_data);
  // The callbacks

  void start_hook (void);
  void stop_hook (void);
  // The hooks...

  void add_callback (Control &);
  // Adds the callbacks to the GUI underneath.....

  void parse_args (int argc, ACE_TCHAR *argv[]);
private:

  CORBA::ORB_var orb_;
  // The ORB

  Stopwatch_var server_;
  // The server.
};

#endif /* CLIENT_H */
