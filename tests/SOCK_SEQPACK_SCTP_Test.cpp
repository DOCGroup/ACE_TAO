// $Id$
//
// *WARRANTY DISCLAIMER: LIMITATION OF LIABILITY. THE SOFTWARE AND
// CONTENT ARE PROVIDED "AS IS" WITH NO EXPRESS OR IMPLIED
// REPRESENTATIONS, GUARANTEES, OR WARRANTIES, INCLUDING BUT NOT LIMITED
// TO SUCH REPRESENTATION, GUARANTEES OR WARRANTIES REGARDING THE
// USABILITY, SUITABILITY, CONDITION, OPERATION OR ACCURACY THEREOF. *
//
// *ALL OTHER WARRANTIES AND CONDITIONS (EXPRESS, IMPLIED OR STATUTORY)
// ARE HEREBY DISCLAIMED, SUCH WARRANTIES AND CONDITIONS INCLUDING
// WITHOUT LIMITATION, ALL WARRANTIES AND CONDITIONS OF MERCHANTABILITY,
// TITLE, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT,
// COMPATIBILITY, AND SECURITY OR ACCURACY.*
//
// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    SOCK_SEQPACK_SCTP_Test.cpp
//
// = DESCRIPTION
//    Performs several tests on the ACE_SOCK_SEQPACK_Connector,
//    ACE_SOCK_SEQPACK_Acceptor, and ACE_SOCK_SEQPACK_Association classes
//    specifically for SCTP using the loopback interface.  Attempts to
//    replicate behavior of SOCK_Test.cpp, but integrating IPv6 tests
//    directly.
//
// = AUTHOR
//    Dave Craig <dwc@qualcomm.com>
//

#include "test_config.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_select.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/SOCK_SEQPACK_Connector.h"
#include "ace/SOCK_SEQPACK_Acceptor.h"
#include "ace/Thread_Manager.h"
#include "ace/Handle_Set.h"

#define TTCPPORT 5001
#define BYTE_MESG 0xcd

struct tdesc {
  ACE_Thread_Semaphore *tsemap;
  bool ipv6_test;
};

typedef struct tdesc tdesc_t;

#ifdef ACE_WIN64
// This arg is ignored on Windows and causes pointer truncation
// warnings on 64-bit compiled.
#define SELECT_WIDTH(x) 0
#else
#define SELECT_WIDTH(x) (x)
#endif

ACE_THR_FUNC_RETURN
Server (void *arg)
{
  ACE_SOCK_SEQPACK_Acceptor *AcceptorSocket =
    reinterpret_cast<ACE_SOCK_SEQPACK_Acceptor *> (arg);

  ACE_SOCK_SEQPACK_Association Stream;

  ACE_Handle_Set handle_set;

  const ACE_Time_Value def_timeout (ACE_DEFAULT_TIMEOUT);

  ACE_Time_Value tv (def_timeout);

  int select_width;

  int result;

  //
  // Make sure AcceptorSocket is in nonblocking mode so as not to
  // hang tests.
  //
  if (-1 == AcceptorSocket->enable (ACE_NONBLOCK))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("AcceptorSocket.enable (ACE_NONBLOCK)")));

  //
  // Set up select to wait for I/O events.
  //
  handle_set.reset ();
  handle_set.set_bit (AcceptorSocket->get_handle ());

  select_width = SELECT_WIDTH(int (AcceptorSocket->get_handle ()) + 1);

  result = ACE_OS::select(select_width,
                          handle_set,
                          0,
                          0,
                          &tv);

  if (-1 == result)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) %p\n"),
                       ACE_TEXT ("select")),
                      0);
  if (0 == result)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P|%t) select timed out, shutting down\n")),
                      0);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) waiting for client to connect\n")));

  while (-1 != AcceptorSocket->accept (Stream)) {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) client connected\n")));

    //
    // Enable non-blocking I/O.
    //
    if (Stream.enable (ACE_NONBLOCK))
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) %p\n"),
                         ACE_TEXT ("Stream.enable (ACE_NONBLOCK)")),
                        0);

    unsigned char byte = BYTE_MESG;

    if (-1 == Stream.send_n (&byte, 1))
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("Stream.send_n")));

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) byte sent\n")));

    //
    // Abruptly terminate the association.
    //
    if (-1 == Stream.abort ())
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("Association.abort")));

    //
    // Negative test: make sure that we cannot send on a closed association.
    //
    if (-1 != Stream.send_n (&byte, 1))
      //FUZZ: disable check_for_lack_ACE_OS
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) Negative test fail: Association")
                  ACE_TEXT(".send_n succeeded after abort()\n")));
      //FUZZ: enable check_for_lack_ACE_OS

  }

  //
  // Close server socket.
  //
  if (-1 == AcceptorSocket->close ())
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("AcceptorSocket.close")));

  return 0;
}

