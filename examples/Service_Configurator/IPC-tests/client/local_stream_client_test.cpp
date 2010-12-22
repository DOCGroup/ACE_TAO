// $Id$

// Tests out the UNIX domain IPC-SAP abstraction.

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
static const ACE_TCHAR *rendezvous = ACE_TEXT ("/tmp/foo_stream");

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

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  parse_args (argc, argv);

  int fd;
  char buf[BUFSIZ];
  int n;

  ACE_LSOCK_Stream sc;
  ACE_LSOCK_Connector con;

  if (con.connect (sc,
                   ACE_UNIX_Addr (rendezvous)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("connect")),
                      -1);

  if ((fd = ACE_OS::open (file_name,
                          O_RDONLY)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("open")),
                      -1);

  // Send the open file descriptor to the server!

  if (sc.send_handle (fd) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("send_handle")),
                      -1);

  if ((n = sc.recv_n (buf,
                      sizeof buf)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("recv")),
                      -1);
  else
    ACE_OS::write (ACE_STDOUT, buf, n);

  if (ACE_OS::close (fd) == -1)
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
