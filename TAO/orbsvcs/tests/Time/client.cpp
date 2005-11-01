// $Id$

#include "Client_i.h"
#include "ace/Log_Msg.h"

ACE_RCSID(Time, client, "$Id$")

// This function runs the Time Service client test.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  Client_i client;

  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Time Service Client\n"));

  if (client.init (convert.get_argc(), convert.get_ASCII_argv()) == -1)
    return -1;
  else
    return client.run ();
}
