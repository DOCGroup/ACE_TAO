// $Id$

#include "server_i.h"

int main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      Server_i svr_i;

      int init_result;

      init_result = svr_i.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (init_result != 0)
        return 1;

    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
            ACE_PRINT_EXCEPTION (ex, "CORBA exception raised in server!");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
