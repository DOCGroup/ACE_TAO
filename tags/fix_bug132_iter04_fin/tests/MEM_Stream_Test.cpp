// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    MEM_Stream_Test.cpp
//
// = DESCRIPTION
//     This is a test of the <ACE_MEM_Acceptor> and
//     <ACE_MEM_Connector> classes.
//
// = AUTHOR
//    Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS.h"
#include "ace/Thread_Manager.h"
#include "ace/MEM_Connector.h"
#include "ace/MEM_Acceptor.h"
#include "ace/Select_Reactor.h"
#include "ace/Connector.h"
#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Singleton.h"

ACE_RCSID(tests, MEM_Stream_Test, "$Id$")

#if defined (ACE_HAS_THREADS)

#include "MEM_Stream_Test.h"         // Defines Echo_Handler

#define NO_OF_CONNECTION 3
#define NO_OF_ITERATION 100

static int opt_wfmo_reactor = 1;
static int opt_select_reactor = 1;
static ACE_MEM_IO::Signal_Strategy client_strategy = ACE_MEM_IO::Reactive;

typedef ACE_Atomic_Op <ACE_Thread_Mutex, u_short> WaitingCounter;
typedef ACE_Singleton <WaitingCounter, ACE_Thread_Mutex> Waiting;

// Number of connections that are currently open
static u_short connection_count = 0;

typedef ACE_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR> ACCEPTOR;
typedef ACE_Strategy_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR> S_ACCEPTOR;

static void reset_handler (void)
{
  (*Waiting::instance ()) = NO_OF_CONNECTION;
  connection_count = 0;
}

int
Echo_Handler::open (void *)
{
  // @@ Nanbor, this method doesn't anything?
  return 0;
}

Echo_Handler::Echo_Handler (ACE_Thread_Manager *thr_mgr)
  : ACE_Svc_Handler<ACE_MEM_STREAM, ACE_MT_SYNCH> (thr_mgr),
    connection_ (++connection_count)
{
  ACE_OS::sprintf (this->name_, ACE_TEXT ("Connection %d --> "),
                   this->connection_);
}

int
Echo_Handler::handle_input (ACE_HANDLE)
{
  ACE_TCHAR buf[MAXPATHLEN];
  ssize_t len;

  len = this->peer ().recv (buf, MAXPATHLEN);

  if (len == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error receiving from MEM_Stream\n")),
                      -1);
  else if (len == 0)
    return -1;

  ACE_TCHAR return_buf[MAXPATHLEN];
  ACE_OS::strcpy (return_buf, this->name_);
  ACE_OS_String::strcat (return_buf, buf);
  len = (ACE_OS::strlen (return_buf) + 1) * sizeof (ACE_TCHAR);

  if (this->peer ().send (return_buf, len) != len)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error sending from MEM_Stream\n")),
                      -1);

  return 0;
}

int
Echo_Handler::handle_close (ACE_HANDLE,
                            ACE_Reactor_Mask mask)
{
  // Reduce count.
  (*Waiting::instance ())--;

#if 1
  if (client_strategy != ACE_MEM_IO::Reactive)
    this->reactor ()->remove_handler (this,
                                      mask | ACE_Event_Handler::DONT_CALL);
#endif /* tests */

  // If no connections are open.
  if ((*Waiting::instance ()) == 0)
    ACE_Reactor::instance ()->end_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Echo_Handler %d::handle_close closing down\n"),
              this->connection_));

  // Shutdown
  this->peer ().cleanup (1);
  this->destroy ();
  return 0;
}

int
Echo_Handler::svc (void)
{
  while (this->handle_input (this->get_handle ()) >= 0)
    ;
  return 0;
}

void *
connect_client (void *arg)
{
  u_short sport =  (*ACE_reinterpret_cast (u_short *, arg));
  ACE_MEM_Addr to_server (sport);
  ACE_MEM_Connector connector;
  connector.preferred_strategy (client_strategy);
  ACE_MEM_Stream stream;

  //  connector.preferred_strategy (ACE_MEM_IO::MT);

  if (connector.connect (stream, to_server.get_remote_addr ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"), ACE_TEXT ("connect_client")),
                      0);

  ACE_TCHAR buf[MAXPATHLEN];

  for (ssize_t cntr = 0; cntr < NO_OF_ITERATION; cntr ++)
    {
      ACE_OS::sprintf (buf, ACE_TEXT ("Iteration %d"), cntr);
      ssize_t slen = (ACE_OS::strlen (buf) + 1) * sizeof (ACE_TCHAR);

      if (stream.send (buf, slen) < slen)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Write_Handler::handle_output - send\n")),
                          0);

      if (stream.recv (buf, MAXPATHLEN) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Write_Handle::handle_output - recv\n")),
                          0);

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Got echo %s\n"), buf));
    }

  stream.close ();
  return 0;
}

void
create_reactor (void)
{
  ACE_Reactor_Impl *impl = 0;

  if (opt_wfmo_reactor)
    {
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
      ACE_NEW (impl,
               ACE_WFMO_Reactor);
#endif /* ACE_WIN32 */
    }
  else if (opt_select_reactor)
    ACE_NEW (impl,
             ACE_Select_Reactor);

  ACE_Reactor *reactor = 0;
  ACE_NEW (reactor,
           ACE_Reactor (impl));
  ACE_Reactor::instance (reactor);
}

