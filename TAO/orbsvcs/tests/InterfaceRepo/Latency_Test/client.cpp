// -*- C++ -*-
#include "Latency_Query_Client.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Latency_Query_Client client;

  if (client.init (argc, argv) == -1)
    {
      return 1;
    }

  return client.run ();
}
