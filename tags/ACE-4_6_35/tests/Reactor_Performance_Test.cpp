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
#include "Reactor_Performance_Test.h"
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

ACE_RCSID(tests, Reactor_Performance_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS)

// Number of client (user) threads
#if (!defined (ACE_WIN32) || (defined (ACE_HAS_WINNT4) && ACE_HAS_WINNT4 != 0))
static int opt_nconnections = 20;
#else /* ACE_WIN32 || (ACE_HAS_WINNT4 && ACE_HAS_WINNT4 != 0) */
static int opt_nconnections = 5;
#endif /* ACE_WIN32 || (ACE_HAS_WINNT4 && ACE_HAS_WINNT4 != 0) */

// Number of data exchanges
static int opt_nloops = 200;

// Use the WFMO_Reactor
static int opt_wfmo_reactor = 0;

// Use the Select_Reactor
static int opt_select_reactor = 0;

// Extra debug messages
static int opt_debug = 0;

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
                       ASYS_TEXT ("(%t) Read_Handler::open, cannot set non blocking mode\n")),
                      -1);

  if (reactor ()->register_handler (this, READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%t) Read_Handler::open, cannot register handler\n")),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) created svc_handler for handle %d\n"),
              get_handle ()));
  return 0;
}

// Handle incoming data
int
Read_Handler::handle_input (ACE_HANDLE handle)
{
  ACE_UNUSED_ARG (handle);
  char buf[BUFSIZ];

  while (1)
    {
      ssize_t result = this->peer ().recv (buf, sizeof (buf) - 1);

      if (result > 0)
        {
          if (opt_debug)
            {
              buf[result] = 0;
              ACE_DEBUG ((LM_DEBUG,
                          ASYS_TEXT ("(%t) Read_Handler::handle_input: %s\n"),
                          buf));
            }
        }
      else if (result < 0)
        {
          if (errno == EWOULDBLOCK)
            return 0;
          else
            // This will cause handle_close to get called.
            return -1;
        }
      else // result == 0
        {
          // This will cause handle_close to get called.
          return -1;
        }
    }

  ACE_NOTREACHED (return 0);
}

// Handle connection shutdown.

int
Read_Handler::handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask)
{
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (close_mask);

  // Reduce count.
  waiting_--;

  // If no connections are open.
  if (waiting_ == 0)
    ACE_Reactor::instance ()->end_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) Read_Handler::handle_close closing down\n")));

  // Shutdown
  this->destroy ();
  return 0;
}

int
Write_Handler::open (void *)
{
  return 0;
}

int
Write_Handler::send_data (void)
{
  int send_size = sizeof (ACE_ALPHABET) - 1;

  if (this->peer ().send_n (ACE_ALPHABET,
                            send_size) != send_size)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%t) %p\n"),
                       ASYS_TEXT ("send_n")),
                      -1);
  return 0;
}

// Connection factories
typedef ACE_Connector<Write_Handler, ACE_SOCK_CONNECTOR> CONNECTOR;
typedef ACE_Acceptor<Read_Handler, ACE_SOCK_ACCEPTOR> ACCEPTOR;

// Execute the client tests.
void *
client (void *arg)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) running client\n")));

  ACE_INET_Addr *connection_addr =
    ACE_reinterpret_cast (ACE_INET_Addr *, arg);
  CONNECTOR connector;

  int i;

  // Automagic memory cleanup.
  ACE_Auto_Basic_Array_Ptr <Write_Handler *> writers;
  Write_Handler **temp_writers;
  ACE_NEW_RETURN (temp_writers,
                  Write_Handler *[opt_nconnections],
                  0);
  writers = temp_writers;

  ACE_Auto_Basic_Array_Ptr <ASYS_TCHAR> failed_svc_handlers;
  ASYS_TCHAR *temp_failed;
  ACE_NEW_RETURN (temp_failed,
                  ASYS_TCHAR[opt_nconnections],
                  0);
  failed_svc_handlers = temp_failed;

  // Automagic memory cleanup.
  ACE_Auto_Array_Ptr <ACE_INET_Addr> addresses;
  ACE_INET_Addr *temp_addresses;
  ACE_NEW_RETURN (temp_addresses,
                  ACE_INET_Addr [opt_nconnections],
                  0);
  addresses = temp_addresses;

  // Initialize array.
  for (i = 0; i < opt_nconnections; i++)
    {
      writers[i] = 0;
      addresses[i] = *connection_addr;
    }

  // Connection all <opt_nconnections> svc_handlers
  int result = connector.connect_n (opt_nconnections,
                                    writers.get (),
                                    addresses.get (),
                                    failed_svc_handlers.get ());
  if (result == -1)
    {
      // Print out the connections that failed...
      for (i = 0; i < opt_nconnections; i++)
        if (failed_svc_handlers.get ()[i])
          {
            ACE_INET_Addr failed_addr = addresses.get()[i];
            ACE_ERROR ((LM_ERROR,
                        ASYS_TEXT ("(%t) connection failed to %s, %d\n"),
                        failed_addr.get_host_name (),
                        failed_addr.get_port_number ()));
          }
      return 0;
    }

  // If no connections failed (result == 0) then there should be valid
  // ACE_Svc_handler pointers in each writers[] position.  Iterate to
  // send data
  for (int j = 0; j < opt_nloops; j++)
    for (i = 0; i < opt_nconnections; i++)
      if (writers[i]->send_data () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("(%t) %p\n"),
                           ASYS_TEXT ("writer::send_data")),
                          0);
  // Cleanup
  for (i = 0; i < opt_nconnections; i++)
    writers[i]->destroy ();

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) finishing client\n")));
  return 0;
}

