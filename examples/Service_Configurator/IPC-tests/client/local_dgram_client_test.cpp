/* Tests out the UNIX domain IPC-SAP abstraction. */
// $Id$


#include "ace/LSOCK_CODgram.h"
#include "ace/LSOCK_Dgram.h"
#include "ace/UNIX_Addr.h"

#include "ace/Get_Opt.h"

#if defined (ACE_HAS_MSG) && !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
/* Name of the program. */
static char *program_name;

/* Number of seconds to sleep. */
static int seconds = 3;

/* Name of rendezvous point. */
static char *rendezvous_codgram = "/tmp/foo_codgram";
static char *rendezvous_dgram	= "/tmp/foo_dgram";

/* Name of file to send. */
static char *file_name = "local_data";

static void print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR, "usage: %s [-r rendezvous_dgram] [-c rendezvous_codgram] [-f file] [-n seconds]\n%a", 
	      program_name, -1));
}

void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "c:f:n:r:");

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
    {
    case 'f':
      file_name = get_opt.optarg;
      break;
    case 'n':
      seconds = atoi (get_opt.optarg);
      break;
    case 'r':
      rendezvous_dgram = get_opt.optarg;
      break;
    case 'c':
      rendezvous_codgram = get_opt.optarg;
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

int
main (int argc, char *argv[])
{
  program_name = argv[0];

  parse_args (argc, argv);

  ACE_LSOCK_Dgram   sd (ACE_Addr::sap_any);
  ACE_LSOCK_CODgram sc;
  int		fd;

  if (sc.open (ACE_UNIX_Addr (rendezvous_codgram), ACE_Addr::sap_any) < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  if ((fd = ACE_OS::open (file_name, O_RDONLY)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

      /* Send the open file descriptor to the server! */

  if (sc.send_handle (fd) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), -1);

  char *name = ACE_OS::cuserid (0);

  if (sd.send (name, strlen (name) + 1, ACE_UNIX_Addr (rendezvous_dgram)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), -1);

  if (ACE_OS::close (fd) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), -1);

  return 0;
}
#else
int main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform must support sendmsg/recvmsg to run this test\n"), -1);
}
#endif /* ACE_HAS_MSG */
