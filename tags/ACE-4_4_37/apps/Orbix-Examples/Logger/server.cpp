// server.C
// $Id$


// The server for the logger example.
// This uses the TRY,CATCHANY,ENDTRY macros for error testing.

// The executable file generated from this code should be registered 
// (under the name 'logger') using the 'putit' command.

#include <iostream.h>
#include "logger_i.h"

int
main (int, char *[]) 
{
  // Tell the server not to hang up while clients are connected.
  CORBA::Orbix.setNoHangup (1);

  // create a logger object - using the implementation class logger_i
#if defined (USE_BOA_IMPL)
  profile_logger_i profile_logger;
#else
  TIE_profile_logger (profile_logger_i) profile_logger (new profile_logger_i);
#endif /* USE_BOA_IMPL */

  TRY {
    // tell Orbix that we have completed the server's initialisation:
    CORBA::Orbix.impl_is_ready (profile_logger_IMPL, IT_X);
  } CATCHANY {
    // an error occured calling impl_is_ready () - output the error.
    cout << IT_X << endl;
  } ENDTRY;

  // impl_is_ready() returns only when Orbix times-out an idle server
  // (or an error occurs).
  cerr << "server exiting" << endl;

  return 0;
}
