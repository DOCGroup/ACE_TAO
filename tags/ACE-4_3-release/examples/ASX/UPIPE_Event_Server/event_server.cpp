// Test the event server.
// $Id$

#include "ace/Stream.h"
#include "ace/Service_Config.h"
#include "Options.h"
#include "Consumer_Router.h"
#include "Event_Analyzer.h"
#include "Supplier_Router.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/UPIPE_Connector.h"

#if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
# include <iostream.h>
#endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

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
  : ACE_Sig_Adapter (ACE_Sig_Handler_Ex (ACE_Reactor::end_event_loop))
{  
  // Register to trap input from the user.
  if (ACE::register_stdin_handler (this,
				   ACE_Reactor::instance (),
				   ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_stdin_handler"));
  // Register to trap the SIGINT signal.
  else if (ACE_Reactor::instance ()->register_handler 
	   (SIGINT, this) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_handler"));
}

int
Quit_Handler::handle_input (ACE_HANDLE)
{
  options.stop_timer ();
  ACE_DEBUG ((LM_INFO, " (%t) closing down the test\n"));
  options.print_results ();

  ACE_Reactor::end_event_loop();
  return 0;
}

static void *
consumer (void *)
{
  ACE_UPIPE_Stream c_stream;
  ACE_UPIPE_Addr c_addr ("/tmp/conupipe");

  int verb = options.verbose ();
  int msiz = options.message_size ();
  int secs, par1, par2;
  time_t currsec;

  if (verb)
    cout << "consumer starting connect" << endl;

  ACE_UPIPE_Connector con;

  if (con.connect (c_stream, c_addr) == -1)
    ACE_DEBUG ((LM_INFO, " (%t) connect failed\n"));
  else
    cout << "consumer :we're connected" << endl;

  ACE_Message_Block *mb_p;

  int done = 0;
  int cnt = 0;
  ACE_OS::time (&currsec);

  par1= (time_t) currsec;

  while (done == 0 
	 && (c_stream.recv (mb_p) != -1))
    if (mb_p->length () > 1) 
      {
	cnt++;
	if (verb)
	  cout << " consumer received message !!!!!! " 
	       << mb_p->rd_ptr () << endl;
      }
    else
      {
	if (verb)
	  cout << "consumer got last mb" 
	       << (char) * (mb_p->rd_ptr ()) << endl;
	c_stream.close ();
	done = 1;
      }

    ACE_OS::time (&currsec);
    par2 = (time_t) currsec;

    secs = par2 - par1;

    if (secs <= 0)
      secs=1;

    cout << "consumer got " << cnt << " messages of size " << msiz 
	 << "within " << secs << " seconds" << endl;

    ACE_OS::sleep (2);
    cout << "consumer terminating " << endl;
    return 0;
}

static void *
supplier (void *dummy)
{
  ACE_UPIPE_Stream s_stream;
  ACE_UPIPE_Addr serv_addr ("/tmp/supupipe");
  ACE_UPIPE_Connector con;

  int iter = options.iterations ();
  int verb = options.verbose ();
  int msiz = options.message_size ();
  cout << "supplier starting connect" << endl;

  if (con.connect (s_stream, serv_addr) == -1)
    ACE_DEBUG ((LM_INFO, " (%t) connect failed\n"));

  cout << "supplier : we're connected" << endl;
  int	   n;
  n = 0;
  ACE_Message_Block * mb_p;

  while (n < iter)
    {
      mb_p = new ACE_Message_Block (msiz);
      strcpy (mb_p->rd_ptr (), (char *) dummy);
      mb_p->length (msiz);
      if (verb)
        cout << "supplier sending 1 message_block" << endl;
      if (s_stream.send (mb_p) == -1)
	{
	  cout << "supplier send failed" << endl;
	  return (void *) -1;
	}
      n++;
    }

  mb_p = new ACE_Message_Block (10);
  mb_p->length (1);
  *mb_p->rd_ptr () = 'g';

  cout << "supplier sending last message_block" << endl;

  if (s_stream.send (mb_p) == -1)
    {
      cout << "supplier send last mb failed" << endl;
      return (void *) -1;
    }
  mb_p = new ACE_Message_Block (10);
  mb_p->length (0);

  if (verb)
    cout << "supplier sending very last message_block" << endl;

  if (s_stream.send (mb_p) == -1)
    {
      cout << "supplier send very last mb failed" << endl;
      return (void *) -1;
    }

  ACE_OS::sleep (2);
  cout << "supplier terminating" << endl;
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_Service_Config daemon;
  
  options.parse_args (argc, argv);
  options.start_timer ();

  // Primary ACE_Stream for EVENT_SERVER application.
  MT_Stream event_server; 

  // Enable graceful shutdowns....
  Quit_Handler quit_handler;

  // Create the modules..

  MT_Module *sr = new MT_Module ("Supplier_Router", 
	  new Supplier_Router (ACE_Thread_Manager::instance ()));
  MT_Module *ea = new MT_Module ("Event_Analyzer", 
				 new Event_Analyzer, 
				 new Event_Analyzer);
  MT_Module *cr = new MT_Module ("Consumer_Router", 
				 0, // 0 triggers the creation of a ACE_Thru_Task...
				 new Consumer_Router (ACE_Thread_Manager::instance ()));

  // Push the modules onto the event_server stream.

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

  // spawn the two threads.

  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (consumer), (void *) 0,
					     THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), 1);

  else if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (supplier), (void *) "hello",
						  THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), 1);
 
  // Perform the main event loop waiting for the user to type ^C or to
  // enter a line on the ACE_STDIN.

  ACE_Reactor::run_event_loop ();

  ACE_DEBUG ((LM_DEBUG, "main exiting\n"));

  return 0;
}
#else
int 
main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "test not defined for this platform\n"), -1);
}
#endif /* ACE_HAS_THREADS */
