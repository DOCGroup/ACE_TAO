// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Priority_Reactor_Test.cpp
//
// = DESCRIPTION
//     This is a test of the <ACE_Priority_Reactor>.  The test forks
//     two processes (for a total of three processes) which connect to
//     the main process and The clients send data to a connector,
//     interestingly enough the acceptor will give more priority to
//     the second connection, which should run always before the first
//     one.
//
//     The test itself is interesting, it shows how to write very
//     simple <ACE_Svc_Handler>, <ACE_Connectors> and <ACE_Acceptors>.
//
// = AUTHOR
//    Carlos O'Ryan
//
// ============================================================================

#include "test_config.h"
#include "ace/Get_Opt.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Handle_Set.h"
#include "ace/Connector.h"
#include "ace/Strategies.h"
#include "ace/Auto_Ptr.h"
#include "ace/Priority_Reactor.h"
#include "Priority_Reactor_Test.h"

static int opt_nchildren = 20;
// The number of children to run, it can be changed using the -c
// option.

static int opt_nloops = 200;
// The number of loops per children, it can be changed using the -l
// option.

static int opt_priority_reactor = 1;
// If not set use the normal reactor, it can be changed using the -d
// option.

typedef ACE_Connector<Write_Handler, ACE_SOCK_CONNECTOR> CONNECTOR;
typedef ACE_Acceptor<Read_Handler, ACE_SOCK_ACCEPTOR> ACCEPTOR;

int Read_Handler::waiting_ = 0;
int Read_Handler::started_ = 0;

void 
Read_Handler::set_countdown (int nchildren)
{
  Read_Handler::waiting_ = nchildren;
}

int 
Read_Handler::open (void *)
{
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) Read_Handler::open, "
                       "cannot set non blocking mode"), -1);

  if (reactor ()->register_handler (this, READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) Read_Handler::open, "
                       "cannot register handler"), -1);

  // A number larger than the actual number of priorities, so some
  // clients are misbehaved, hence pusnished.
  const int max_priority = 15;

  priority (ACE_Event_Handler::LO_PRIORITY + started_ % max_priority);
  started_++;
  ACE_DEBUG((LM_DEBUG, "(%P|%t) created svc_handler for handle %d "
	     "with priority %d\n", get_handle (), priority ()));

  return 0;
}

int 
Read_Handler::handle_input (ACE_HANDLE h)
{
  // ACE_DEBUG((LM_DEBUG,
  // "(%P|%t) Read_Handler::handle_input(%d)\n", h));
  ACE_UNUSED_ARG (h);

  char buf[BUFSIZ];

  ssize_t result = this->peer ().recv (buf, sizeof(buf));
  if (result <= 0)
    {
      if (result < 0 && errno == EWOULDBLOCK)
	return 0;

      if (result != 0)
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) %p\n",
                    "Read_Handler::handle_input"));
      waiting_--;
      if (waiting_ == 0)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "last svc_handler closed, shutting down"));
	  ACE_Reactor::instance()->end_event_loop();
	}
      
      ACE_DEBUG ((LM_DEBUG, 
                  "(%P|%t) Read_Handler::handle_input closing down\n"));
      return -1;
    }
  
  // ACE_DEBUG((LM_DEBUG,
  // "(%P|%t) read %d bytes from handle %d, priority %d\n",
  // result, h, priority ()));
  return 0;
}

int 
Write_Handler::open (void *)
{
  return 0;
}

int 
Write_Handler::svc (void)
{
  // Send several short messages, doing pauses between each message.
  // The number of messages can be controlled from the command line.
  ACE_Time_Value pause (0, 1000);
  for (int i = 0; i < opt_nloops; ++i)
    {
      if (this->peer ().send_n (ACE_ALPHABET,
				sizeof(ACE_ALPHABET) - 1) == -1)
	  ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "send_n"));
      ACE_OS::sleep (pause);
    }
  return 0;
}

// Execute the client tests.
void * 
client (void *arg)
{
  ACE_INET_Addr *connection_addr = (ACE_INET_Addr*)arg;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) running client\n"));
  CONNECTOR connector;

  Write_Handler *writer = 0;

  // Do exponential backoff connections
  ACE_Synch_Options options = ACE_Synch_Options::synch;

  ACE_Time_Value msec(0,1000); // start with one msec timeouts.
  options.timeout (msec);

  while (connector.connect (writer,
			    *connection_addr,
			    options) == -1)
    {
      // Double the timeout...
      ACE_Time_Value tmp = options.timeout ();
      tmp += options.timeout ();
      options.timeout (tmp);
      writer = 0;
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) still trying to connect\n"));
    }

  // Let the new Svc_Handler to its jobs....
  writer->svc ();

  // Close the connection...
  writer->destroy ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) finishing client\n"));
  return 0;
}

