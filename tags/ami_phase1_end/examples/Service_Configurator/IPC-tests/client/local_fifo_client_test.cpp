// $Id$

// Send a file through ACE_FIFO communication channel by
// breaking it (the file) into pieces.

#include "ace/Mem_Map.h"
#include "ace/FIFO_Send_Msg.h"
#include "ace/Get_Opt.h"

ACE_RCSID(client, local_fifo_client_test, "$Id$")

// Name of the program.
static char *program_name;

// debug state on or off
static int debug = 0;

char *rendezvous_fifo = "/tmp/foo_fifo";

/* Name of file to send. */
static char *file_name = "./local_data";

static void 
print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR, "usage: %s [-d] [-f rendezvous_fifo]\n%a", 
	      program_name, -1));
}

static void
parse_arguments (int argc, char *argv[])
{
  int tracing = 1;
  program_name = argv[0];

  ACE_Get_Opt get_opt (argc, argv, "df:");

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
    {
    case 'd':
      debug = 1;
      break;
    case 'f':
      rendezvous_fifo = get_opt.optarg;
      break;
    default:
      print_usage_and_die ();
      break;
  }

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
		"rendezvous_fifo = %s\n"
		"trace = %s\n",
		rendezvous_fifo, tracing ? "on" : "off"));
}

int
main(int argc, char *argv[])
{
  parse_arguments (argc, argv);

  ACE_FIFO_Send_Msg fifo;

  if (fifo.open ((const char *) rendezvous_fifo, O_WRONLY, 0666) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open %s for requesting a new communication channel"
		       "in local_fifo_client_test\n", rendezvous_fifo), -1);


  void *cp;
  ACE_Mem_Map mmap (file_name);

  if (mmap (cp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "mmap"), -1);
      
  /* Next, send the file's contents. */

  ACE_Str_Buf msg (cp, int (mmap.size ()));

  if (fifo.send (msg) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), -1);
  return 0;
}
