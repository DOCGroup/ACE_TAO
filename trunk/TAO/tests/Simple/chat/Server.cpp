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

#include "Server_i.h"

int
main (int argc, char *argv[])
{
  Server_i server_i("chat.ior");

  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
     ACE_OS::printf("Running chat server...");

     if (server_i.init(argc, argv, TAO_TRY_ENV) != 0 ||
	 server_i.run(TAO_TRY_ENV) != 0)
	 {
	    ACE_OS::printf("\n error in init or run.\n");
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
