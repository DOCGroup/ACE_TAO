// $Id$

#include "Client_i.h"
#include "ace/Log_Msg.h"



// This function runs the Time Service client test.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Client_i client;

  ACE_DEBUG ((LM_DEBUG,
              "[CLIENT] Process/Thread Id : (%P/%t) Time Service Client\n"));

  if (client.init (argc, argv) == -1)
    return -1;
  else
    return client.run ();
}
