// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    Entry point for the chat server.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#include "Server_i.h"

int
main (int argc, char *argv[])
{
  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
      Server_i server_i;

      if (server_i.init (argc, argv, TAO_TRY_ENV) != 0)
	{
	  TAO_CHECK_ENV;

	  ACE_ERROR_RETURN ((LM_ERROR,
			     "\n error in init.\n"),
			    1);
	}
      if (server_i.run (TAO_TRY_ENV) != 0)
	{
	  TAO_CHECK_ENV;

	  ACE_ERROR_RETURN ((LM_ERROR,
			     "\n error in run.\n"),
			    1);
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("server::main\t\n");
      return 1;
    }
  TAO_ENDTRY;

  return 0;
}
