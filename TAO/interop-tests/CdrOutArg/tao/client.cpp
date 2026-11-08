#include "ace/Get_Opt.h"
#include "testC.h"
#include "Client_ORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"

ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (ior);

      Interop::CDR_Out_Arg_var const server =
        Interop::CDR_Out_Arg::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      CORBA::Long lv {};
      CORBA::Long_out arg (lv);

      server->get_out (arg);

      ACE_DEBUG ((LM_DEBUG, "first call passed\n"));

      server->get_out (arg);

      ACE_DEBUG ((LM_DEBUG, "second call passed\n"));

      server->shutdown ();

      orb->destroy ();
    }
    catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception in client:");
      return 1;
    }

  return 0;
}
