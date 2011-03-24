//=============================================================================
/**
 *  @file    SOCK_Dgram_Bcast_Test.cpp
 *
 *  $Id$
 *
 *   This simple broadcast test is intended to check if ACE is capable
 *   of sending and receiving broadcasts. In single host environment most
 *   errors related to invalid broadcast initialization will not manifest
 *   themself, because usually broadcast on localhost interface works
 *   correctly. For this reason one should run also this test on two distinct
 *   hosts in single LAN.
 *   Tests that a call to open with an any address binds to the any address
 *   for the protocol passed in.
 *
 *
 *  @author Marek Brudka (mbrudka@elka.pw.edu.pl)
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include "ace/Thread_Manager.h"
#include "ace/Process.h"
#include "ace/Process_Manager.h"

static int              dgram_port  = 14521;
static int              dgrams_no   = 10;
static ACE_Time_Value   dgram_recv_timeout( 5, 0 );
static ACE_exitcode     receiver_exit_code = 0;

/*\brief Create and send single datagram
  \param socket broadcast over this socket
  \param datagram_no datagram identifier
  \return -1 if error, 0 if OK
*/
int send_datagram (ACE_SOCK_Dgram_Bcast &socket, int datagram_no)
{
    static char dgram_buffer[BUFSIZ];

    ACE_OS::snprintf (dgram_buffer, sizeof(dgram_buffer),
                      "Datagram %d", datagram_no);
    if (socket.send (dgram_buffer,
                     ACE_OS::strlen (dgram_buffer) + 1,
                     dgram_port) < 0 )
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                           ACE_TEXT ("Cannot broadcast datagram")), -1);
    else
        ACE_DEBUG ((LM_INFO, ACE_TEXT ("%C sent\n"), dgram_buffer));
    return 0;
}

/*\brief Send a sequence of datagrams with 1 second period
  \note Th function employs dgram_port and dgrams_no global variables
  \retval -1 if error
  \retval 0 if sent
*/
int run_sender( )
{
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("Sending %d datagrams on port %d\n"),
                dgrams_no,
                dgram_port));
    ACE_SOCK_Dgram_Bcast socket;

    if (socket.open (ACE_Addr::sap_any) != -1)
      {
        while (dgrams_no-- != 0)
          {
            if (send_datagram (socket, dgrams_no) < 0)
              break;
            ACE_OS::sleep (1);
          }
        socket.close ();
        return (0);
      }

    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Cannot open broadcast socket")), -1);
}

/*\brief Receive single datagram
  \note The function employes dgram_port and dgram_recv_timeout variables
  \retval -1 if not received,
  \retval 0  received a datagrams
*/
int run_receiver ()
{
  ACE_DEBUG
    ((LM_INFO,
      ACE_TEXT ("Receiving datagrams from port %d with timeout %d ms\n"),
      dgram_port, dgram_recv_timeout.msec ()));

  ACE_SOCK_Dgram socket;
  ACE_INET_Addr  remote ;
  static char    dgram_buffer[BUFSIZ];

  if (socket.open (ACE_INET_Addr (dgram_port)) != -1)
    if (socket.recv (dgram_buffer, sizeof (dgram_buffer),
                     remote, 0, &dgram_recv_timeout) > 0)
      {
        ACE_DEBUG ((LM_INFO, ACE_TEXT ("%C received\n"), dgram_buffer));
        return 0;
      }
    else
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                           ACE_TEXT ("Cannot receive datagrams")), -1);
      }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p: %d\n"),
                         ACE_TEXT ("Cannot open broadcast socket on port"), dgram_port), -1);
    }
}

#if !defined (ACE_HAS_PROCESS_SPAWN) && defined (ACE_HAS_THREADS)
/* \brief Thread main function to run run_receiver function
   \note run_receiver return valu is stored in receiver_exit_code global variable
*/
static ACE_THR_FUNC_RETURN run_thread_receiver (void *)
{
  receiver_exit_code = run_receiver ();
  return 0;
}
#endif /* !defined (ACE_HAS_PROCESS_SPAWN) && defined (ACE_HAS_THREADS) */

