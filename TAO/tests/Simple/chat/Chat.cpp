// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    Chat.cpp
//
// = DESCRIPTION
//    The Chat client program entry point.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#include "Chat_i.h"

int
main(int argc, char* argv[])
{
  printf("\n============= Simple Chat ===========\n");

  char *nick;
  char def_nick[] = "chat client";

  if (argc >=2 )
    {
      nick = argv[1];
    }
  else
    {
      printf("\n usage: Chat <nickname>\n");
      nick = def_nick;
    }

  TAO_TRY
    {
      Chat_i chat_i ("chat.ior", nick);

      if (chat_i.init (argc, argv) == -1 || chat_i.run() == -1)
	{
	  return 1;
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("chat::main\t\n");
      return 1;
    }
  TAO_ENDTRY;

  return 0;
}
