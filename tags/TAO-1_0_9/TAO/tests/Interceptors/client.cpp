// $Id$

#include "ace/Get_Opt.h"
#include "testC.h"
#include "interceptors.h"

ACE_RCSID(Interceptors, client, "$Id$")

const char *ior = "file://test.ior";
POA_PortableInterceptor::ClientRequestInterceptor *interceptor_impl = 0;
PortableInterceptor::ClientRequestInterceptor_ptr interceptor = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "e");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'e':
      break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-v "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

void
run_test (Test_Interceptors::Visual_ptr server,
          CORBA::Environment &ACE_TRY_ENV)
{
  server->normal (10, ACE_TRY_ENV);
  ACE_CHECK;

  server->nothing (ACE_TRY_ENV);
  ACE_CHECK;

  ACE_TRY
    {
      server->user (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (Test_Interceptors::Silly, userex)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught Silly\n"));
    }
  ACE_ENDTRY;
  ACE_CHECK;

  ACE_TRY_EX (SYS)
    {
      server->system (ACE_TRY_ENV);
      ACE_TRY_CHECK_EX (SYS);
    }
  ACE_CATCH (CORBA::INV_OBJREF, sysex)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught Inv_Objref\n"));
    }
  ACE_ENDTRY;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Installing the Echo interceptor
      ACE_NEW_RETURN (interceptor_impl,
                      Echo_Client_Request_Interceptor (orb),
                      -1);
      interceptor = interceptor_impl->_this ();
      orb->_register_client_interceptor (interceptor);

      CORBA::Object_var object =
        orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_Interceptors::Visual_var server =
        Test_Interceptors::Visual::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      run_test (server.in (), ACE_TRY_ENV);

      server->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Catched exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
