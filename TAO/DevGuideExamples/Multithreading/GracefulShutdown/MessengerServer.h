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
  struct Options
  {
    enum class ShutdownMethod
    {
      client_call,
      polling_loop,
      timer,
      console_input,
      run_time_value
    };

    ShutdownMethod shutdown_method {ShutdownMethod::client_call};
    ACE_TString ior_output_file {ACE_TEXT ("test.ior")};
    int loop_iterations {};
    int timeout {};
  };

  //Constructor
  explicit MessengerServer (CORBA::ORB_ptr orb);

  //Destructor
  ~MessengerServer ();

  MessengerServer (MessengerServer const &) = delete;
  MessengerServer (MessengerServer &&) = delete;
  MessengerServer &operator= (MessengerServer const &) = delete;
  MessengerServer &operator= (MessengerServer &&) = delete;

  static int parse_args (int argc, ACE_TCHAR *argv[], Options &options);

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
