/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    CLASSIX_Con_Acc_Test.cpp
//
// = DESCRIPTION
//     Based on $ACE_ROOT/tests/Priority_Reactor_Test.cpp
//     However, this test is not to test the priority aspect of the
//     CLASSIX Reactor.  In fact <ACE_CLASSIX_Select_Reactor> does not
//     handle priorities.
//
//     This is a test for <ACE_CLASSIX_CO_Connector> and
//     <ACE_CLASSIX_CO_Acceptor>
// ============================================================================

#include "ace/Get_Opt.h"
#include "ace/Acceptor.h"
#include "ace/Handle_Set.h"
#include "ace/Connector.h"
#include "ace/Strategies.h"
#include "ace/Auto_Ptr.h"
#include "ace/Priority_Reactor.h"

#include "ace/CLASSIX/CLASSIX_CO_Connector.h"
#include "ace/CLASSIX/CLASSIX_CO_Acceptor.h"

#include "CLASSIX_test_config.h"
#include "CLASSIX_Con_Acc_Test.h"


// The number of children to run, it can be changed using the -c
// option.
static int opt_nchildren = 2 /* 10 */;

// The number of loops per children, it can be changed using the -l
// option.
static int opt_nloops = 200 /* 200 */;

// If not set use the normal reactor, it can be changed using the -d
// option.
static int opt_priority_reactor = 1;

// Maximum time to wait for the test termination (-t)
static int opt_max_duration = 60;

// Maximum number of unread messages (-i)
static int opt_max_msgs = 300;

// Maximum number of retries to connect, it can be changed using the
// -m option.
static int max_retries = 5;

typedef ACE_Connector<Write_Handler, ACE_CLASSIX_CO_CONNECTOR>
        CONNECTOR;
typedef ACE_Acceptor<Read_Handler, ACE_CLASSIX_CO_ACCEPTOR>
        ACCEPTOR;

typedef ACE_CLASSIX_Port ADDR;

ACE_Atomic_Op<ACE_Thread_Mutex, int> Read_Handler::waiting_ = 0;
ACE_Atomic_Op<ACE_Thread_Mutex, int> Read_Handler::started_ = 0;

// Maximum number of unread messages
// This is to overcome the blocking problem with ipcSend() in ClassixOS 3.1
ACE_Atomic_Op<ACE_Thread_Mutex, int> max_msg = 0;

void
Read_Handler::set_countdown (int nchildren)
{
  Read_Handler::waiting_ = nchildren;
}

int
Read_Handler::get_countdown (void)
{
  return Read_Handler::waiting_.value();
}

int
Read_Handler::open (void *)
{
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Read_Handler::open, "
                       "cannot set non blocking mode"), -1);

  if (reactor ()->register_handler (this, READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Read_Handler::open, "
                       "cannot register handler"), -1);

  // A number larger than the actual number of priorities, so some
  // clients are misbehaved, hence pusnished.
  const int max_priority = 15;

  this->priority (ACE_Event_Handler::LO_PRIORITY + started_.value() % max_priority);
  started_++;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) created svc_handler for handle %d\n",
              get_handle ()));
  return 0;
}

int
Read_Handler::handle_input (ACE_HANDLE h)
{
  char buf[BUFSIZ];

//  ACE_DEBUG((LM_DEBUG,
//           "(%P|%t|%x) read from handle %d...", this, h));
  ssize_t result = this->peer ().recv (buf, sizeof (buf));

  //
  // Work around the blocking problem with ipcSend() in ClassixOS 3.1
  // This counter ensures that the reader reads before the writer sends too
  // many messages.
  //
  max_msg--;

  if (result <= 0)
    {
      if (result < 0 && errno == EWOULDBLOCK)
      {
          max_msg++;
          return 0;
      }

      if (result != 0)
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) %p\n",
                    "Read_Handler::handle_input"));
      waiting_--;

      if (waiting_ == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Last svc_handler closed, shutting down\n"));
          ACE_Reactor::instance()->end_event_loop();
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Read_Handler(%d)::handle_input closing down\n", h));
      return -1;
    }

//  ACE_DEBUG((LM_DEBUG,
//           "...(%P|%t) read %d bytes from handle %d, priority %d\n",
//           result, h, priority ()));
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
        // Work around the blocking problem with ipcSend() in ClassixOS 3.1
        // ipcSend() will block instead of returning K_EFULL value,
        // if resources are not available.
        //
        // sleep for the reader to empty some messages
        while (max_msg.value() >= opt_max_msgs)
            ACE_OS::sleep(pause);


        if (this->peer ().send_n (ACE_ALPHABET,
                                  sizeof (ACE_ALPHABET) - 1) == -1)
        {
            ACE_DEBUG((LM_DEBUG, "%t %p\n", "send_n\n"));
            ACE_OS::sleep (pause);
            continue;
        }
        max_msg++;
    }
  this->peer().close_writer();
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Write Handler exiting svc\n"));
  return 0;
}

