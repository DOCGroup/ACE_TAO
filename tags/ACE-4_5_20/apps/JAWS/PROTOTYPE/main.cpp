// $Id$

#include "JAWS/Server.h"
#include "HTTP_10.h"

int
main (int argc, char *argv[])
{
  JAWS_Server server (argc, argv);
  JAWS_HTTP_10_Read_Task HTTP_Read;
  JAWS_HTTP_10_Parse_Task HTTP_Parse;
  JAWS_HTTP_10_Write_Task HTTP_Write;

  HTTP_Read.next (&HTTP_Parse);
  HTTP_Parse.next (&HTTP_Write);

  if (server.open (&HTTP_Read) == -1)
    ACE_DEBUG ((LM_DEBUG, "JAWS: Error openning server\n"));

  return 0;
}
