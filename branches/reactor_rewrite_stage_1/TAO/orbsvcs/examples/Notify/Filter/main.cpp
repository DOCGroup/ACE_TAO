// -*- C++ -*-
// $Id$

#include "Filter.h"

int
main (int argc, char *argv [])
{
  FilterClient client;

  ACE_TRY_NEW_ENV
    {
      client.init (argc, argv
                   TAO_ENV_ARG_PARAMETER); //Init the Client
      ACE_TRY_CHECK;

      client.run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "TLS_Client user error: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "Filter system error: ");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
