// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//    The Chat client program entry point.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#include "client_i.h"

int
main (int argc, char *argv[])
{
  char *nick;

  if (argc >= 2)
    nick = argv[1];
  else
    ACE_ERROR_RETURN ((LM_ERROR,
		       "\n usage: client <nickname>\n"),
		      -1);

  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
		 "\n============= Simple Chat ===========\n"));

      ACE_DEBUG ((LM_DEBUG,
		 "\n============= type 'quit' to exit  ===========\n"));

      // @@ Please make the "chat.ior" an option or something you can
      // override as a user.
      Client_i client_i ("chat.ior", nick);

      if (client_i.init (argc, argv) == -1
	  || client_i.run () == -1)
	return -1;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("client::main\t\n");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
