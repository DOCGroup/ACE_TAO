// $Id$

// Another test of UNIX domain IPC-SAP abstraction.  This one opens 2
// pipes and then ships certain ends over to the server to act as a
// filter!

#include "ace/OS_main.h"
#include "ace/LSOCK_Connector.h"
#include "ace/UNIX_Addr.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"

#if defined (ACE_HAS_MSG) && !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
// Name of the program.
static ACE_TCHAR *program_name;

// Name of rendezvous point.
static const ACE_TCHAR *rendezvous = ACE_TEXT ("/tmp/foo_pipe");

// Name of file to send.
static const ACE_TCHAR *file_name = ACE_TEXT ("local_data");

static void
print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("usage: %s [-r rendezvous] [-f file]\n"),
              program_name));
  ACE_OS::exit (1);
}

void
parse_args (int argc, ACE_TCHAR *argv[])
{
  program_name = argv[0];

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("f:r:"));

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
    {
    case 'f':
      file_name = get_opt.opt_arg ();
      break;
    case 'r':
      rendezvous = get_opt.opt_arg ();
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

static int
do_client_processing (ACE_LSOCK_Stream &sc)
{
  ACE_HANDLE fd_read[2];
  ACE_HANDLE fd_write[2];
  char buf[BUFSIZ];
  int n;

  if (ACE_OS::pipe (fd_read) == -1 || ACE_OS::pipe (fd_write) == -1)
    return -1;

  if (sc.send_handle (fd_write[0]) == -1 || sc.send_handle (fd_read[1]) == -1)
    return -1;

  // Close off the ends we aren't interested in.

  if (ACE_OS::close (fd_read[1]) || ACE_OS::close (fd_write[0]) == -1)
    return -1;

  // Do a silly dup just for fun...

  ACE_HANDLE fd1 = ACE_OS::open (file_name, O_RDONLY);

  if (fd1 == ACE_INVALID_HANDLE)
    return -1;

  while ((n = ACE_OS::read (fd1, buf, sizeof buf)) > 0)
    {
      if (ACE_OS::write (fd_write[1],
                         buf,
                         n) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("write")),
                          -1);
      if ((n = ACE_OS::read (fd_read[0],
                             buf,
                             sizeof buf)) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("read")),
                          -1);
      if (ACE_OS::write (ACE_STDOUT,
                         buf,
                         n) == -1)
        return -1;
    }

  if (ACE_OS::close (fd_read[0]) == -1
      || ACE_OS::close (fd_write[1]) == -1
      || ACE_OS::close (fd1) == -1)
    ACE_OS::exit (1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  parse_args (argc, argv);

  ACE_LSOCK_Stream sc;
  ACE_LSOCK_Connector con;

  if (con.connect (sc,
                   ACE_UNIX_Addr (rendezvous)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("connect")),
                      -1);

  if (do_client_processing (sc) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("do_client_processing")),
                      -1);

  if (sc.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("close")),
                      -1);

  return 0;
}
#else
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("your platform must support sendmsg/recvmsg to run this test\n")),
                    -1);
}
#endif /* ACE_HAS_MSG */
