// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "orbsvcs/CosEvent/CEC_Event_Loader.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID(CosEvent_Service, CosEvent_Service, "$Id$")

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  TAO_CEC_Default_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Copy command line parameter.
      ACE_Argv_Type_Converter command_line(argc, argv);

      // Intialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (command_line.get_argc(), command_line.get_ASCII_argv(), 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Call TAO_CEC_Event_Loader::init (argc, argv) from here.
      TAO_CEC_Event_Loader event_service;

      // To intialise the service
      int result =
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
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Destroy the ORB
      orb->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, argv[0]);
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}
