// $Id$

// Main driver program for the event server example.

#include "ace/OS_main.h"
#include "ace/Service_Config.h"
#include "ace/OS_NS_unistd.h"
#include "Options.h"
#include "Consumer_Router.h"
#include "Event_Analyzer.h"
#include "Supplier_Router.h"
#include "ace/Sig_Adapter.h"
#include "ace/Stream.h"

// Typedef these components to handle multi-threading correctly.
typedef ACE_Stream<ACE_SYNCH> MT_Stream;
typedef ACE_Module<ACE_SYNCH> MT_Module;


class Event_Server : public ACE_Sig_Adapter
{
  // = TITLE
  //     Run the logic for the <Event_Server>.

  //
  // = DESCRIPTION
  //     In addition to packaging the <Event_Server> components, this
  //     class also handles SIGINT and terminate the entire
  //     application process.  There are several ways to terminate
  //     this application process:
  //
  //     1. Send a SIGINT signal (e.g., via ^C)
  //     2. Type any character on the STDIN.
  //
  //     Note that by inheriting from the <ACE_Sig_Adapter> we can
  //     shutdown the <ACE_Reactor> cleanly when a SIGINT is
  //     generated.
public:
  Event_Server (void);
  // Constructor.

  int svc (void);
  // Run the event-loop for the event server.

private:
  virtual int handle_input (ACE_HANDLE handle);
  // Hook method called back when a user types something into the
  // STDIN in order to shut down the program.

  int configure_stream (void);
  // Setup the plumbing in the stream.

  int set_watermarks (void);
  // Set the high and low queue watermarks.

  int run_event_loop (void);
  // Run the event-loop for the <Event_Server>.

  MT_Stream event_server_;
  // The <ACE_Stream> that contains the <Event_Server> application
  // <Modules>.
};

Event_Server::Event_Server (void)
  : ACE_Sig_Adapter (ACE_Sig_Handler_Ex (ACE_Reactor::end_event_loop))
  // Shutdown the <ACE_Reactor>'s event loop when a SIGINT is
  // received.
{
  // Register to trap STDIN from the user.
  if (ACE_Event_Handler::register_stdin_handler (this,
                                                 ACE_Reactor::instance (),
                                                 ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("register_stdin_handler")));
  // Register to trap the SIGINT signal.
  else if (ACE_Reactor::instance ()->register_handler
           (SIGINT, this) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("register_handler")));
}

int
Event_Server::handle_input (ACE_HANDLE)
{
  // This code here will make sure we actually wait for the user to
  // type something. On platforms like Win32, <handle_input> is called
  // prematurely (even when there is no data).
  char temp_buffer [BUFSIZ];

  ssize_t n = ACE_OS::read (ACE_STDIN,
                            temp_buffer,
                            sizeof (temp_buffer));
  // This ought to be > 0, otherwise something very strange has
  // happened!!
  ACE_ASSERT (n > 0);
  ACE_UNUSED_ARG (n); // To avoid compile warning with ACE_NDEBUG.

  Options::instance ()->stop_timer ();

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%t) closing down the test\n")));
  Options::instance ()->print_results ();

  ACE_Reactor::instance ()->end_reactor_event_loop ();
  return -1;
}

int
Event_Server::configure_stream (void)
{
  Peer_Router_Context *src;
  // Create the <Supplier_Router>'s routing context.  This contains a
  // context shared by both the write-side and read-side of the
  // <Supplier_Router> Module.
  ACE_NEW_RETURN (src,
                  Peer_Router_Context (Options::instance ()->supplier_port ()),
                  -1);

  MT_Module *srm = 0;
  // Create the <Supplier_Router> module.
  ACE_NEW_RETURN (srm,
                  MT_Module
                  (ACE_TEXT ("Supplier_Router"),
                   new Supplier_Router (src),
                   new Supplier_Router (src)),
                  -1);

  MT_Module *eam = 0;
  // Create the <Event_Analyzer> module.
  ACE_NEW_RETURN (eam,
                  MT_Module
                  (ACE_TEXT ("Event_Analyzer"),
                   new Event_Analyzer,
                   new Event_Analyzer),
                  -1);

  Peer_Router_Context *crc;
  // Create the <Consumer_Router>'s routing context.  This contains a
  // context shared by both the write-side and read-side of the
  // <Consumer_Router> Module.
  ACE_NEW_RETURN (crc,
                  Peer_Router_Context (Options::instance ()->consumer_port ()),
                  -1);

  MT_Module *crm = 0;
  // Create the <Consumer_Router> module.
  ACE_NEW_RETURN (crm,
                  MT_Module
                  (ACE_TEXT ("Consumer_Router"),
                   new Consumer_Router (crc),
                   new Consumer_Router (crc)),
                  -1);

  // Push the Modules onto the event_server stream.

  if (this->event_server_.push (srm) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push (Supplier_Router)")),
                      -1);
  else if (this->event_server_.push (eam) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push (Event_Analyzer)")),
                      -1);
  else if (this->event_server_.push (crm) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push (Consumer_Router)")),
                      -1);
  return 0;
}

int
Event_Server::set_watermarks (void)
{
  // Set the high and low water marks appropriately.  The water marks
  // control how much data can be buffered before the queues are
  // considered "full."
  size_t wm = Options::instance ()->low_water_mark ();

  if (this->event_server_.control (ACE_IO_Cntl_Msg::SET_LWM,
                                   &wm) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push (setting low watermark)")),
                      -1);

  wm = Options::instance ()->high_water_mark ();
  if (this->event_server_.control (ACE_IO_Cntl_Msg::SET_HWM,
                                   &wm) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push (setting high watermark)")),
                      -1);
  return 0;
}

int
Event_Server::run_event_loop (void)
{
  // Begin the timer.
  Options::instance ()->start_timer ();

  // Perform the main event loop waiting for the user to type ^C or to
  // enter a line on the ACE_STDIN.

  ACE_Reactor::instance ()->run_reactor_event_loop ();

  // Close down the stream and call the <close> hooks on all the
  // <ACE_Task>s in the various Modules in the Stream.
  this->event_server_.close ();

  // Wait for the threads in the <Consumer_Router> and
  // <Supplier_Router> to exit.
  return ACE_Thread_Manager::instance ()->wait ();
}

int
Event_Server::svc (void)
{
  if (this->configure_stream () == -1)
    return -1;
  else if (this->set_watermarks () == -1)
    return -1;
  else if (this->run_event_loop () == -1)
    return -1;
  else
    return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
#if defined (ACE_HAS_THREADS)
  Options::instance ()->parse_args (argc, argv);

  // Initialize the <Event_Server>.
  Event_Server event_server;

  // Run the event server's event-loop.
  int result = event_server.svc ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("exiting main\n")));

  return result;
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("threads not supported on this platform\n")),
                    1);
#endif /* ACE_HAS_THREADS */
}
