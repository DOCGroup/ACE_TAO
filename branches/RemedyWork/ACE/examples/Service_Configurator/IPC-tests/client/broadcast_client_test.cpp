// $Id$

// Tests out the broadcast service of the
// Internet domain IPC-SAP dgram abstraction.

#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

/* Name of the program. */
static ACE_TCHAR *program_name;

/* Port number to use. */
static unsigned short broadcast_port_number = ACE_DEFAULT_BROADCAST_PORT;

static void
print_usage_and_die (void)
{
  ACE_OS::fprintf (stderr, "usage: %s [-p broadcast portnum]\n",
                   program_name);
  ACE_OS::exit (1);
}

void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("p:"));

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'p':
        broadcast_port_number = ACE_OS::atoi (get_opt.opt_arg ());
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

  ACE_SOCK_Dgram_Bcast sd (ACE_Addr::sap_any);

  static char buf[] = "testing socket broadcast service";

  if (sd.send (buf, ACE_OS::strlen (buf), broadcast_port_number) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "can't send broadcast"), -1);

  return 0;
}
