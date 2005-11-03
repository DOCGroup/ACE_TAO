// -*- C++ -*-
// $Id$

#include "Latency_Query_Client.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID (Latency__Test, 
           client, 
           "$Id$")

int 
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);

  Latency_Query_Client client;

  int retval = client.init (convert.get_argc(), convert.get_ASCII_argv());

  if (retval == -1)
    {
      return 1;
    }

  retval = client.run ();

  return retval;
}
