// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Reactor_Performance_Test.cpp
//
// = DESCRIPTION
//
//    This test is used to time the dispatching mechanisms of the
//    ACE_Reactors. Both the WFMO_Reactor and Select_Reactor can be
//    tested. 
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#include "test_config.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Auto_Ptr.h"

#if defined (ACE_HAS_THREADS) 

// Number of client (user) threads 
static int opt_nconnections = 20;

// Number of data exchanges
static int opt_nloops = 200;

// Use the WFMO_Reactor
static int opt_wfmo_reactor = 0;

// Use the Select_Reactor
static int opt_select_reactor = 0;

// Simple class for reading in the data
class Read_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
{
public:
  static void set_countdown (int nconnections);

  virtual int open (void *);
  virtual int handle_input (ACE_HANDLE h);
  virtual int handle_close (ACE_HANDLE handle,
			    ACE_Reactor_Mask close_mask);
  // The Svc_Handler callbacks.

private:
  static int waiting_;
  // How many connections are we waiting for.
};

int Read_Handler::waiting_ = 0;

void 
Read_Handler::set_countdown (int nconnections)
{
  Read_Handler::waiting_ = nconnections;
}

// Initialize the Svc_Handler
int 
Read_Handler::open (void *)
{
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%t) Read_Handler::open, cannot set non blocking mode\n"), -1);
  
  if (reactor ()->register_handler (this, READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%t) Read_Handler::open, cannot register handler\n"), -1);
  
  ACE_DEBUG ((LM_DEBUG, "(%t) created svc_handler for handle %d\n", get_handle ()));
  return 0;
}

// Handle incoming data
int 
Read_Handler::handle_input (ACE_HANDLE handle)
{
  ACE_UNUSED_ARG (handle);

  char buf[BUFSIZ];
  
  ssize_t result = this->peer ().recv (buf, sizeof (buf));
  if (result <= 0)
    {
      if (result < 0 && errno == EWOULDBLOCK)
	return 0;
      
      if (result != 0)
        ACE_DEBUG ((LM_DEBUG, "(%t) %p\n", "Read_Handler::handle_input"));
      
      // This will cause handle_close to get called
      return -1;
    }
  
  return 0;
}

// Handle connection shutdown
int 
Read_Handler::handle_close (ACE_HANDLE handle,
			    ACE_Reactor_Mask close_mask)
{
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (close_mask);
  
  // Reduce count
  waiting_--;
  
  // If no connections are open
  if (waiting_ == 0)
    {
      ACE_Reactor::instance ()->end_event_loop ();
    }
  
  ACE_DEBUG ((LM_DEBUG, 
              "(%t) Read_Handler::handle_close closing down\n"));
  
  // Shutdown
  this->destroy ();
  return 0;
}

// This Svc_Handler simply connects to a server and sends some output
// to it.  Its purpose is to feed the test.
class Write_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
{
public:
  virtual int open (void *);
  virtual int send_data (void);
};

int 
Write_Handler::open (void *)
{
  return 0;
}

int 
Write_Handler::send_data (void)
{
  int send_size = sizeof (ACE_ALPHABET) - 1;

  if (this->peer ().send_n (ACE_ALPHABET, send_size) != send_size)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "send_n"), -1);

  return 0;
}

// Connection factories
typedef ACE_Connector<Write_Handler, ACE_SOCK_CONNECTOR> CONNECTOR;
typedef ACE_Acceptor<Read_Handler, ACE_SOCK_ACCEPTOR> ACCEPTOR;

// Execute the client tests.
void * 
client (void *arg)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) running client\n"));

  ACE_INET_Addr *connection_addr = (ACE_INET_Addr*) arg;
  CONNECTOR connector;

  int i;
  
  // Automagic memory cleanup
  ACE_Auto_Basic_Array_Ptr <Write_Handler *> writers;
  Write_Handler **temp_writers;
  ACE_NEW_RETURN (temp_writers, Write_Handler *[opt_nconnections], 0);
  writers = temp_writers;

  // Automagic memory cleanup
  ACE_Auto_Array_Ptr <ACE_INET_Addr> addresses;
  ACE_INET_Addr *temp_addresses;
  ACE_NEW_RETURN (temp_addresses, ACE_INET_Addr [opt_nconnections], 0);
  addresses = temp_addresses;

  // Initialize array
  for (i = 0; i < opt_nconnections; i++)
    {
      writers[i] = 0;
      addresses[i] = *connection_addr;
    }

  // Connection all <opt_nconnections> svc_handlers
  int result = connector.connect_n (opt_nconnections, 
                                    writers.get (), 
                                    addresses.get ());
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "connects failed"), 0);
  
  // Iterate to send data
  for (int j = 0; j < opt_nloops; j++)
    for (i = 0; i < opt_nconnections; i++)
      if (writers[i]->send_data () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "writer::send_data"), 0);
  
  // Cleanup
  for (i = 0; i < opt_nconnections; i++)
    writers[i]->destroy ();  

  ACE_DEBUG ((LM_DEBUG, "(%t) finishing client\n"));
  return 0;
}

