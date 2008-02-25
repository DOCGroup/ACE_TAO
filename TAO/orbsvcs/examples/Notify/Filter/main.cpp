// -*- C++ -*-
// $Id$

#include "Filter.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  FilterClient client;

  try
    {
      client.init (argc, argv); //Init the Client

      client.run ();
    }
  catch (const CORBA::UserException& ue)
    {
      ue._tao_print_exception ("TLS_Client user error: ");
      return 1;
    }
  catch (const CORBA::SystemException& se)
    {
      se._tao_print_exception ("Filter system error: ");
      return 1;
    }

  return 0;
}
