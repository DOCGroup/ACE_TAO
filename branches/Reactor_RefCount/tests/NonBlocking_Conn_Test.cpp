// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    NonBlocking_Conn_Test.cpp
//
// = DESCRIPTION
//    This test checks for the proper working of the following:
//    - blocking connections
//    - blocking connections with timeouts
//    - non-blocking connections
//    - non-blocking connections without waiting for completions
//    - non-blocking connections with timeouts
//
// = AUTHOR
//    Irfan Pyarali <irfan@oomworks.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Get_Opt.h"

static int test_select_reactor = 1;
static int test_tp_reactor = 1;
static int test_wfmo_reactor = 1;

class Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:

  enum Connection_Status
    {
      SUCCEEDED,
      FAILED
    };

  Svc_Handler (void);

  void connection_status (Connection_Status &status,
                          int &completion_counter);

  int open (void *);

  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask mask);

  Connection_Status *status_;
  int *completion_counter_;
};

Svc_Handler::Svc_Handler (void)
  : status_ (0),
    completion_counter_ (0)
{
}

void
Svc_Handler::connection_status (Connection_Status &status,
                                int &completion_counter)
{
  this->status_ = &status;
  this->completion_counter_ = &completion_counter;
}

int
Svc_Handler::open (void *)
{
  *this->status_ = SUCCEEDED;
  (*this->completion_counter_)++;

  return 0;
}

int
Svc_Handler::handle_close (ACE_HANDLE handle,
                           ACE_Reactor_Mask mask)
{
  *this->status_ = FAILED;
  (*this->completion_counter_)++;

  return ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::handle_close (handle,
                                                                         mask);
}

typedef ACE_Connector<Svc_Handler,
                      ACE_SOCK_CONNECTOR>
        CONNECTOR;

static const char* hosts[] = {
  "www.russiantvguide.com:80",
  "www.pakarmy.gov.pk:80",
  "www.cnn.com:80",
  "www.waca.com.au:80",
  "www.uganda.co.ug:80",
  "www.cs.wustl.edu:80",
  "www.affa-az.com:80",
  "www.dhm.gov.np:80",
  "www.msn.com:80",
  "www.presidencymaldives.gov.mv:80" };

static int number_of_connections = 0;

void
test_connect (ACE_Reactor &reactor,
              ACE_INET_Addr *addresses,
              ACE_Synch_Options &synch_options,
              int complete_nonblocking_connections)
{
  CONNECTOR connector (&reactor);

  int i = 0;

  int completion_counter = 0;
  Svc_Handler::Connection_Status *connection_status =
    new Svc_Handler::Connection_Status[number_of_connections];

  Svc_Handler **svc_handlers =
    new Svc_Handler *[number_of_connections];

  for (i = 0; i < number_of_connections; ++i)
    {
      svc_handlers[i] =
        new Svc_Handler;

      svc_handlers[i]->connection_status (connection_status[i],
                                          completion_counter);
    }

  connector.connect_n (number_of_connections,
                       svc_handlers,
                       addresses,
                       0,
                       synch_options);

  if (!synch_options[ACE_Synch_Options::USE_REACTOR])
    ACE_ASSERT (completion_counter == number_of_connections);

  if (complete_nonblocking_connections)
    {
      while (completion_counter != number_of_connections)
        {
          connector.reactor ()->handle_events ();
        }
    }

  connector.close ();

  for (i = 0; i < number_of_connections; ++i)
    {
      ACE_TCHAR buffer[1024];
      addresses[i].addr_to_string (buffer,
                                   sizeof buffer,
                                   1);

      ACE_DEBUG ((LM_DEBUG,
                  "Connection to %s %s\n",
                  buffer,
                  connection_status[i] == Svc_Handler::SUCCEEDED ?
                  "succeeded" : "failed"));

      if (connection_status[i] == Svc_Handler::SUCCEEDED)
        {
          svc_handlers[i]->close ();
        }
    }

  delete[] svc_handlers;
  delete[] connection_status;
}

void
test (ACE_Reactor_Impl *impl)
{
  number_of_connections = sizeof hosts / sizeof (char *);

  ACE_INET_Addr *addresses =
    new ACE_INET_Addr[number_of_connections];

  for (int i = 0; i < number_of_connections; ++i)
    {
      addresses[i] = ACE_INET_Addr (hosts[i]);
    }

  ACE_Reactor reactor (impl,
                       1);

  int complete_nonblocking_connections = 1;
  int dont_wait_for_nonblocking_connections = 0;
  int ignored = 99;

  ACE_Synch_Options blocking_connect =
    ACE_Synch_Options::defaults;

  ACE_DEBUG ((LM_DEBUG,
              "\nBlocking connections...\n\n"));

  test_connect (reactor,
                addresses,
                blocking_connect,
                ignored);

  blocking_connect.set (ACE_Synch_Options::USE_TIMEOUT,
                        ACE_Time_Value (0, 50 * 1000));

  ACE_DEBUG ((LM_DEBUG,
              "\nBlocking connections (with timeouts)...\n\n"));

  test_connect (reactor,
                addresses,
                blocking_connect,
                ignored);

  ACE_Synch_Options nonblocking_connect
    (ACE_Synch_Options::USE_REACTOR);

  ACE_DEBUG ((LM_DEBUG,
              "\nNon-blocking connections...\n\n"));

  test_connect (reactor,
                addresses,
                nonblocking_connect,
                complete_nonblocking_connections);

  ACE_DEBUG ((LM_DEBUG,
              "\nNon-blocking connections (without waiting for completions)...\n\n"));

  test_connect (reactor,
                addresses,
                nonblocking_connect,
                dont_wait_for_nonblocking_connections);

  nonblocking_connect.set (ACE_Synch_Options::USE_REACTOR |
                           ACE_Synch_Options::USE_TIMEOUT,
                           ACE_Time_Value (0, 500 * 1000));

  ACE_DEBUG ((LM_DEBUG,
              "\nNon-blocking connections (with timeouts)...\n\n"));

  test_connect (reactor,
                addresses,
                nonblocking_connect,
                complete_nonblocking_connections);

  delete[] addresses;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("a:b:c:"));

  int cc;
  while ((cc = get_opt ()) != -1)
    {
      switch (cc)
        {
        case 'a':
          test_select_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'b':
          test_tp_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'c':
          test_wfmo_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case '?':
        case 'u':
        default:
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("\nusage: %s \n\n")
                      ACE_TEXT ("\t[-a test Select Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-b test TP Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-c test WFMO Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\n"),
                      argv[0],
                      test_select_reactor,
                      test_tp_reactor,
                      test_wfmo_reactor));
          return -1;
        }
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("NonBlocking_Conn_Test"));

  // Validate options.
  int result =
    parse_args (argc, argv);
  if (result != 0)
    return result;

  if (test_select_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\nTesting Select Reactor....\n\n"));

      test (new ACE_Select_Reactor);
    }

  if (test_tp_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\nTesting TP Reactor....\n\n"));

      test (new ACE_TP_Reactor);
    }

#if defined (ACE_WIN32)

  if (test_wfmo_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\nTesting WFMO Reactor....\n\n"));

      test (new ACE_WFMO_Reactor);
    }

#endif /* ACE_WIN32 */

  ACE_END_TEST;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_NonBlocking_Connect_Handler<Svc_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_NonBlocking_Connect_Handler<Svc_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
