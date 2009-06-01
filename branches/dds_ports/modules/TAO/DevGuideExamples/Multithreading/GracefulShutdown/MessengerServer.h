/* -*- C++ -*- $Id$ */

#ifndef MESSENGERSERVER_H_
#define MESSENGERSERVER_H_

#include "MessengerS.h"
#include "ace/Task.h"
#include <iostream>

//Class MessengerServer
class MessengerServer
{
public:
  //Constructor
  MessengerServer (CORBA::ORB_ptr orb);

  //Destructor
  virtual ~MessengerServer (void);

  // parse arguments
  int parse_args (int argc, ACE_TCHAR* argv[]);

  // run the ORB's event loop continuously
  void run ();

  // run the ORB's event loop for some number of seconds
  void run (int seconds);

  // handle ORB events in a polling loop for some number of iterations
  void poll (int iterations);

  // schedule a shutdown timer with the ORB's reactor to timeout
  // after some seconds
  void schedule_shutdown_timer (int seconds);

  // spawn thread to monitor console and shutdown on console input
  void shutdown_on_console_input ();

  enum ShutdownMethod {
    s_client_call,     // shutdown on client invocation
    s_polling_loop,    // shutdown after some iterations through loop
    s_timer,           // schedule a timer to shutdown
    s_console_input,   // shutdown on console input
    s_run_time_value   // use CORBA::ORB::run() with time value
  };

  // Task to monitor console input.
  class ConsoleMonitor : public ACE_Task_Base
  {
  public:
    ConsoleMonitor (CORBA::ORB_ptr orb)
      : orb_(CORBA::ORB::_duplicate(orb))
      {
      }

    virtual int svc()
      {
        char c;
        std::cin.get (c);
        orb_->shutdown (1);
        return 0;
      }
  private:
    CORBA::ORB_var orb_;
  };

private:

  CORBA::ORB_var orb_;
  ConsoleMonitor * monitor_;

};

#endif /* MESSENGERSERVER_H_  */
