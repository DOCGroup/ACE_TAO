// $Id$

// Tests out the Internet domain IPC-SAP dgram abstraction.

#include "ace/Mem_Map.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"
#include "ace/Get_Opt.h"

ACE_RCSID(client, remote_dgram_client_test, "$Id$")

/* Name of the program. */
static char *program_name;

/* Port number to use. */
static unsigned short port_number = ACE_DEFAULT_SERVER_PORT;

/* Name of remote host. */
static char *host_name = ACE_DEFAULT_SERVER_HOST;

/* Name of file to send. */
static char  *file_name = "./remote_data";

static void print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR, "usage: %s [-p portnum] [-h host_name] [-f file]\n%a",
	     program_name, -1));
}

void
parse_args (int argc, char *argv[])
{
  program_name = argv[0];

  ACE_Get_Opt get_opt (argc, argv, "f:h:p:");

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'f':
	file_name = get_opt.optarg;
	break;
      case 'h':
	host_name = get_opt.optarg;
	break;
      case 'p':
	port_number = ACE_OS::atoi (get_opt.optarg);
	break;
      default:
	print_usage_and_die ();
	break;
      }
}

int
main (int argc, char *argv[])
{
  parse_args (argc, argv);

  ACE_SOCK_Dgram sd (ACE_Addr::sap_any);
  void *cp;
  ACE_INET_Addr  sa (port_number, host_name);

  ACE_Mem_Map mmap (file_name);

  if (mmap (cp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "mmap"), -1);
      
      /* Next, send the file's contents. */

  ssize_t cc = sd.send (cp, mmap.size (), sa);

  if (cc == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), -1);
  else if (cc != (ssize_t) mmap.size ())
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", 
		       "Not all the contents of mmap file are sent."), -1);
  return 0;
}
