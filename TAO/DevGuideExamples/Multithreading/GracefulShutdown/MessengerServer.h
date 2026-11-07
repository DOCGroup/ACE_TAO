/* -*- C++ -*-  */

#ifndef MESSENGERSERVER_H_
#define MESSENGERSERVER_H_

#include "MessengerS.h"
#include "ace/Task.h"
#include <iostream>
#include <memory>

class MessengerShutdownTimer;

//Class MessengerServer
class MessengerServer
{
public:
  //Constructor
  explicit MessengerServer (CORBA::ORB_ptr orb);

  //Destructor
  ~MessengerServer ();

  MessengerServer (MessengerServer const &) = delete;
  MessengerServer (MessengerServer &&) = delete;
  MessengerServer &operator= (MessengerServer const &) = delete;
  MessengerServer &operator= (MessengerServer &&) = delete;

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

  enum class ShutdownMethod {
    client_call,     // shutdown on client invocation
    polling_loop,    // shutdown after some iterations through loop
    timer,           // schedule a timer to shutdown
    console_input,   // shutdown on console input
    run_time_value   // use CORBA::ORB::run() with time value
  };

  // Task to monitor console input.
  class ConsoleMonitor : public ACE_Task_Base
  {
  public:
    explicit ConsoleMonitor (CORBA::ORB_ptr orb)
      : orb_(CORBA::ORB::_duplicate(orb))
      {
      }

    ~ConsoleMonitor () override = default;

    ConsoleMonitor (ConsoleMonitor const &) = delete;
    ConsoleMonitor (ConsoleMonitor &&) = delete;
    ConsoleMonitor &operator= (ConsoleMonitor const &) = delete;
    ConsoleMonitor &operator= (ConsoleMonitor &&) = delete;

    int svc () override
      {
        char c;
        std::cin.get (c);
        orb_->shutdown (true);
        return 0;
      }
  private:
    CORBA::ORB_var orb_;
  };

private:
  CORBA::ORB_var orb_;
  std::unique_ptr<ConsoleMonitor> monitor_;
  std::unique_ptr<MessengerShutdownTimer> timer_;
};

#endif /* MESSENGERSERVER_H_  */
