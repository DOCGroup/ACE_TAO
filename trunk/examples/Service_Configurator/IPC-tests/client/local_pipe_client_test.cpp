// Another test of UNIX domain IPC-SAP abstraction.  This one opens 2
// @(#)local_pipe_client_test.cpp	1.1	10/18/96

// pipes and then ships certain ends over to the server to act as a
// filter!

#include "ace/LSOCK_Connector.h"
#include "ace/UNIX_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

#if defined (ACE_HAS_MSG) && !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
/* Name of the program. */
static char *program_name;

/* Name of rendezvous point. */
static char *rendezvous = "/tmp/foo_pipe";

/* Name of file to send. */
static char *file_name = "local_data";

static void 
print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR, "usage: %s [-r rendezvous] [-f file]\n%a",
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
do_client_processing (ACE_LSOCK_Stream &sc)
{
  int	fd_read[2];
  int	fd_write[2];
  char	buf[BUFSIZ];
  int	n;
  int	fd1;

  if (ACE_OS::pipe (fd_read) == -1 || ACE_OS::pipe (fd_write) == -1)
    return -1;

  if (sc.send_handle (fd_write[0]) == -1 || sc.send_handle (fd_read[1]) == -1)
    return -1;

  /* Close off the ends we aren't interested in. */

  if (ACE_OS::close (fd_read[1]) || ACE_OS::close (fd_write[0]) == -1)
    return -1;

  /* Do a silly dup just for fun... */

  if ((fd1 = ACE_OS::open (file_name, O_RDONLY)) == -1)
    return -1;

  while ((n = ACE_OS::read (fd1, buf, sizeof buf)) > 0)
    {
      if (ACE_OS::write (fd_write[1], buf, n) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "write"), -1);
      if ((n = ACE_OS::read (fd_read[0], buf, sizeof buf)) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "read"), -1);
      if (ACE_OS::write (ACE_STDOUT, buf, n) == -1)
	return -1;
    }

  if (ACE_OS::close (fd_read[0]) == -1 
      || ACE_OS::close (fd_write[1]) == -1 
      || ACE_OS::close (fd1) == -1)
    ACE_OS::exit (1);

  return 0;
}

int
main (int argc, char *argv[])
{
  parse_args (argc, argv);

  ACE_LSOCK_Stream sc;
  ACE_LSOCK_Connector con;

  if (con.connect (sc, ACE_UNIX_Addr (rendezvous)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "connect"), -1);

  if (do_client_processing (sc) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "do_client_processing"), -1);

#if defined (SunOS4)
  ACE_OS::sleep (1);
#endif /* SunOS4 */

  if (sc.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), -1);

  return 0;
}
#else
int main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform must support sendmsg/recvmsg to run this test\n"), -1);
}
#endif /* ACE_HAS_MSG */
