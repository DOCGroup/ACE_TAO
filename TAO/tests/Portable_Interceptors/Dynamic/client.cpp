// $Id$

#include "ace/Get_Opt.h"

#include "testC.h"
#include "Echo_Client_ORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"

ACE_RCSID (Dynamic,
           client,
           "$Id$")

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
        ior = get_opts.opt_arg ();
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
run_test (Test_Interceptors::Visual_ptr server
          ACE_ENV_ARG_DECL)
{
  CORBA::String_var msg;

  server->normal (10,
                  msg.out ()
                  ACE_ENV_ARG_PARAMETER);

  CORBA::Long one = 1, two = 1, result = 0;
  result = server->calculate (one,
                              two
                              ACE_ENV_ARG_PARAMETER);

  ACE_DEBUG ((LM_DEBUG,
              "calculate() result = %d\n",
              result));

  Test_Interceptors::Visual::VarLenStruct_var v =
    server->the_structure ();

  ACE_DEBUG ((LM_DEBUG,
              "the_structure() result is:\n"
              "  flag    = %d\n"
              "  message = %s\n",
              v->flag,
              v->message.in ()));

  ACE_TRY
    {
      server->user ();
    }
  ACE_CATCH (Test_Interceptors::Silly, userex)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught Silly\n"));
    }
  ACE_ENDTRY;

  ACE_TRY_EX (SYS)
    {
      server->system ();
      ACE_TRY_CHECK_EX (SYS);
    }
  ACE_CATCH (CORBA::INV_OBJREF, sysex)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught CORBA::INV_OBJREF\n"));
    }
  ACE_ENDTRY;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
#if TAO_HAS_INTERCEPTORS == 1
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      Echo_Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
#endif /* TAO_HAS_INTERCEPTORS == 1 */

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);

      Test_Interceptors::Visual_var server =
        Test_Interceptors::Visual::_narrow (object.in () ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      run_test (server.in () ACE_ENV_ARG_PARAMETER);

      server->shutdown ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
