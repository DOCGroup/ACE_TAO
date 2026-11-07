// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "ace/SString.h"

#include "testC.h"
#include "FOO_Client_ORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"

struct Options
{
  ACE_TString ior { ACE_TEXT ("file://test.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage: %s -k IOR\n"),
                           argv[0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  try
    {
#if TAO_HAS_INTERCEPTORS == 1
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};

      ACE_NEW_RETURN (temp_initializer,
                      FOO_Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
#endif /* TAO_HAS_INTERCEPTORS == 1 */

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "client_orb");

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var obj =
        orb->string_to_object (options.ior.c_str ());

      FOO::test_var server =
        FOO::test::_narrow (obj.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior.c_str ()),
                            1);
        }

      server->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("IORInterceptor test (client-side):");
      return -1;
    }

  return 0;
}
