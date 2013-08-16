// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Daemon_Utilities.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "orbsvcs/CosEvent/CEC_Event_Loader.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_main.h"

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  TAO_CEC_Default_Factory::init_svcs ();

  try
    {
       // Check if -ORBDaemon is specified and if so, daemonize at this moment,
       // -ORBDaemon in the ORB core is faulty, see bugzilla 3335
      TAO_Daemon_Utility::check_for_daemon (argc, argv);

      // Copy command line parameter.
      ACE_Argv_Type_Converter command_line(argc, argv);

      // Intialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (command_line.get_argc(), command_line.get_TCHAR_argv());

      // Call TAO_CEC_Event_Loader::init (argc, argv) from here.
      TAO_CEC_Event_Loader event_service;

      // To intialise the service
      int const result =
        event_service.init (command_line.get_argc(), command_line.get_TCHAR_argv());

      if (result == -1)
        return 1;

      // @@ Run forever... even after the EC is destroyed...
      //    The "Right Way"[tm] to fix this is to modify
      //    TAO_CEC_EventChannel to get a new option.. If the option is
      //    set then we destroy the ORB on EventChannel::destroy(),
      //    otherwise we don't.
      //    The option will not be set by default, but the user can pass
      //    it in the command line, and in this main program with set it
      //    to 1 by default or something like that...
      //    Don't worry about this change yet... Let's get all the changes
      //    in and then we can fix the EC shutdown problem...
      //
      //    As inidicated above, The Typed EC implementation can now be
      //    destroyed by passing -d at the command line and
      //    calling destroy on the typed EC interface.
      //    Calling fini() completes the destruction, although most of
      //    this is done in TAO_CEC_TypedEventChannel::shutdown().
      orb->run ();

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
      event_service.fini();
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

      // Destroy the ORB
      orb->destroy();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (argv[0]);
      return 1;
    }
  return 0;
}
