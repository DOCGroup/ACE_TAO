// Test the client-side of the ACE Name Server...
// $Id$

#include "ace/Service_Config.h"
#include "Dump_Restore.h"

int 
main (int argc, char *argv[])
{
  ACE_Service_Config daemon (argv[0]);

  ACE_DEBUG ((LM_DEBUG, "entering main\n"));

  // Get a handler
  Dump_Restore client_handler (argc, argv);

  for (;;)
    daemon.run_reactor_event_loop ();
  
  /* NOTREACHED */
  ACE_DEBUG ((LM_DEBUG, "leaving main\n"));
  return 0;
}