// Sets up the correct reactor (based on platform and options)
void
create_reactor (void)
{
  ACE_Reactor_Impl *impl = 0;
  
  if (opt_wfmo_reactor)
    {
#if defined (ACE_WIN32)
      ACE_NEW (impl, ACE_WFMO_Reactor);
#endif /* ACE_WIN32 */
    }
  else if (opt_select_reactor)
    {
      ACE_NEW (impl, ACE_Select_Reactor);
    }
  ACE_Reactor *reactor = 0;
  ACE_NEW (reactor, ACE_Reactor (impl));
  ACE_Reactor::instance (reactor);
}

// Print stats
void
print_results (ACE_Profile_Timer::ACE_Elapsed_Time &et)
{
  char *reactor_type = 0;
  if (opt_wfmo_reactor)
    reactor_type = "WFMO_Reactor";
  else if (opt_select_reactor)
    reactor_type = "Select_Reactor";
  else
    reactor_type = "Platform's default Reactor";

  ACE_DEBUG ((LM_DEBUG, "\n\tReactor_Performance Test statistics:\n\n"));
  ACE_DEBUG ((LM_DEBUG, "\tReactor Type: %s\n", reactor_type));
  ACE_DEBUG ((LM_DEBUG, "\tConnections: %d\n", opt_nconnections));
  ACE_DEBUG ((LM_DEBUG, "\tIteration per connection: %d\n", opt_nloops));

  ACE_DEBUG ((LM_DEBUG, "\n\tTiming results:\n"));
  ACE_DEBUG ((LM_DEBUG,
	      "\t\treal time = %f secs \n\t\tuser time = %f secs \n\t\tsystem time = %f secs\n\n",
	      et.real_time,
	      et.user_time,
	      et.system_time));  
}

int 
main (int argc, char *argv[])
{
  ACE_START_TEST ("Reactor_Performance_Test");

  ACE_Get_Opt getopt (argc, argv, "swc:l:", 1);
  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
      case 's':
	opt_select_reactor = 1;
	break;
      case 'w':
	opt_wfmo_reactor = 1;
	break;
      case 'c':
	opt_nconnections = atoi (getopt.optarg);
	break;
      case 'l':
	opt_nloops = atoi (getopt.optarg);
	break;
      }

  // Sets up the correct reactor (based on platform and options)
  create_reactor ();

  // Manage memory automagically.
  auto_ptr<ACE_Reactor> reactor (ACE_Reactor::instance ());
  auto_ptr<ACE_Reactor_Impl> impl; 

  // If we are using other that the default implementation, we must
  // clean up.
  if (opt_select_reactor || opt_wfmo_reactor)
    impl = ACE_Reactor::instance ()->implementation ();  

  Read_Handler::set_countdown (opt_nconnections);

  // Acceptor
  ACCEPTOR acceptor;
  ACE_INET_Addr server_addr;

  // Bind acceptor to any port and then find out what the port was.
  if (acceptor.open ((const ACE_INET_Addr &) ACE_Addr::sap_any) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "open"), -1);

  ACE_DEBUG ((LM_DEBUG, "(%t) starting server at port %d\n",
	      server_addr.get_port_number ()));

  ACE_INET_Addr connection_addr (server_addr.get_port_number (), "localhost");
      
  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (client),
       (void *) &connection_addr,
       THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "thread create failed"));

  ACE_Profile_Timer timer;
  timer.start ();
  ACE_Reactor::instance()->run_event_loop ();
  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;
  timer.elapsed_time (et);

  // Print results
  print_results (et);

  ACE_DEBUG ((LM_DEBUG, "(%t) waiting for the client thread...\n"));

  ACE_Thread_Manager::instance ()->wait ();

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Connector<Write_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Acceptor<Read_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>;
template class auto_ptr<ACE_Reactor>;
template class ACE_Auto_Basic_Ptr<ACE_Reactor>;
template class auto_ptr<ACE_Reactor_Impl>;
template class ACE_Auto_Basic_Ptr<ACE_Reactor_Impl>;
template class ACE_Map_Manager<int,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<int,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int,ACE_Svc_Tuple<Write_Handler>*>;
template class ACE_Svc_Tuple<Write_Handler>;
template class ACE_Auto_Basic_Array_Ptr <Write_Handler *>;
template class ACE_Auto_Basic_Array_Ptr <ACE_INET_Addr>;
template class ACE_Auto_Array_Ptr <ACE_INET_Addr>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Connector<Write_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Acceptor<Read_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
#pragma instantiate auto_ptr<ACE_Reactor>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Reactor>
#pragma instantiate auto_ptr<ACE_Reactor_Impl>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Reactor_Impl>
#pragma instantiate ACE_Map_Manager<int,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<int,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int,ACE_Svc_Tuple<Write_Handler>*>
#pragma instantiate ACE_Svc_Tuple<Write_Handler>
#pragma instantiate ACE_Auto_Basic_Array_Ptr <Write_Handler *>
#pragma instantiate ACE_Auto_Basic_Array_Ptr <ACE_INET_Addr>
#pragma instantiate ACE_Auto_Array_Ptr <ACE_INET_Addr>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
main (int, char *[])
{
  ACE_START_TEST ("Reactor_Performance_Test");

  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));

  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_THREADS */