/* \brief Just runs automatic tests

Function sends a number of datagrams, spawns child thread or process and
tries to receive at least one datagram.
\retval 0 datagram was received
\retval -1 datagram was not received
*/
int run_auto_test (const ACE_TCHAR *prog_name)
{
#if defined (ACE_HAS_PROCESS_SPAWN)
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Running auto_tests in process mode\n")));

  ACE_Process_Options opts;
  pid_t child_pid;
  opts.command_line (ACE_TEXT ("%s -p %d -t %d -a -r"),
                     prog_name, dgram_port, dgram_recv_timeout.msec ());
  if ((child_pid = ACE_Process_Manager::instance ()->spawn (opts)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn_n()")), -1);

#elif defined (ACE_HAS_THREADS)
  ACE_UNUSED_ARG (prog_name);
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Running auto_tests in thread mode\n")));
  if (ACE_Thread_Manager::instance ()->spawn (run_thread_receiver) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn_n ()")), -1);
#else
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("Cannot run in auto_test mode without fork or threads.\n")),
                      -1);
#endif /* defined (ACE_HAS_PROCESS_SPAWN) */

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Sending datagrams on port %d in auto_test mode\n"),
              dgram_port));

  ACE_SOCK_Dgram_Bcast socket;

  if (socket.open (ACE_Addr::sap_any) != -1)
    {
      // send datagrams until child finishes
      while (1)
        {
          send_datagram (socket, dgrams_no--);
          ACE_Time_Value child_timeout (1);
#if defined (ACE_HAS_PROCESS_SPAWN)

          if (ACE_Process_Manager::instance ()->wait (child_pid,
                                                      child_timeout,
                                                      &receiver_exit_code) == child_pid)
            break;
#else /* ACE_HAS_THREADS */
          // sleep 1 second or wait for child thread
          child_timeout += ACE_OS::gettimeofday () ;
          if (ACE_Thread_Manager::instance ()->wait (&child_timeout) == 0)
            break;
#endif
        }
      socket.close ();
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Child finished with %d exit code\n"),
                  receiver_exit_code));
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Cannot open broadcast socket")), -1);
  return (receiver_exit_code);
}

void print_usage (void)
{
  ACE_OS::printf("Usage:SOCK_Dgram_Bast_Test [-p port] [-n dgrams_no] [-t timeout_ms] [-s] [-r]\n"
                 "\tp broadcast port [14521]\n"
                 "\tn number of datagrams to broadcast [30] (<0 infinite)\n"
                 "\tt timeout in seconds for receive [5] (<=0 infinite)\n"
                 "\ts send datagrams and exit\n"
                 "\tr receive one datagram and exit\n\n"
                 "\t  run auto-test when no r and s option is passed\n"
                 "\t  test failures are minifested by -1 exit value, otherwise 0\n");
}

int run_main (int argc, ACE_TCHAR *argv[])
{
  // parse options and run in appropriate mode
  int opt             = 0;
  int auto_test_recv  = 0;
  int result          = 0;
  ACE_Get_Opt opts (argc, argv, ACE_TEXT ("p:t:n:sra"));
  while ((opt = opts ()) != -1)
    switch (opt)
      {
      case 'a':
        auto_test_recv = 1;
        break;
      case 's':
        return (run_sender());
      case 'r':
        {
          if (auto_test_recv)
            {
              ACE_START_TEST (ACE_TEXT ("SOCK_Dgram_Bcast_Test_Child"));
              result = run_receiver ();
              ACE_END_TEST;
              return result;
            }
          return (run_receiver ());
        }
      case 'n':
        dgrams_no = ACE_OS::atoi (opts.opt_arg ());
        break;
      case 't':
        dgram_recv_timeout.msec (ACE_OS::atoi (opts.opt_arg ()));
        break;
      case 'p':
        dgram_port = ACE_OS::atoi (opts.opt_arg ());
        break;
      default:
        print_usage ();
        return -1;
      }
  ACE_START_TEST (ACE_TEXT ("SOCK_Dgram_Bcast_Test"));
  result = run_auto_test (argc > 0 ? argv[0] : ACE_TEXT ("SOCK_Dgram_Bcast_Test"));
  ACE_END_TEST;
  return result;
}
