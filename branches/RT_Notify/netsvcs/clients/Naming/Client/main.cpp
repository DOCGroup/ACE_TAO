// $Id$

// Test the client-side of the ACE Name Server...

#include "ace/Service_Config.h"
#include "ace/Naming_Context.h"
#include "ace/ARGV.h"
#include "ace/Log_Msg.h"

#include "Client_Test.h"

ACE_RCSID(Client, main, "$Id$")

int
main (int, char *argv[])
{
  ACE_Service_Config daemon;
  ACE_ARGV new_args;

  // Load the existing <argv> into our new one.
  new_args.add (argv);
  // Enable loading of static services.
  new_args.add ("-y"); 
  // Enable debugging within dynamically linked services.
  new_args.add ("-d"); 

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("argc = %d\n"),
              ACE_TEXT (new_args.argc ())));

  // Print the contents of the combined <ACE_ARGV>.
  for (size_t i = 0; i < new_args.argc (); i++)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%d) %s\n"),
                i,
                new_args.argv ()[i]));

  if (daemon.open (new_args.argc (),
                   new_args.argv ()) == -1)
    {
      if (errno != ENOENT)
	ACE_ERROR ((LM_ERROR,
                    "%p\n%a",
                    "open",
                    1));
      else // Use static binding.
	{
          ACE_ARGV args;

          args.add (argv[0]);
	  args.add ("-p10011"); // Port number.
	  ACE_Service_Object *so =
            ACE_SVC_INVOKE (ACE_Naming_Context);

	  if (so->init (args.argc (),
                        args.argv ()) == -1)
	    ACE_ERROR ((LM_ERROR,
                        "%p\n%a",
                        "ACE_Naming_Context",
                        1));

	  so = ACE_SVC_INVOKE (Client_Test);

	  if (so->init (0,
                        args.argv ()) == -1)
	    ACE_ERROR ((LM_ERROR,
                        "%p\n%a",
                        "Client_Test",
                        1));
	}
    }

  // Run forever, performing the configured services until we are shut
  // down by a SIGINT/SIGQUIT signal.

  ACE_Reactor::run_event_loop ();

  return 0;
}
