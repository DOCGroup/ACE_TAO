/* Tests out the UNIX domain IPC-SAP abstraction. */
// @(#)local_stream_client_test.cpp	1.1	10/18/96

#include "ace/LSOCK_Connector.h"
#include "ace/UNIX_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

#if defined (ACE_HAS_MSG) && !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
/* Name of the program. */
static char *program_name;

/* Name of rendezvous point. */
static char *rendezvous = "/tmp/foo_stream";

/* Name of file to send. */
static char *file_name = "local_data";

static void 
print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR, "usage: %s [-r rendezvous] [-f file]%a\n",
	     program_name, -1));
}

void
parse_args (int argc, char *argv[])
{
  program_name = argv[0];
  ACE_Get_Opt get_opt (argc, argv, "f:r:");

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'f':
	file_name = get_opt.optarg;
	break;
      case 'r':
	rendezvous = get_opt.optarg;
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

  int fd;
  char buf[BUFSIZ];
  int n;

  ACE_LSOCK_Stream sc;
  ACE_LSOCK_Connector con;

  if (con.connect (sc, ACE_UNIX_Addr (rendezvous)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "connect"), -1);

  if ((fd = ACE_OS::open (file_name, O_RDONLY)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

      /* Send the open file descriptor to the server! */

  if (sc.send_handle (fd) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send_handle"), -1);

  if ((n = sc.recv_n (buf, sizeof buf)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "recv"), -1);
  else
    ACE_OS::write (ACE_STDOUT, buf, n);

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
