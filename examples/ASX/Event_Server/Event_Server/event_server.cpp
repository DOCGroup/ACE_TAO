// $Id$

// Test the event server.

#include "ace/Log_Msg.h"
#include "ace/Stream.h"
#include "ace/Service_Config.h"
#include "Options.h"
#include "Consumer_Router.h"
#include "Event_Analyzer.h"
#include "Supplier_Router.h"

#if defined (ACE_HAS_THREADS)

typedef ACE_Stream<ACE_MT_SYNCH> MT_Stream;
typedef ACE_Module<ACE_MT_SYNCH> MT_Module;

// Handle SIGINT and terminate the entire application.

class Quit_Handler : public ACE_Sig_Adapter
{
public:
  Quit_Handler (void);
  virtual int handle_input (ACE_HANDLE fd);
};

Quit_Handler::Quit_Handler (void)
  : ACE_Sig_Adapter (ACE_Sig_Handler_Ex (ACE_Service_Config::end_reactor_event_loop))
{  
  // Register to trap input from the user.
  if (ACE::register_stdin_handler (this,
				   ACE_Service_Config::reactor (),
				   ACE_Service_Config::thr_mgr ()) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_stdin_handler"));
  // Register to trap the SIGINT signal.
  else if (ACE_Service_Config::reactor ()->register_handler 
	   (SIGINT, this) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_handler"));
}

int
Quit_Handler::handle_input (ACE_HANDLE)
{
  options.stop_timer ();
  ACE_DEBUG ((LM_INFO, "(%t) closing down the test\n"));
  options.print_results ();

  ACE_Service_Config::end_reactor_event_loop ();
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_Service_Config daemon;
  
  options.parse_args (argc, argv);

  {
    // Primary ACE_Stream for EVENT_SERVER application.
    MT_Stream event_server; 

    // Enable graceful shutdowns...
    Quit_Handler quit_handler;

    // Create the Supplier Router module.

    MT_Module *sr = new MT_Module ("Supplier_Router", 
				   new Supplier_Router (ACE_Service_Config::thr_mgr ()));

    // Create the Event Analyzer module.

    MT_Module *ea = new MT_Module ("Event_Analyzer", 
				   new Event_Analyzer, 
				   new Event_Analyzer);

    // Create the Consumer Router module.

    MT_Module *cr = new MT_Module ("Consumer_Router", 
				   0, // 0 triggers the creation of a ACE_Thru_Task...
				   new Consumer_Router (ACE_Service_Config::thr_mgr ()));

    // Push the Modules onto the event_server stream.

    if (event_server.push (sr) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push (Supplier_Router)"), -1);
					
    if (event_server.push (ea) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push (Event_Analyzer)"), -1);

    if (event_server.push (cr) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push (Consumer_Router)"), -1);

    // Set the high and low water marks appropriately.

    int wm = options.low_water_mark ();

    if (event_server.control (ACE_IO_Cntl_Msg::SET_LWM, &wm) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "push (setting low watermark)"), -1);

    wm = options.high_water_mark ();
    if (event_server.control (ACE_IO_Cntl_Msg::SET_HWM, &wm) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "push (setting high watermark)"), -1);

    options.start_timer ();

    // Perform the main event loop waiting for the user to type ^C or to
    // enter a line on the ACE_STDIN.

    daemon.run_reactor_event_loop ();
    // The destructor of event_server will close down the stream and
    // call the close() hooks on all the ACE_Tasks.
  }

  // Wait for the threads to exit.
  ACE_Service_Config::thr_mgr ()->wait ();
  ACE_DEBUG ((LM_DEBUG, "exiting main\n"));
  return 0;
}
#else
int 
main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "test not defined for this platform\n"), -1);
}
#endif /* ACE_HAS_THREADS */