ACE_THR_FUNC_RETURN
Client(void *arg)
{
  ACE_Multihomed_INET_Addr *ServerAddr =
    reinterpret_cast<ACE_Multihomed_INET_Addr *> (arg);

  ACE_SOCK_SEQPACK_Connector Connector;

  ACE_SOCK_SEQPACK_Association Stream;

  ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);

  char b;
  size_t bytes;

  if (-1 == Connector.connect (Stream,
                               *ServerAddr,
                               &tv,
                               ACE_Addr::sap_any,
                               1))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) %p to %s:%d\n"),
                       ACE_TEXT ("Connector.connect"),
                       ServerAddr->get_host_name (),
                       ServerAddr->get_port_number ()),
                      0);
  }

  if (-1 == Stream.disable (ACE_NONBLOCK))
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("Association.disable (ACE_NONBLOCK)")));
  }


  if (-1 == Stream.recv_n (&b, 1, &tv, &bytes))
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("Association.recv_n")));
  }

  if (1 == bytes)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) Client received %B bytes\n"),
                bytes));
  else
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) Client received %B bytes; expected 1\n"),
                bytes));

  //
  // Give server a little time to abort the association.
  //
  ACE_OS::sleep(1);

  // abort closes the connection, so the recv should either see a closed
  // socket or some failure other than a timeout.
  ssize_t cnt = Stream.recv_n (&b, 1, &tv, &bytes);
  if (cnt > 0 || (cnt == -1 && errno == ETIME))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) Negative test failed; Association")
                ACE_TEXT (".recv_n returned %b (w/ %m) after abort\n"),
                cnt));

  return 0;
}

//
// Spawn server and client threads and then wait until they complete the
// test.  There must be a timeout on the wait, so executable does not hang the
// tests indefinitely.
//
int
spawn_test(bool ipv6_test)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) spawn_test started ipv6 %d\n"),
              ipv6_test));

  ACE_SOCK_SEQPACK_Acceptor AcceptorSocket;

  const ACE_TCHAR *addrstr =
#ifdef ACE_HAS_IPV6
    ipv6_test ? ACE_IPV6_LOCALHOST : ACE_LOCALHOST;
#else
    ACE_LOCALHOST;
#endif /* ACE_HAS_IPV6 */
  ACE_Multihomed_INET_Addr ServerAddr (TTCPPORT,
                                       addrstr
#ifdef ACE_HAS_IPV6
                                       ,1,
                                       ipv6_test ? AF_INET6 : AF_INET
#endif /* ACE_HAS_IPV6 */
                                       );

  if (-1 == AcceptorSocket.open (ServerAddr, 1))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("AcceptorSocket.open")));
    }

  if (-1 == AcceptorSocket.get_local_addr (ServerAddr))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("AcceptorSocket.get_local_addr")));
    }

#ifndef ACE_LACKS_FORK
  switch (ACE_OS::fork (ACE_TEXT ("child")))
  {
  case -1:
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p"),
                ACE_TEXT ("fork failed")));
    break;
  case 0:
    ACE_LOG_MSG->sync (ACE_TEXT ("SOCK_SEQPACK_SCTP_Test"));
    Client (&ServerAddr);
    ACE_OS::exit (0);
    break;
  default:
    Server (reinterpret_cast<void *> (&AcceptorSocket));
    ACE_OS::wait ();
    break;
  }
#elif defined (ACE_HAS_THREADS)
  if (-1 == ACE_Thread_Manager::instance ()->spawn
      (Server,
       reinterpret_cast<void *> (&AcceptorSocket),
       THR_NEW_LWP | THR_DETACHED))
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p%a"),
                ACE_TEXT ("thread create failed")));
  }

  if (-1 == ACE_Thread_Manager::instance ()->spawn
      (Client,
       reinterpret_cast<void *> (&ServerAddr),
       THR_NEW_LWP | THR_DETACHED))
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p%a"),
                ACE_TEXT ("thread create failed")));
  }

  ACE_Thread_Manager::instance ()->wait ();
#else /* ACE_LACKS_FORK && ! ACE_HAS_THREADS */
  ACE_ERROR ((LM_DEBUG,
              ACE_TEXT ("(%P|%t)\n"),
              ACE_TEXT ("only one thread may be run ")
              ACE_TEXT ("in a process on this platform\n")));
#endif /* ACE_LACKS_FORK && ! ACE_HAS_THREADS */

  return 0;
}

int
do_test(void)
{
  spawn_test(false);

#ifdef ACE_HAS_IPV6
  spawn_test(true);
#endif

  return 0;
}

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SOCK_SEQPACK_SCTP_Test"));

  //
  // Check whether host OS has SCTP support before starting this test.
  // If not, just pass because there is not a hope of testing
  // SOCK_SEQPACK.
  //
  int status = 0;

#ifdef ACE_HAS_SCTP
  status = do_test();
#else /* ! ACE_HAS_SCTP */
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("SCTP not supported by ACE.\n")
              ACE_TEXT("This test will not do anything.\n")));
#endif /* ! ACE_HAS_SCTP */

  ACE_END_TEST;

  return status;
}

