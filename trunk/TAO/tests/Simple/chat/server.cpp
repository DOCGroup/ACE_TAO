// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    Server.cpp
//
// = DESCRIPTION
//    Entry point for the chat server.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#include "server_i.h"

int
main (int argc, char *argv[])
{
  // @@ Please make the "chat.ior" be something that can be overridden
  // by a user.

  Server_i server_i ("chat.ior");

  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
		  "Running chat server...\n"));

      if (server_i.init (argc, argv, TAO_TRY_ENV) != 0
	  || server_i.run (TAO_TRY_ENV) != 0)
	{
	  ACE_DEBUG((LM_DEBUG, "\n error in init or run.\n"));
	  return 1;
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
