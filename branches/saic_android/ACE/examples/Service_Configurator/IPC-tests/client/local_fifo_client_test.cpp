// $Id$

// Send a file through ACE_FIFO communication channel by
// breaking it (the file) into pieces.

#include "ace/OS_main.h"
#include "ace/Mem_Map.h"
#include "ace/FIFO_Send_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_stropts.h"

// Name of the program.
static ACE_TCHAR *program_name;

// debug state on or off
static int debug = 0;

static const ACE_TCHAR *rendezvous_fifo = ACE_TEXT("/tmp/foo_fifo");

// Name of file to send.
static const ACE_TCHAR *file_name = ACE_TEXT("./local_data");

static void
print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR,
              "usage: %s [-d] [-f rendezvous_fifo]\n",
              program_name));
  ACE_OS::exit (1);
}

static void
parse_arguments (int argc, ACE_TCHAR *argv[])
{
  int tracing = 1;
  program_name = argv[0];

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("df:"));

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
    {
    case 'd':
      debug = 1;
      break;
    case 'f':
      rendezvous_fifo = get_opt.opt_arg ();
      break;
    default:
      print_usage_and_die ();
      break;
  }

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "rendezvous_fifo = %s\n"
                "trace = %s\n",
                rendezvous_fifo,
                tracing ? "on" : "off"));
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  parse_arguments (argc, argv);

  ACE_FIFO_Send_Msg fifo;

  if (fifo.open (rendezvous_fifo,
                 O_WRONLY,
                 0666) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open %s for requesting a new communication channel"
                       "in local_fifo_client_test\n",
                       rendezvous_fifo),
                      -1);
  void *cp = 0;

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Mem_Map mmap (file_name);

  if (mmap (cp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "mmap"),
                      -1);
  //FUZZ: enable check_for_lack_ACE_OS

  // Next, send the file's contents.

  ACE_Str_Buf msg (cp,
                   int (mmap.size ()));

  if (fifo.send (msg) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "send"),
                      -1);
  return 0;
}
