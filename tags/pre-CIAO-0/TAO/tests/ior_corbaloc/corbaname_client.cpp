
//=============================================================================
/**
 *  @file    corbaname_client.cpp
 *
 *  $Id$
 *
 *   This implements a simple CORBA client to tests the
 *   corbaname: style IOR parser
 *
 *
 *  @author  Craig Rodrigues <crodrigu@bbn.com>
 */
//=============================================================================


#include "ior_corbaloc_client_i.h"

int main (int argc, char *argv [])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "" /* the ORB name, it can be anything! */
                         ACE_ENV_ARG_PARAMETER);

      if(argc < 2 )
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            "Usage:\n  %s [corbaname URL]\n",
                            argv[0]),
                           1);
        }

      // Get an object reference using a corbaname: style URL
      CORBA::Object_var obj =
        orb->string_to_object (argv[1]
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow
      corbaloc::Status_var factory =
        corbaloc::Status::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Object reference <%s> is nil\n",
                             argv[1]),
                            -1);
        }

      // Invoke a request on the server
      CORBA::Boolean ret_value =
        factory->print_status (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ret_value != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "The server has not been contacted. Error!!\n",
                      0));
        }

      return 0;
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      //
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