int test_reactive (ACE_MEM_Addr &server_addr)
{
  ACE_DEBUG ((LM_DEBUG, "Testing Reactive MEM_Stream\n\n"));

  ACE_Accept_Strategy<Echo_Handler, ACE_MEM_ACCEPTOR> accept_strategy;
  ACE_Creation_Strategy<Echo_Handler> create_strategy;
  ACE_Reactive_Strategy<Echo_Handler> reactive_strategy (ACE_Reactor::instance ());
  S_ACCEPTOR acceptor;
  if (acceptor.open (server_addr,
                     ACE_Reactor::instance (),
                     &create_strategy,
                     &accept_strategy,
                     &reactive_strategy) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("MEM_Acceptor::accept\n")), 1);
  acceptor.acceptor ().mmap_prefix (ACE_TEXT ("MEM_Acceptor_"));

  ACE_MEM_Addr local_addr;
  if (acceptor.acceptor ().get_local_addr (local_addr) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("MEM_Acceptor::get_local_addr\n")),
                        1);
    }

  u_short sport = local_addr.get_port_number ();

  ACE_Thread_Manager::instance ()->spawn_n (NO_OF_CONNECTION,
                                            connect_client,
                                            &sport);
  ACE_Time_Value tv(60, 0);
  ACE_Reactor::instance ()->run_event_loop (tv);

  if (tv == ACE_Time_Value::zero)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("Reactor::run_event_loop timeout\n")),
                        1);
    }

  ACE_DEBUG ((LM_DEBUG, "Reactor::run_event_loop finished\n"));

  ACE_Thread_Manager::instance ()->wait ();

  if (acceptor.close () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("MEM_Acceptor::close\n")),
                        1);
    }
  return 0;
}

int test_multithreaded (ACE_MEM_Addr &server_addr)
{
  ACE_DEBUG ((LM_DEBUG, "Testing Multithreaded MEM_Stream\n\n"));

  client_strategy = ACE_MEM_IO::MT;
  ACE_Accept_Strategy<Echo_Handler, ACE_MEM_ACCEPTOR> accept_strategy;
  ACE_Creation_Strategy<Echo_Handler> create_strategy;
  ACE_Thread_Strategy<Echo_Handler> thr_strategy;
  S_ACCEPTOR acceptor;


  if (acceptor.open (server_addr,
                     ACE_Reactor::instance (),
                     &create_strategy,
                     &accept_strategy,
                     &thr_strategy) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("MEM_Acceptor::accept\n")), 1);

  acceptor.acceptor ().malloc_options ().minimum_bytes_ = 1024 * 1024 ;
  acceptor.acceptor ().mmap_prefix (ACE_TEXT ("MEM_Acceptor_"));
  acceptor.acceptor ().preferred_strategy (ACE_MEM_IO::MT);

  ACE_MEM_Addr local_addr;
  if (acceptor.acceptor ().get_local_addr (local_addr) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("MEM_Acceptor::get_local_addr\n")),
                        1);
    }

  u_short sport = local_addr.get_port_number ();

  ACE_Thread_Manager::instance ()->spawn_n (NO_OF_CONNECTION,
                                            connect_client,
                                            &sport);
  ACE_Time_Value tv(60, 0);
  ACE_Reactor::instance ()->run_event_loop (tv);

  if (tv == ACE_Time_Value::zero)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("Reactor::run_event_loop timeout\n")),
                        1);
    }

  ACE_DEBUG ((LM_DEBUG, "Reactor::run_event_loop finished\n"));

  ACE_Thread_Manager::instance ()->wait ();

  if (acceptor.close () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("MEM_Acceptor::close\n")),
                        1);
    }
  return 0;
}

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("MEM_Stream_Test"));

  create_reactor ();

  unsigned short port = 0;
  ACE_MEM_Addr server_addr (port);

  reset_handler ();

  test_reactive (server_addr);

  ACE_Reactor::instance ()->reset_event_loop ();

  reset_handler ();

  test_multithreaded (server_addr);

  // Now testing

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Svc_Handler <ACE_MEM_STREAM, ACE_MT_SYNCH>;
template class ACE_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR>;
template class ACE_Atomic_Op<ACE_Thread_Mutex, u_short>;
template class ACE_Singleton<ACE_Atomic_Op<ACE_Thread_Mutex, u_short>,ACE_Thread_Mutex>;
template class ACE_Accept_Strategy<Echo_Handler, ACE_MEM_ACCEPTOR>;
template class ACE_Creation_Strategy<Echo_Handler>;
template class ACE_Reactive_Strategy<Echo_Handler>;
template class ACE_Strategy_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR>;
template class ACE_Concurrency_Strategy<Echo_Handler>;
template class ACE_Scheduling_Strategy<Echo_Handler>;
template class ACE_Thread_Strategy<Echo_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Svc_Handler <ACE_MEM_STREAM, ACE_MT_SYNCH>
#pragma instantiate ACE_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR>
#pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, u_short>
#pragma instantiate ACE_Singleton<ACE_Atomic_Op<ACE_Thread_Mutex, u_short>,ACE_Thread_Mutex>
#pragma instantiate ACE_Accept_Strategy<Echo_Handler, ACE_MEM_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<Echo_Handler>
#pragma instantiate ACE_Reactive_Strategy<Echo_Handler>
#pragma instantiate ACE_Strategy_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR>
#pragma instantiate ACE_Concurrency_Strategy<Echo_Handler>
#pragma instantiate ACE_Scheduling_Strategy<Echo_Handler>
#pragma instantiate ACE_Thread_Strategy<Echo_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("MEM_Stream_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_THREADS */
