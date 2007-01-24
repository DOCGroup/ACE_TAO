// $Id$

#include "server_i.h"

int main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      Server_i svr_i;

      const int init_result = svr_i.init (argc, argv ACE_ENV_ARG_PARAMETER);

      if (init_result != 0)
        return 1;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "CORBA exception raised in server!");
    }
  ACE_ENDTRY;

  return 0;
}
