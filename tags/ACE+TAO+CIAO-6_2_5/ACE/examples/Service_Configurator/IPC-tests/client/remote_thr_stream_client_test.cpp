// $Id$

// Test program for the INET ACE_TLI-SAPs...

#include "ace/OS_main.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Mem_Map.h"
#include "ace/TLI_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

#if defined (ACE_HAS_TLI)

// Name of the program.
static const ACE_TCHAR *program_name;

// Port number to use.
static u_short port_number = ACE_DEFAULT_THR_PORT;

// Name of remote host.
static const ACE_TCHAR *host_name = ACE_DEFAULT_SERVER_HOST;

// Name of file to send.
static const ACE_TCHAR *file_name = ACE_TEXT ("./remote_data");

static void print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("usage: %s [-p portnum] [-h host_name] [-f file]\n"),
              program_name));
  ACE_OS::exit (1);
}

void
parse_args (int argc, ACE_TCHAR *argv[])
{
  program_name = argv[0];
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("f:h:p:"));

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
  void *cp;
  char buf[BUFSIZ];
  ACE_TLI_Stream sc;
  ACE_TLI_Connector con;

  if (con.connect (sc,
                   ACE_INET_Addr (port_number,
                                  host_name)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("open")),
                      -1);

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Mem_Map mmap (file_name, PROT_READ);

  if (mmap (cp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("mmap")), -1);
  //FUZZ: enable check_for_lack_ACE_OS

  // Next, send the file's contents.

  if (sc.send_n (cp, mmap.size ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("send_n")),
                       -1);

  if (sc.sndrel () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("close_writer")),
                      -1);

  for (int n; (n = sc.recv (buf, sizeof buf)) > 0; )
    if (ACE_OS::write (ACE_STDOUT, buf, n) != n)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("write")),
                        -1);

  if (sc.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("close")),
                      -1);
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("you must have TLI to run this test\n")),
                    1);
}
#endif /* ACE_HAS_TLI */
