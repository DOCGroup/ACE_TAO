// $Id$

#include "JAWS/Server.h"
#include "HTTP_10.h"

int
main (int argc, char *argv[])
{
  JAWS_Server server (argc, argv);
  HTTP_Pipeline HTTP;

  if (server.open (&HTTP) == -1)
    ACE_DEBUG ((LM_DEBUG, "JAWS: Error openning server\n"));

  return 0;
}
