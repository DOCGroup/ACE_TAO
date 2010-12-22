// $Id$

// Test program for the INET IPC-SAPs...

#include "ace/OS_main.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Mem_Map.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_CODgram.h"
#include "ace/INET_Addr.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/Truncate.h"

// Name of the program.
static const ACE_TCHAR *program_name;

// Port number to use.
static u_short port_number = ACE_DEFAULT_SERVER_PORT;

// Name of remote host.
static const ACE_TCHAR *host_name = ACE_DEFAULT_SERVER_HOST;

// Name of file to send.
static const ACE_TCHAR *file_name = ACE_TEXT("./remote_data");

static void
print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR,
              "usage: %s [-p portnum] [-h host_name] [-f file]\n",
              program_name));
  ACE_OS::exit (1);
}

void
parse_args (int argc, ACE_TCHAR *argv[])
{
  program_name = argv[0];
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("f:h:p:"));

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'f':
        file_name = get_opt.opt_arg ();
        break;
      case 'h':
        host_name = get_opt.opt_arg ();
        break;
      case 'p':
        port_number = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        print_usage_and_die ();
        break;
      }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  parse_args (argc, argv);
  ACE_INET_Addr sa (port_number, host_name);
  void *cp = 0;
  char buf[BUFSIZ];
  int n;
  ACE_SOCK_CODgram dc;

  if (dc.open (sa, ACE_Addr::sap_any) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  // First send the name of the file as a datagram.

  iovec iov[2];

  iov[0].iov_base = (char *) "filename: ";
  iov[0].iov_len  = 11;
  iov[1].iov_base = (char *) file_name;
  iov[1].iov_len  =
    ACE_Utils::truncate_cast<u_long> (ACE_OS::strlen (file_name));

  if (dc.send (iov, 2) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "send"),
                      -1);

  ACE_SOCK_Stream sc;
  ACE_SOCK_Connector con;

  if (con.connect (sc, sa) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "connect"),
                      -1);

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Mem_Map mmap (file_name);

  if (mmap (cp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "mmap"),
                      -1);
  //FUZZ: enable check_for_lack_ACE_OS

  // Next, send the file's contents.

  if (sc.send_n (cp, mmap.size ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "send_urg"),
                      -1);

  if (sc.close_writer () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "close_writer"),
                      -1);

  if ((n = sc.recv_n (buf, sizeof buf)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "recv"),
                      -1);
  else
    ACE_OS::write (ACE_STDOUT, buf, n);

  return 0;
}
