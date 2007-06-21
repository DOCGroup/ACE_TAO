// -*- C++ -*-
// $Id$

#include "ifr_dii_client.h"

ACE_RCSID (Application_Test,
           client,
           "$Id$")

int
main (int argc, char *argv[])
{
  IFR_DII_Client client;

  try
    {
      if (client.init (argc,
                       argv)
           == -1)
        {
          return 1;
        }
      else
        {
           int status = client.run ();

           if (status == -1)
           {
              return 1;
           }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client Exception");
      return -1;
    }

  return 0;
}