// Sets up the correct reactor (based on platform and options).

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

// Print stats.

void
print_results (ACE_Profile_Timer::ACE_Elapsed_Time &et)
{
  const ASYS_TCHAR *reactor_type = 0;

  if (opt_wfmo_reactor)
    reactor_type = ASYS_TEXT ("WFMO_Reactor");
  else if (opt_select_reactor)
    reactor_type = ASYS_TEXT ("Select_Reactor");
  else
    reactor_type = ASYS_TEXT ("Platform's default Reactor");

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("\n\tReactor_Performance Test statistics:\n\n")));
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("\tReactor Type: %s\n"),
              reactor_type));
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("\tConnections: %d\n"),
              opt_nconnections));
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("\tIteration per connection: %d\n"),
              opt_nloops));

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("\n\tTiming results:\n")));
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("\t\treal time = %f secs \n\t\tuser time = %f secs \n\t\tsystem time = %f secs\n\n"),
              et.real_time,
              et.user_time,
              et.system_time));
}

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Reactor_Performance_Test"));

  ACE_Get_Opt getopt (argc, argv, ASYS_TEXT ("dswc:l:"), 1);
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
        opt_nconnections = ACE_OS::atoi (getopt.optarg);
        break;
      case 'l':
        opt_nloops = ACE_OS::atoi (getopt.optarg);
        break;
      case 'd':
        opt_debug = 1;
        break;
      }

  // Sets up the correct reactor (based on platform and options).
  create_reactor ();

  // Manage memory automagically.
  auto_ptr<ACE_Reactor> reactor (ACE_Reactor::instance ());
  auto_ptr<ACE_Reactor_Impl> impl;

  // If we are using other that the default implementation, we must
  // clean up.
  if (opt_select_reactor || opt_wfmo_reactor)
    impl = auto_ptr<ACE_Reactor_Impl> (ACE_Reactor::instance ()->implementation ());

  Read_Handler::set_countdown (opt_nconnections);

  // Acceptor
  ACCEPTOR acceptor;
  ACE_INET_Addr server_addr;

  // Bind acceptor to any port and then find out what the port was.
  if (acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &)) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%t) %p\n"),
                       ASYS_TEXT ("open")),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) starting server at port %d\n"),
              server_addr.get_port_number ()));

  ACE_INET_Addr connection_addr (server_addr.get_port_number (),
                                 ACE_DEFAULT_SERVER_HOST);

  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (client),
       (void *) &connection_addr,
       THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("(%t) %p\n"),
                ASYS_TEXT ("thread create failed")));

  ACE_Profile_Timer timer;
  timer.start ();
  ACE_Reactor::instance()->run_event_loop ();
  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;
  timer.elapsed_time (et);

  // Print results
  print_results (et);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) waiting for the client thread...\n")));

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
template class ACE_Map_Manager<ACE_HANDLE,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<ACE_HANDLE,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<ACE_HANDLE,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_HANDLE,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<ACE_HANDLE,ACE_Svc_Tuple<Write_Handler>*>;
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
#pragma instantiate ACE_Map_Manager<ACE_HANDLE,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<ACE_HANDLE,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_HANDLE,ACE_Svc_Tuple<Write_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<ACE_HANDLE,ACE_Svc_Tuple<Write_Handler>*>
#pragma instantiate ACE_Svc_Tuple<Write_Handler>
#pragma instantiate ACE_Auto_Basic_Array_Ptr <Write_Handler *>
#pragma instantiate ACE_Auto_Basic_Array_Ptr <ACE_INET_Addr>
#pragma instantiate ACE_Auto_Array_Ptr <ACE_INET_Addr>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Reactor_Performance_Test"));

  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("threads not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_THREADS */
