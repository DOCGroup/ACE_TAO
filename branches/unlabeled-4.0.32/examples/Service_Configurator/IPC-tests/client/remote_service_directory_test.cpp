/* Test program for the INET IPC-SAPs... */
// @(#)remote_service_directory_test.cpp	1.1	10/18/96


#include "ace/Log_Msg.h"
#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/Get_Opt.h"

/* Port number to use. */
static unsigned short port_number = ACE_DEFAULT_SERVICE_PORT;

/* Name of remote host. */
static char *host_name = ACE_DEFAULT_SERVER_HOST;

/* Trigger a remote reconfiguration */
static int remote_reconfigure = 0;

static void 
print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR, "usage: %n [-p portnum] [-h host_name] [-r] [-f file]\n%a", 1));
}

void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "p:h:r:");

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
    {
    case 'h':
      host_name = get_opt.optarg;
      break;
    case 'p':
      port_number = ACE_OS::atoi (get_opt.optarg);
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
main (int argc, char *argv[]) 
{ 
  ACE_LOG_MSG->open (argv[0]);

  parse_args (argc, argv);
  static char buf[BUFSIZ] = "help";
  int n;
  ACE_SOCK_Stream   sc;
  ACE_SOCK_Connector con;

  if (con.connect (sc, ACE_INET_Addr (port_number, host_name)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n%a", "connect", 1), -1);

  if (remote_reconfigure)
    ACE_OS::strcpy (buf, "reconfigure");

  /* Send the command */

  if (sc.send_n (buf, ACE_OS::strlen (buf) + 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n%a", "send", 1), -1);

  /* Next, read the response. */

  while ((n = sc.recv (buf, sizeof buf)) > 0)
    if (ACE_OS::write (ACE_STDOUT, buf, n) != n)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n%a", "write", 1), -1);
	 
  if (sc.close () == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n%a", "close", 1), -1);
    
  return 0;
}
