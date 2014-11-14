//=============================================================================
/**
 *  @file  Bug_4189_Regression_Test.cpp
 *
 *  $Id$
 *
 * This test shows the reuse address issue on broadcast sockets.
 * Start two process instances of this test on the same host within x sec.
 * For broadcast sockets it should be possible to open the same port more than
 * once at the time on a host.
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Log_Msg.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include "ace/Thread_Manager.h"

int result = 0;

const int UDP_PORT = 3000;
const int SEC_TO_KEEP_SOCKET_OPEN = 2;

#if defined (ACE_HAS_THREADS)

int reuseAddr_test (void)
{
  ACE_SOCK_Dgram_Bcast sock1;
  if (sock1.open(ACE_INET_Addr(UDP_PORT),PF_INET,0,1) != 0)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Could not open socket for broadcast on port %d\n"), UDP_PORT ));
    ++result;
  }
  else
  {
    // Keep the socket open for some time
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Opened socket on port %d and keep it open for %d sec\n"), UDP_PORT,  SEC_TO_KEEP_SOCKET_OPEN));

    ACE_OS::sleep(ACE_Time_Value(SEC_TO_KEEP_SOCKET_OPEN,0));

    sock1.close();
  }
  return result;
}

/* \brief Thread main function to run run_receiver function
   \note run_receiver return valu is stored in receiver_exit_code global variable
*/
static ACE_THR_FUNC_RETURN run_thread_receiver (void *)
{
  reuseAddr_test ();
  return 0;
}
#endif /* defined (ACE_HAS_THREADS) */

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_4189_Regression_Test"));

#if defined (ACE_HAS_THREADS)
  if (ACE_Thread_Manager::instance ()->spawn_n (2, run_thread_receiver) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn_n ()")), -1);

  ACE_Thread_Manager::instance ()->wait ();
#endif

  ACE_END_TEST;
  return result;
}
