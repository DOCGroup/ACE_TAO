// $Id$

// Tests out the UNIX domain IPC-SAP abstraction.

#include "ace/OS_main.h"
#include "ace/LSOCK_CODgram.h"
#include "ace/LSOCK_Dgram.h"
#include "ace/UNIX_Addr.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID(client, local_dgram_client_test, "$Id$")

#if defined (ACE_HAS_MSG) && !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
// Name of the program.
static char *program_name;

// Name of rendezvous point.
static const char *rendezvous_codgram = "/tmp/foo_codgram";
static const char *rendezvous_dgram = "/tmp/foo_dgram";

// Name of file to send.
static const char *file_name = "local_data";

static void
print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR,
              "usage: %s [-r rendezvous_dgram] [-c rendezvous_codgram] [-f file]\n",
	      program_name));
  ACE_OS::exit (1);
}

void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "c:f:r:");

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
    {
    case 'f':
      file_name = get_opt.opt_arg ();
      break;
    case 'r':
      rendezvous_dgram = get_opt.opt_arg ();
      break;
    case 'c':
      rendezvous_codgram = get_opt.opt_arg ();
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  program_name = argv[0];

  parse_args (argc, argv);

  char *sock_client = ACE_OS::tempnam (0, 0);

  ACE_LSOCK_Dgram sd ((ACE_UNIX_Addr) (sock_client));
  if (ACE_OS::unlink (sock_client))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "unlink"),
                      -1);
  ACE_OS::free ((void *) sock_client);

  ACE_LSOCK_CODgram sc;

  if (sc.open (ACE_UNIX_Addr (rendezvous_codgram),
               ACE_Addr::sap_any) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  ACE_HANDLE handle = ACE_OS::open (file_name, O_RDONLY);

  if (handle == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  // Send the open file descriptor to the server!

  if (sc.send_handle (handle) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "send"),
                      -1);

  char name[ACE_MAX_USERID];
  ACE_OS::cuserid (name);

  if (sd.send (name,
               ACE_OS::strlen (name) + 1,
               ACE_UNIX_Addr (rendezvous_dgram)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "send"),
                      -1);

  if (ACE_OS::close (handle) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "close"),
                      -1);
  return 0;
}
#else
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "your platform must support sendmsg/recvmsg to run this test\n"),
                    -1);
}
#endif /* ACE_HAS_MSG */