// Execute the client tests.
static void *
client (void *arg)
{
  ADDR *connection_addr = (ADDR *) arg;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) running client\n"));
  CONNECTOR connector;

  Write_Handler *writer = 0;

  // Do exponential backoff connections
  ACE_Synch_Options options = ACE_Synch_Options::synch;

  // Start with one msec timeouts.
  ACE_Time_Value msec (0, 1000);
  options.timeout (msec);

  // Try up to <max_retries> to connect to the server.
  for (int i = 0; i < max_retries; i++)
    {
      if (connector.connect (writer,
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
      else
        {
            // Let the new Svc_Handler to its job...
            writer->svc ();

            // then close the connection and release the Svc_Handler.
            writer->destroy ();

            return 0;
        }
    }

  ACE_ERROR ((LM_ERROR,
              "(%P|%t) failed to connect after %d retries\n",
              max_retries));
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Con_Acc_Test");

  // initialize environment, eg. reactor, etc.
  ACE_CLASSIX_OS os;

  ACE_Get_Opt getopt (argc, argv, "dc:l:m:t:i:", 1);

  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
      case 'd':
        opt_priority_reactor = 0;
        break;
      case 'i':
        opt_max_msgs = atoi (getopt.opt_arg ());
        break;
      case 'c':
        opt_nchildren = atoi (getopt.opt_arg ());
        break;
      case 'l':
        opt_nloops = atoi (getopt.opt_arg ());
        break;
      case 'm':
        max_retries = atoi (getopt.opt_arg ());
        break;
      case 't':
        opt_max_duration = atoi (getopt.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR, "Usage: Priority_Reactor_Test "
                           "   [-d] (disable priority reactor)\n"
                           "   [-i max_msgs] (max. unread messages)\n"
                           "   [-c nchildren] (number of threads/processes)\n"
                           "   [-l loops] (number of loops per child)\n"
                           "   [-m maxretries] (attempts to connect)\n"
                           "   [-t max_time] (limits test duration)\n"), -1);
        ACE_NOTREACHED (break);
      }

  Read_Handler::set_countdown (opt_nchildren);

  // Acceptor
  ACE_DEBUG((LM_DEBUG, "Create an Acceptor\n"));
  ACCEPTOR acceptor(ACE_Reactor::instance(), 0);
  // The acceptor uses the global reactor and does not use select.

  acceptor.priority (ACE_Event_Handler::HI_PRIORITY);
  ADDR server_addr;

  // Bind acceptor to any port and then find out what the port was.
  ACE_DEBUG((LM_DEBUG, "Open the acceptor\n"));
  if (acceptor.open ((const ADDR &) ACE_Addr::sap_any) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "open"), -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting server at port %d\n",
              server_addr.get_port_number ()));

  ADDR connection_addr (server_addr);

  int i;

  for (i = 0; i < opt_nchildren; ++i)
    {
      if (ACE_Thread_Manager::instance ()->spawn
          (ACE_THR_FUNC (client),
           (void *) &connection_addr,
           THR_NEW_LWP | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));
    }

  ACE_Time_Value tv (opt_max_duration);

  ACE_Reactor::instance()->register_handler
    (&acceptor, ACE_Event_Handler::READ_MASK);
  ACE_Reactor::instance()->run_event_loop (tv);

  if (Read_Handler::get_countdown () != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) running out of time, "
                  "probably due to failed connections.\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) waiting for the children...\n"));

  ACE_Thread_Manager::instance ()->wait ();

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Connector<Write_Handler, ACE_CLASSIX_CO_CONNECTOR>;
template class ACE_Acceptor<Read_Handler, ACE_CLASSIX_CO_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_CLASSIX_STREAM, ACE_SYNCH>;
template class auto_ptr<ACE_Reactor>;
template class ACE_Auto_Basic_Ptr<ACE_Reactor>;
template class auto_ptr<ACE_Select_Reactor>;
template class ACE_Auto_Basic_Ptr<ACE_Select_Reactor>;
template class ACE_NonBlocking_Connect_Handler<Write_Handler>;
template class ACE_Atomic_Op<ACE_Thread_Mutex, int>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Connector<Write_Handler, ACE_CLASSIX_CONNECTOR>
#pragma instantiate ACE_Acceptor<Read_Handler, ACE_CLASSIX_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_CLASSIX_STREAM, ACE_SYNCH>
#pragma instantiate auto_ptr<ACE_Reactor>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Reactor>
#pragma instantiate auto_ptr<ACE_Select_Reactor>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Select_Reactor>
#pragma instantiate ACE_NonBlocking_Connect_Handler<Write_Handler>
#pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
