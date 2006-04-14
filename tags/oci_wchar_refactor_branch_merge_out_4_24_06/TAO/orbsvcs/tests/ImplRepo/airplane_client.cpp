// $Id$

#include "airplane_client_i.h"

#include "ace/Log_Msg.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID (ImplRepo, 
           airplane_client, 
           "$Id$")

// This function runs the test.

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  ACE_Argv_Type_Converter convert (argc, argv);

  Airplane_Client_i client;

  ACE_DEBUG ((LM_DEBUG, "\n\tPaper Airplane Client\n\n"));

  if (client.init (convert.get_argc(), convert.get_ASCII_argv()) == -1)
    return 1;
  else
    return client.run ();
}
