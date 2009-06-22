// $Id$

#include "nestea_client_i.h"

#include "ace/Log_Msg.h"

ACE_RCSID (ImplRepo,
           nestea_client,
           "$Id$")

// This function runs the test.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Nestea_Client_i client;

  ACE_DEBUG ((LM_DEBUG, "\n\tNestea Bookshelf Client\n\n"));

  if (client.init (argc, argv) == -1)
    return 1;
  else
    return client.run ();
}

