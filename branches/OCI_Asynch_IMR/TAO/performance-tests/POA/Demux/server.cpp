//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  Server-side driver program
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "demux_test_server.h"
#include "tao/debug.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  // Set TAO_debug_level to 0 to ensure silent running unless
  // debugging is explicitly invoked

  TAO_debug_level = 0;

  Demux_Test_Server demux_test_server;

  try
    {
      int r =
        demux_test_server.init (argc, argv);

      if (r == -1)
        return -1;

      demux_test_server.run ();
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }
  return 0;
}