int 
main (int argc, char *argv[])
{
  ACE_START_TEST ("Priority_Reactor_Test");

  ACE_Get_Opt getopt (argc, argv, "dc:l:", 1);
  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
      case 'd':
	opt_priority_reactor = 0;
	break;
      case 'c':
	opt_nchildren = atoi (getopt.optarg);
	break;
      case 'l':
	opt_nloops = atoi (getopt.optarg);
	break;
      }

  // Manage memory automagically.
  auto_ptr<ACE_Select_Reactor> impl;
  auto_ptr<ACE_Reactor> reactor;

  if (opt_priority_reactor)
    {
      ACE_Select_Reactor *impl_ptr;
      ACE_NEW_RETURN (impl_ptr, ACE_Priority_Reactor, -1);
      impl = impl_ptr;
      ACE_Reactor *reactor_ptr;
      ACE_NEW_RETURN (reactor_ptr, ACE_Reactor (impl_ptr), -1);
      reactor = reactor_ptr;
      ACE_Reactor::instance (reactor_ptr);
    }

  Read_Handler::set_countdown (opt_nchildren);

  // Acceptor
  ACCEPTOR acceptor;

  acceptor.priority (ACE_Event_Handler::HI_PRIORITY);
  ACE_INET_Addr server_addr;

  // Bind acceptor to any port and then find out what the port was.
  if (acceptor.open ((const ACE_INET_Addr &) ACE_Addr::sap_any) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "open"), -1);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting server at port %d\n",
	      server_addr.get_port_number ()));

  ACE_INET_Addr connection_addr (server_addr.get_port_number (), "localhost");
      
  int i;

#if defined (ACE_HAS_THREADS) 
  for (i = 0; i < opt_nchildren; ++i)
    {
      if (ACE_Thread_Manager::instance ()->spawn
	  (ACE_THR_FUNC (client),
	   (void *) &connection_addr,
	   THR_NEW_LWP | THR_DETACHED) == -1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));
    }
#elif !defined (ACE_WIN32) && !defined (VXWORKS) 
  for (i = 0; i < opt_nchildren; ++i)
    {
      switch (ACE_OS::fork ("child"))
	{
	case -1:
	  ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "fork failed"));
	  exit (-1);
	  /* NOTREACHED */
	case 0:
	  client (&connection_addr);
	  exit (0);
	  break;
	  /* NOTREACHED */
	default:
	  break;
	  /* NOTREACHED */
	}
    }
#else
  ACE_ERROR ((LM_ERROR,
	      "(%P|%t) only one thread may be run in a process on this platform\n%a", 1));
#endif /* ACE_HAS_THREADS */

  ACE_Reactor::instance()->register_handler
    (&acceptor, ACE_Event_Handler::READ_MASK);
  ACE_Reactor::instance()->run_event_loop ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) waiting for the children...\n"));

#if defined (ACE_HAS_THREADS)
  ACE_Thread_Manager::instance ()->wait ();
#elif !defined (ACE_WIN32) && !defined (VXWORKS)
  for (i = 0; i < opt_nchildren; ++i)
    {
      pid_t pid = ACE_OS::wait();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) child %d terminated\n", pid));
    }
#else
  /* NOTREACHED */
  // We aborted on the previous #ifdef
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Connector<Write_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Acceptor<Read_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>;
template class auto_ptr<ACE_Reactor>;
template class ACE_Auto_Basic_Ptr<ACE_Reactor>;
template class auto_ptr<ACE_Select_Reactor>;
template class ACE_Auto_Basic_Ptr<ACE_Select_Reactor>;
template class ACE_Map_Manager<int,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<int,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int,ACE_Svc_Tuple<Write_Handler>*>;
template class ACE_Svc_Tuple<Write_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Connector<Write_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Acceptor<Read_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
#pragma instantiate auto_ptr<ACE_Reactor>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Reactor>
#pragma instantiate auto_ptr<ACE_Select_Reactor>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Select_Reactor>
#pragma instantiate ACE_Map_Manager<int,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<int,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int,ACE_Svc_Tuple<Write_Handler>*>
#pragma instantiate ACE_Svc_Tuple<Write_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
