// $Id$

// Test the event server.

#include "ace/Stream.h"
#include "ace/Service_Config.h"
#include "Options.h"
#include "Consumer_Router.h"
#include "Event_Analyzer.h"
#include "Supplier_Router.h"

typedef ACE_Stream<ACE_MT_SYNCH> MT_Stream;
typedef ACE_Module<ACE_MT_SYNCH> MT_Module;

class Quit_Handler : public ACE_Sig_Adapter
  // = TITLE
  //     Handle SIGINT and terminate the entire application.
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
  Options::instance ()->stop_timer ();
  ACE_DEBUG ((LM_INFO, "(%t) closing down the test\n"));
  Options::instance ()->print_results ();

  ACE_Service_Config::end_reactor_event_loop ();
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_Service_Config daemon;
  
  Options::instance ()->parse_args (argc, argv);
  {
    // Primary ACE_Stream for EVENT_SERVER application.
    MT_Stream event_server; 

    // Enable graceful shutdowns...
    Quit_Handler quit_handler;

    Peer_Router_Context *src;
    // Create the Supplier_Router's routing context, which contains
    // context shared by both the write-side and read-side of the
    // Supplier_Router Module.
    ACE_NEW_RETURN (src, 
		    Peer_Router_Context (Options::instance ()->supplier_port ()),
		    -1);

    MT_Module *srm = 0;
    // Create the Supplier Router module.
    ACE_NEW_RETURN (srm, MT_Module 
		    ("Supplier_Router", 
		     new Supplier_Router (src),
		     new Supplier_Router (src)),
		    -1);

    MT_Module *eam = 0;
    // Create the Event Analyzer module.
    ACE_NEW_RETURN (eam, MT_Module 
		    ("Event_Analyzer", 
		     new Event_Analyzer, 
		     new Event_Analyzer), 
		    -1);

    Peer_Router_Context *crc;
    // Create the Consumer_Router's routing context, which contains
    // context shared by both the write-side and read-side of the
    // Consumer_Router Module.
    ACE_NEW_RETURN (crc, 
		    Peer_Router_Context (Options::instance ()->consumer_port ()),
		    -1);

    MT_Module *crm = 0;
    // Create the Consumer Router module.
    ACE_NEW_RETURN (crm, MT_Module 
		    ("Consumer_Router",
		     new Consumer_Router (crc),
		     new Consumer_Router (crc)),
		    -1);

    // Push the Modules onto the event_server stream.

    if (event_server.push (srm) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push (Supplier_Router)"), -1);
					
    if (event_server.push (eam) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push (Event_Analyzer)"), -1);

    if (event_server.push (crm) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push (Consumer_Router)"), -1);

    // Set the high and low water marks appropriately.

    int wm = Options::instance ()->low_water_mark ();

    if (event_server.control (ACE_IO_Cntl_Msg::SET_LWM, &wm) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "push (setting low watermark)"), -1);

    wm = Options::instance ()->high_water_mark ();
    if (event_server.control (ACE_IO_Cntl_Msg::SET_HWM, &wm) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "push (setting high watermark)"), -1);

    Options::instance ()->start_timer ();

    // Perform the main event loop waiting for the user to type ^C or
    // to enter a line on the ACE_STDIN.

    daemon.run_reactor_event_loop ();
    // The destructor of event_server will close down the stream and
    // call the close() hooks on all the ACE_Tasks.
  }

  // Wait for the threads to exit.
  ACE_Service_Config::thr_mgr ()->wait ();
  ACE_DEBUG ((LM_DEBUG, "exiting main\n"));
  return 0;
}
