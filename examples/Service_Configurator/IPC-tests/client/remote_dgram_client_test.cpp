// $Id$

// Tests out the Internet domain IPC-SAP dgram abstraction.

#include "ace/OS_main.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Mem_Map.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

// Name of the program.
static const ACE_TCHAR *program_name;

// Port number to use.
static u_short port_number = ACE_DEFAULT_SERVER_PORT;

// Name of remote host.
static const ACE_TCHAR *host_name = ACE_DEFAULT_SERVER_HOST;

// Name of file to send.
static const ACE_TCHAR *file_name = ACE_TEXT("./remote_data");

static void print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR,
              "usage: %s [-p portnum] [-h host_name] [-f file]\n",
              program_name));
  ACE_OS::exit (1);
}

void
parse_args (int argc, ACE_TCHAR *argv[])
{
  program_name = argv[0];

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("f:h:p:"));

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
        case 'f':
          file_name = get_opt.opt_arg ();
          break;
        case 'h':
          host_name = get_opt.opt_arg ();
          break;
        case 'p':
          port_number = ACE_OS::atoi (get_opt.opt_arg ());
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

  ACE_SOCK_Dgram sd (ACE_Addr::sap_any);
  void *cp = 0;
  ACE_INET_Addr sa (port_number, host_name);

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Mem_Map mmap (file_name);

  if (mmap (cp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "mmap"),
                      -1);
  //FUZZ: enable check_for_lack_ACE_OS

  // Next, send the file's contents.

  ssize_t cc = sd.send (cp, mmap.size (), sa);

  if (cc == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "send"),
                      -1);
  else if (cc != (ssize_t) mmap.size ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "Not all the contents of mmap file are sent."),
                      -1);
  return 0;
}
