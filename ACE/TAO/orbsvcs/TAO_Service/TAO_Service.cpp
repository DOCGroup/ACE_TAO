// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs/TAO_Service/
//
// = FILENAME
//   TAO_Service.cpp
//
// = DESCRIPTION
//   This directory contains an example that illustrates how the ACE
//   Service Configurator can dynamically configure an ORB and its
//   servants from a svc.conf file.
//
// = AUTHOR
//     Priyanka Gontla <pgontla@ece.uci.edu>
//
// ============================================================================

#include "tao/ORB_Constants.h"
#include "tao/ORB.h"
#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"
#include "ace/Signal.h"
#include "ace/Time_Value.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID (TAO_Service,
           TAO_Service,
           "$Id$")


extern "C" void handler (int)
{
  ACE_Service_Config::reconfig_occurred (1);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      ACE_Argv_Type_Converter argcon (argc, argv);
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argcon.get_argc (), argcon.get_ASCII_argv (),
                         "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGHUP);

      for (;;)
        {
          ACE_Time_Value tv (5, 0);

          orb->perform_work (tv ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG,
                      "Reconfig flag = %d\n",
                      ACE_Service_Config::reconfig_occurred ()));

          if (ACE_Service_Config::reconfig_occurred ())
            ACE_Service_Config::reconfigure ();
        }

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, argv[0]);
    }
  ACE_ENDTRY;

  return -1;
}
