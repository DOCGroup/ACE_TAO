// $Id$

#include "server_i.h"

ACE_RCSID(IDL_Cubit, server, "$Id$")

int
main (int argc, char *argv[])
{
  Cubit_Server cubit_server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_Cubit:SERVER\n\n"));
  if (cubit_server.init (argc, argv) == -1)
        return 1;
  else
    {
      cubit_server.run ();
    }
  return 0;
}
