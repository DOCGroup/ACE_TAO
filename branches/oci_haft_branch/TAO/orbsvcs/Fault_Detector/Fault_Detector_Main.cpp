// $Id$
//FaultDetectorMain.cpp

#include "Fault_Detector.h"
#include "FT_DetectorFactory_i.h"

#include "tao/PortableServer/ORB_Manager.h"

#include "PG_Property_Set_Helper.h"

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
        if ( Portable_Group::Property_Set::test_encode_decode () )
        {
          std::cout << "Passed property set self-test. " << std::endl;
        }

        result = factory.self_register(orbManager);
        if (result == 0)
        {
          std::cout << "FT Replica: Ready. ";
          factory.identity(std::cout);
          std::cout << std::endl;

          // Run the main event loop for the ORB.
          result = orbManager.run (ACE_ENV_SINGLE_ARG_PARAMETER);
          if (result == -1)
          {
            ACE_ERROR_RETURN (
              (LM_ERROR, "FT_Replica_i::run"),
              -1);
          }
          ACE_TRY_CHECK;
          std::cout << "FT Replica: Terminated normally.";
          factory.identity(std::cout);
          std::cout << std::endl;
        }
        else
        {
          ACE_ERROR_RETURN (
            (LM_ERROR, "%p\n", "Write IOR failed\n"),
            -1);
        }
      }
      else
      {
        ACE_ERROR_RETURN (
          (LM_ERROR, "%p\n", "orb manager init failed\n"),
          -1);
      }
    }
    ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
        "server::main\t\n");
      result = -1;
    }
    ACE_ENDTRY;
  }
  return result;
}
