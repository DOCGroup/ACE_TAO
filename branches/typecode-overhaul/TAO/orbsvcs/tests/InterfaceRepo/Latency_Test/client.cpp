// -*- C++ -*-
// $Id$

#include "Latency_Query_Client.h"

ACE_RCSID (Latency__Test, 
           client, 
           "$Id$")

int 
main (int argc, char *argv[])
{
  Latency_Query_Client client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\t IFR Latency Test \n\n"));

  int retval = client.init (argc, 
                            argv);

  if (retval == -1)
    {
      return 1;
    }

  retval = client.run ();

  return retval;
}
