/* Send a file through ACE_SPIPE communication channel by
// $Id$

   break it (the file) into pieces. */

#include "ace/Mem_Map.h"
#include "ace/SPIPE_Connector.h"

#include "ace/Get_Opt.h"

#if defined (ACE_HAS_STREAM_PIPES)

static char *program_name;

// debug state on or off
static int debug = 0;

char *rendezvous_spipe = "/tmp/foo_spipe";

/* Name of file to send. */
static char  *file_name = "./local_data";

static void 
print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR, "usage: %s [-d] [-r rendezvous_spipe]\n%a", 
	      program_name, -1));
}

static void
parse_arguments (int argc, char *argv[])
{
  program_name = argv[0];
  ACE_Get_Opt get_opt (argc, argv, "dr:");

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
    {
    case 'd':
      debug = 1;
      break;
    case 'r':
      rendezvous_spipe = get_opt.optarg;
      break;
    default:
      print_usage_and_die ();
      break;
  }
  if (debug)
    ACE_DEBUG ((LM_DEBUG, "rendezvous_spipe = %s\n", 
		rendezvous_spipe));
}

int
main(int argc, char *argv[])
{
  parse_arguments (argc, argv);

  ACE_SPIPE_Stream spipe;
  ACE_SPIPE_Connector con;

  if (con.connect (spipe, ACE_SPIPE_Addr (rendezvous_spipe)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "Cannot open %s for requesting a new communication channel"
		       " in local_spipe_client_test.\n", rendezvous_spipe), -1);

  ACE_Mem_Map mmap (file_name);
  void *cp;

  if (mmap (cp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "mmap"), -1);
      
  /* Next, send the file's contents. */

  ACE_Str_Buf msg (cp, int (mmap.size ()));

  if (spipe.send ((ACE_Str_Buf *) 0, &msg) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), -1);
  return 0;
}
#else
#include <stdio.h>
int 
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "This feature is not supported\n"), -1);
}
#endif /* ACE_HAS_STREAM_PIPES */
