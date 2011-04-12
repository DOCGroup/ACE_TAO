// $Id$

#ifndef _CLIENT_
#define _CLIENT_

#include "TestC.h"

#include "ace/Time_Value.h"

#include <string>

class Client
{
 public:
  Client (int argc, ACE_TCHAR* argv[]);
  ~Client ();

  bool run ();

 private:
  bool init (int argc, ACE_TCHAR* argv[]);
  bool parse_args (int argc, ACE_TCHAR* argv[]);

  enum Flushing_Strategy
  {
    LF,
    BLOCKING,
    REACTIVE
  };

  bool init_;
  bool one_way_test_;
  CORBA::ORB_var orb_;
  Test_var test_obj_;
  Test_var management_;
  Flushing_Strategy flush_strategy_;

  Test_var test_obj_none_timeout_;
  Test_var test_obj_eager_timeout_;
  Test_var test_obj_delayed_timeout_;
  Test_var test_obj_transport_timeout_;
  Test_var test_obj_server_timeout_;
  Test_var test_obj_target_timeout_;

  // flood up connection for 'tv' time.
  //  if tv is 0, remember to unsleep server.
  bool flood_connection (ACE_Time_Value& tv);
  bool unplug_transport ();
  bool test_oneway_timeout (bool flood);
};

#endif //_CLIENT_
