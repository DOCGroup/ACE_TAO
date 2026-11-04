// -*- C++ -*-
#include "testC.h"
#include "Client_ORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"

#include <string>

int
parse_args (int argc, ACE_TCHAR *argv[], std::basic_string<ACE_TCHAR> &ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage:  %s ")
                           ACE_TEXT ("-k IOR\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
#if TAO_HAS_INTERCEPTORS == 1
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
      // The var takes ownership of the ORB initializer's initial reference.
      PortableInterceptor::ORBInitializer_var const orb_initializer (temp_initializer);

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

      ACE_Argv_Type_Converter narrow (argc, argv);
      CORBA::ORB_var const orb {
        CORBA::ORB_init (narrow.get_argc (), narrow.get_ASCII_argv (), "ClientORB")};

      ACE_Argv_Type_Converter wide (narrow.get_argc (), narrow.get_ASCII_argv ());
      std::basic_string<ACE_TCHAR> ior {};
      if (::parse_args (wide.get_argc (), wide.get_TCHAR_argv (), ior) != 0)
        return -1;

      CORBA::Object_var const object =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior.c_str ()));

      hello::GoodDay_var const server =
        hello::GoodDay::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil\n"),
                             ior.c_str ()),
                            1);
        }

      constexpr int request_count = 3;
      for (int i {1}; i <= request_count; ++i)
        {
          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("CLIENT: Issuing request %d.\n"),
                      i));

          CORBA::Short const number = server->number ();

          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("CLIENT: Request %d returns %d.\n"),
                      i,
                      number));
        }

      server->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
