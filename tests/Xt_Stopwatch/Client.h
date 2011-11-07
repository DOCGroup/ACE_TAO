
//=============================================================================
/**
 *  @file   Client.h
 *
 *  $Id$
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
  Client (CORBA::ORB_ptr orb);

  ///Dtor..
  ~Client (void);

  /// The callbacks
  static void start_callback (Widget w,
                              XtPointer client_data,
                              XtPointer call_data);
  static void stop_callback  (Widget w,
                              XtPointer client_data,
                              XtPointer call_data);

  /// The hooks...
  void start_hook (void);
  void stop_hook (void);

  /// Adds the callbacks to the GUI underneath.....
  void add_callback (Control &);

  void parse_args (int argc, ACE_TCHAR *argv[]);
private:

  /// The ORB
  CORBA::ORB_var orb_;

  /// The server.
  Stopwatch_var server_;
};

#endif /* CLIENT_H */
