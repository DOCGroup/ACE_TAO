/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FaultDetectorMain.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file provides the main routine for a process that
 *  implements the FaultDetectorFactory interface and manages
 *  a set of FaultDetectors.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "ace/pre.h"
#include "FT_FaultDetectorFactory_i.h"

#include "tao/PortableServer/ORB_Manager.h"

#ifdef PG_PS_UNIT_TEST
# include "orbsvcs/PortableGroup/PG_Properties_Encoder.h"
# include "orbsvcs/PortableGroup/PG_Properties_Decoder.h"
#endif //  PG_PS_UNIT_TEST

int main (int argc, ACE_TCHAR * argv[] )
{
  // create an instance of the factory and give it the first
  // chance at the arguments.
  FT_FaultDetectorFactory_i factory;
  int result = factory.parse_args (argc, argv);
  if (result == 0)
  {
    ACE_TRY_NEW_ENV
    {

      // Create an object that manages all the
      // details of being a server.  It, too, gets to see the command line.
      TAO_ORB_Manager orbManager;

      result = orbManager.init (argc, argv
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

        result = factory.self_register(orbManager);
        if (result == 0)
        {
          ACE_ERROR ((LM_ERROR,
            "%n\n%T: FaultDetectorFactory Ready %s\n", factory.identity()
            ));

          // Run the main event loop for the ORB.
          result = orbManager.run (ACE_ENV_SINGLE_ARG_PARAMETER);
          if (result == -1)
          {
            ACE_ERROR_RETURN (
              (LM_ERROR, "%n\n%T: FT_Replica_i::run error"),
              -1);
          }
          ACE_TRY_CHECK;
          ACE_ERROR ((LM_ERROR,
            "%n\n%T: Terminated normally. %s\n", factory.identity()
            ));
        }
        else
        {
          ACE_ERROR ((LM_ERROR,
            "%n\n%T: Write IOR failed: %p\n"
            ));
          result = -1;
        }
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
          "%n\n%T: orb manager init failed\n"
        ));
        result = -1;
      }
    }
    ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
        "Fault Detector Factory::main\t\n");
      result = -1;
    }
    ACE_ENDTRY;
  }
  return result;
}


