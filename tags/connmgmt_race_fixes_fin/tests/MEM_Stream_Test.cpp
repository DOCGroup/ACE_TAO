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

ACE_RCSID(tests, MEM_Stream_Test, "$Id$")

#if defined (ACE_HAS_THREADS)

#include "MEM_Stream_Test.h"         // Defines Echo_Handler

#define NO_OF_CONNECTION 3
#define NO_OF_ITERATION 100

static int opt_wfmo_reactor = 1;
static int opt_select_reactor = 1;

u_short Echo_Handler::waiting_ = NO_OF_CONNECTION;
u_short Echo_Handler::connection_count_ = 0;

typedef ACE_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR> ACCEPTOR;

//  int
//  Echo_Handler::open (void *)
//  {
//    // @@ Nanbor, this method doesn't anything?
//    return 0;
//  }

Echo_Handler::Echo_Handler ()
  : connection_ (++Echo_Handler::connection_count_)
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
                            ACE_Reactor_Mask)
{
  // Reduce count.
  this->waiting_--;

  // If no connections are open.
  if (this->waiting_ == 0)
    ACE_Reactor::instance ()->end_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Echo_Handler %d::handle_close closing down\n"),
              this->connection_));

  // Shutdown
  this->peer ().remove ();
  this->destroy ();
  return 0;
}

void *
connect_client (void *arg)
{
  u_short sport =  (*ACE_reinterpret_cast (u_short *, arg));
  ACE_MEM_Addr to_server (sport);
  ACE_MEM_Connector connector;
  ACE_MEM_Stream stream;

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

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("MEM_Stream_Test"));

  create_reactor ();

  unsigned short port = 0;
  ACE_MEM_Addr server_addr (port);

  ACCEPTOR acceptor;
  acceptor.acceptor ().mmap_prefix (ACE_TEXT ("MEM_Acceptor_"));
  if (acceptor.open (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("MEM_Acceptor::accept\n")), 1);

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

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Svc_Handler <ACE_MEM_STREAM, ACE_MT_SYNCH>;
template class ACE_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Svc_Handler <ACE_MEM_STREAM, ACE_MT_SYNCH>
#pragma instantiate ACE_Acceptor<Echo_Handler, ACE_MEM_ACCEPTOR>
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
