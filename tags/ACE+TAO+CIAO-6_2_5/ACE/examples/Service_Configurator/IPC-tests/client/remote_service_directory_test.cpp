// $Id$

// Test program for the INET IPC-SAPs...

#include "ace/OS_main.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

// Port number to use.
static unsigned short port_number = ACE_DEFAULT_SERVICE_PORT;

// Name of remote host.
static const ACE_TCHAR *host_name = ACE_DEFAULT_SERVER_HOST;

// Trigger a remote reconfiguration.
static int remote_reconfigure = 0;

static void
print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR,
              "usage: %n [-p portnum] [-h host_name] [-r]\n"));
  ACE_OS::exit (1);
}

void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("p:h:r"));

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
    {
    case 'h':
      host_name = get_opt.opt_arg ();
      break;
    case 'p':
      port_number = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'r':
      remote_reconfigure = 1;
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  parse_args (argc, argv);
  // Default is to ask the server for ``help.''
  static char buf[BUFSIZ] = "help\n";
  int n;
  ACE_SOCK_Stream   sc;
  ACE_SOCK_Connector con;

  if (con.connect (sc,
                   ACE_INET_Addr (port_number,
                                  host_name)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n%a",
                       "connect",
                       1),
                      -1);

  if (remote_reconfigure)
    // Remotely instruct the server to reconfigure itself.
    ACE_OS::strcpy (buf, "reconfigure\n");

  // Send the command.

  if (sc.send_n (buf,
                 ACE_OS::strlen (buf) + 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n%a",
                       "send",
                       1), -1);

  // Next, read the response.

  while ((n = sc.recv (buf,
                       sizeof buf)) > 0)
    if (ACE_OS::write (ACE_STDOUT,
                       buf,
                       n) != n)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n%a",
                         "write",
                         1),
                        -1);

  if (sc.close () == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n%a",
                         "close",
                         1),
                        -1);

  return 0;
}
