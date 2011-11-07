// -*- C++ -*-
// $Id$

#include "Latency_Query_Client.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Latency_Query_Client client;

  int retval = client.init (argc, argv);

  if (retval == -1)
    {
      return 1;
    }

  retval = client.run ();

  return retval;
}
