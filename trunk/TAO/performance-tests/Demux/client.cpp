// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Demux
//
// = FILENAME
//    client.cpp
//
//    client side main program
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================


#include "demux_test_client.h"

ACE_RCSID(CodeGen, client, "$Id$")

int
main (int argc, char *argv [])
{

  // Set TAO_debug_level to 0 to ensure silent running unless
  // debugging is explicitly invoked
  TAO_debug_level = 0;
  
  Demux_Test_Client demux_test_client; 
  
  TAO_TRY
    {
      if (demux_test_client.init (argc, argv, TAO_TRY_ENV) == -1)
        return -1;
      else
        {
          TAO_CHECK_ENV;

          demux_test_client.run (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}
