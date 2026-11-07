#include "ace/Get_Opt.h"

#include "testC.h"
#include "Echo_Client_ORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"

ACE_TCHAR const *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("ef:"));
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
run_test (Test_Interceptors::Visual_ptr server)
{
  CORBA::String_var msg;

  server->normal (10, msg.out ());

  CORBA::Long const one {1};
  CORBA::Long const two {1};
  CORBA::Long const result {server->calculate (one, two)};

  ACE_DEBUG ((LM_DEBUG,
              "calculate() result = %d\n",
              result));

  Test_Interceptors::Visual::VarLenStruct_var v =
    server->the_structure ();

  ACE_DEBUG ((LM_DEBUG,
              "the_structure() result is:\n"
              "  flag    = %d\n"
              "  message = %C\n",
              v->flag,
              v->message.in ()));

  try
    {
      server->user ();
    }
  catch (Test_Interceptors::Silly const&)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught Silly\n"));
    }

  try
    {
      server->system ();
    }
  catch (CORBA::INV_OBJREF const&)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught CORBA::INV_OBJREF\n"));
    }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result {};

  try
    {
#if TAO_HAS_INTERCEPTORS == 1
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      Echo_Client_ORBInitializer (result),
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
#endif /* TAO_HAS_INTERCEPTORS == 1 */

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Test_Interceptors::Visual_var server =
        Test_Interceptors::Visual::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      run_test (server.in ());

      server->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return result;
}
