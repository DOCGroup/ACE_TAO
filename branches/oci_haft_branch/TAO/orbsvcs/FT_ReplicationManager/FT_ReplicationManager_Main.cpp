/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_ReplicationManager_Main.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file provides the main routine for a process that
 *  implements the FT_ReplicationManager interface.
 *
 *  @author Curt Hibbs <hibbs_c@ociweb.com>
 */
//=============================================================================

#include "ace/pre.h"

#include "tao/PortableServer/ORB_Manager.h"
#include "orbsvcs/FT_ReplicationManager/FT_ReplicationManager.h"
#include "orbsvcs/FT_ReplicationManagerS.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"

#include "ace/Argv_Type_Converter.h"
#include "tao/PortableServer/ORB_Manager.h"

#ifdef PG_PS_UNIT_TEST
# include "orbsvcs/PortableGroup/PG_Properties_Encoder.h"
# include "orbsvcs/PortableGroup/PG_Properties_Decoder.h"
#endif //  PG_PS_UNIT_TEST



int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Copy command line parameter.
  // and hide it's unicodeness.
  ACE_Argv_Type_Converter command_line(argc, argv);

  char ** asciiArgv = command_line.get_ASCII_argv();

  // create an instance of the replication manager and give it the first
  // chance at the arguments.
  TAO::FT_ReplicationManager rm;
  int result = rm.parse_args (argc, asciiArgv);
  if (result == 0)
  {
    ACE_TRY_NEW_ENV
    {

      // Create an object that manages all the
      // details of being a server.  It, too, gets to see the command line.
      TAO_ORB_Manager orbManager;

      result = orbManager.init (argc, asciiArgv
          ACE_ENV_ARG_PARAMETER);
      if(result == 0)
      {
        ACE_CHECK_RETURN (-1);

// property set helper test
#ifdef PG_PS_UNIT_TEST
        if ( Portable_Group::Properties::test_encode_decode () )
        {
          ACE_ERROR ((LM_ERROR,
            "%n\n%T: Passed property set self-test.\n "
            ));
        }
#endif
        
        // initialize the replication manager
        result = rm.init(orbManager);
        if (result == 0)
        {
          ACE_ERROR ((LM_ERROR,
            "%n\n%T: ReplicationManager Ready %s\n", rm.identity()
            ));

          // Run the main event loop for the ORB.
          result = orbManager.run (ACE_ENV_SINGLE_ARG_PARAMETER);
          if (result == -1)
          {
            ACE_ERROR_RETURN (
              (LM_ERROR, "%n\n%T: FT::run error"),
              -1);
          }
          ACE_TRY_CHECK;
          ACE_ERROR ((LM_ERROR,
            "%n\n%T: Terminated normally. %s\n", rm.identity()
            ));
        }
        else
        {
          ACE_ERROR ((LM_ERROR,
            "%n\n%T: FT_ReplicationManager init failed: %p\n"
            ));
          result = -1;
        }
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
          "%n\n%T: ORB manager init failed\n"
        ));
        result = -1;
      }
    }
    ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
        "FT_ReplicationManager::main\t\n");
      result = -1;
    }
    ACE_ENDTRY;
  }
  return result;
}
