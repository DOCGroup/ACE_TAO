// $Id$

#include "MessengerServer.h"
#include "Messenger_i.h"
#include "MessengerShutdownTimer.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"
#include "tao/ORB_Core.h"
#include <iostream>
#include <fstream>
ACE_TString ior_output_file;

// By default, shutdown when client calls Messenger::shutdown().
MessengerServer::ShutdownMethod s_method = MessengerServer::s_client_call;

int loop_iterations;
int timeout;

// Constructor.
MessengerServer::MessengerServer (CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb))
  , monitor_(0)
{
}

// Destructor.
MessengerServer::~MessengerServer ()
{
  if (monitor_ != 0)
    delete monitor_;
  orb_->destroy ();
}

// run the ORB's event loop continuously
void MessengerServer::run ()
{
  std::cout << "Running the ORB event loop continuously..." << std::endl;
  orb_->run ();
  std::cout << "Finished running the ORB." << std::endl;
}

// run the ORB's event loop for some number of seconds
void MessengerServer::run (int seconds)
{
  std::cout << "Running the ORB event loop for " << seconds
       << " seconds..." << std::endl;
  ACE_Time_Value tv ((long)seconds, 0);
  orb_->run (tv);
  std::cout << "Finished running the ORB." << std::endl;
}

// handle ORB events in a polling loop for some number of iterations
void MessengerServer::poll (int iterations)
{
  std::cout << "Polling for ORB events for " << iterations
       << " iterations..." << std::endl;
  int x = iterations;
  do
  {
    std::cout << "iteration: " << iterations - x << std::endl;
    ACE_Time_Value tv (1,0);
    orb_->perform_work (tv);
  }
  while ((--x > 0) && (orb_->orb_core ()->has_shutdown() == 0));
  std::cout << "Finished running the ORB." << std::endl;
}

// schedule a shutdown timer with the ORB's reactor to timeout
// after some seconds
void MessengerServer::schedule_shutdown_timer (int seconds)
{
  // Create a shutdown timer.
  std::cout << "Creating shutdown timer" << std::endl;
  MessengerShutdownTimer * timer = new MessengerShutdownTimer (orb_.in());

  // Schedule the timer to shutdown the ORB, with no repeat.
  ACE_Time_Value tv ((long)seconds, 0);
  std::cout << "Scheduling shutdown timer" << std::endl;
  orb_->orb_core ()->reactor ()->schedule_timer (
    timer, // handler : ACE_Event_Handler *
    0,     // args : void *
    tv     // relative timeout value : ACE_Time_Value &
    );
}

// spawn thread to monitor console and shutdown on console input
void MessengerServer::shutdown_on_console_input ()
{
  // Spawn a thread to monitor console and shutdown on console input.
  monitor_ = new MessengerServer::ConsoleMonitor (orb_.in());
  monitor_->activate ();
  std::cout << "Enter any input on keyboard to shut down ORB..." << std::endl;
}

// parse arguments
int MessengerServer::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xp:t:cr:o:"));
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'x':
        s_method = MessengerServer::s_client_call;
        break;
      case 'p':
        s_method = MessengerServer::s_polling_loop;
        loop_iterations = ACE_OS::atoi(get_opts.opt_arg());
        break;
      case 't':
        s_method = MessengerServer::s_timer;
        timeout = ACE_OS::atoi(get_opts.opt_arg());
        break;
      case 'c':
        s_method = MessengerServer::s_console_input;
        break;
      case 'o':
        ior_output_file = get_opts.optarg;
        break;
      case 'r':
        s_method = MessengerServer::s_run_time_value;
        timeout = ACE_OS::atoi(get_opts.opt_arg());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s\n"
                           "-x (default) - shutdown on client invocation\n"
                           "-p <n> - use polling loop for <n> iterations\n"
                           "-t <n> - schedule timer for <n> seconds\n"
                           "-o <iorfile>\n"
                           "-c     - shutdown on console input\n"
                           "-r <n> - run ORB for <n> seconds\n",
                           argv[0]),
                           -1);

    }
  }
  return 0;
}


// ------------------------------------------------------------------
// main
// ------------------------------------------------------------------

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    //Get reference to the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

    // Activate the POAManager.
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create a servant.
    Messenger_i messenger_servant (orb.in());

    // Register the servant with the RootPOA, obtain its object
    // reference, stringify it, and write it to a file.
    PortableServer::ObjectId_var oid =
      poa->activate_object( &messenger_servant );
    CORBA::Object_var messenger_obj = poa->id_to_reference( oid.in() );
    CORBA::String_var str = orb->object_to_string( messenger_obj.in() );
    std::ofstream iorFile( "Messenger.ior" );
    iorFile << str.in() << std::endl;
    iorFile.close();
    std::cout << "IOR written to file Messenger.ior" << std::endl;

    // Create a MessengerServer object.
    MessengerServer * server = new MessengerServer (orb.in());

    // Parse arguments to determine how we should shutdown.
    if (server->parse_args (argc, argv) != 0)
      return 1;

    switch (s_method)
    {
      // shutdown on client invocation
      case MessengerServer::s_client_call:
        std::cout << "Will shutdown on client invocation." << std::endl;
        server->run ();
        break;

      // shutdown after some iterations through loop
      case MessengerServer::s_polling_loop:
        server->poll (loop_iterations);
        break;

      // schedule a timer to shutdown
      case MessengerServer::s_timer:
        server->schedule_shutdown_timer (timeout);
        server->run ();
        break;

      // shutdown on console input
      case MessengerServer::s_console_input:
        server->shutdown_on_console_input ();
        server->run ();
        break;

      // use CORBA::ORB::run() with time value
      case MessengerServer::s_run_time_value:
        server->run (timeout);
        break;
    }

    // Finished.
    delete server;

  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "CORBA exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
