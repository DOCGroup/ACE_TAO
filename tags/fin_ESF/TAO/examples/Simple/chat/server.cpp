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
  ACE_TRY_NEW_ENV
    {
      Server_i server_i;

      if (server_i.init (argc, argv, ACE_TRY_ENV) != 0)
	{
	  ACE_TRY_CHECK;

	  ACE_ERROR_RETURN ((LM_ERROR,
			     "\n error in init.\n"),
			    1);
	}
      if (server_i.run (ACE_TRY_ENV) != 0)
	{
	  ACE_TRY_CHECK;

	  ACE_ERROR_RETURN ((LM_ERROR,
			     "\n error in run.\n"),
			    1);
	}
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "server::main\t\n");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
