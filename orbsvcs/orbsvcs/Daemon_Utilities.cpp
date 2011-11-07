// $Id$

#include "orbsvcs/Daemon_Utilities.h"
#include "ace/Arg_Shifter.h"
#include "ace/ACE.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void TAO_Daemon_Utility::check_for_daemon (int &argc, ACE_TCHAR* argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      if (0 == arg_shifter.cur_arg_strncasecmp (ACE_TEXT ("-ORBDaemon")))
        {
          // Daemonize, parent exits, child returns here.
          ACE::daemonize();

          arg_shifter.consume_arg ();
        }
      // Can't interpret this argument.
      // Move on to the next argument.
      else
        {
          // Any arguments that don't match are ignored so
          // that the caller can still use them.
          arg_shifter.ignore_arg ();
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
