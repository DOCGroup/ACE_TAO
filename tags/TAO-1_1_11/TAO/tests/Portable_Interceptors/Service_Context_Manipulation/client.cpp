// $Id$

#include "ace/Get_Opt.h"
#include "testC.h"
#include "interceptors.h"
#include "Client_ORBInitializer.h"

ACE_RCSID(Interceptors, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "ef:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'e':
      break;
      case 'f':
        ior = get_opts.optarg;
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

  cout << "normal done"<<endl;
  server->nothing (ACE_TRY_ENV);

  cout<< "nothing done"<<endl;
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
      PortableInterceptor::ORBInitializer_ptr temp_initializer;

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in (),
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Transfer ownership to the ORB.
      (void) initializer._retn ();

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

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
