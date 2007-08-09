// $Id$

#include "test_config.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_select.h"
#include "ace/OS_NS_sys_wait.h"
//#include "ace/SOCK_SEQPACK_Connector.h"
#include "ace/SOCK_SCTP_SEQPACK_Acceptor.h"
#include "ace/Thread_Manager.h"
#include "ace/Handle_Set.h"

#define TTCPPORT 5001
#define BYTE_MESG 0xcd

int run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ACE_TEXT ("SOCK_SCTP_SEQPACK_Test"));

  //
  // Check whether host OS has SCTP support before starting this test.
  // If not, just pass because there is not a hope of testing
  // SOCK_SEQPACK.
  //
  int status = 0;

#ifdef ACE_HAS_SCTP
  //status = do_test();
#else /* ! ACE_HAS_SCTP */
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("SCTP not supported by ACE.\n")
              ACE_TEXT("This test will not do anything.\n")));
#endif /* ! ACE_HAS_SCTP */

  ACE_END_TEST;

  return status;
}

