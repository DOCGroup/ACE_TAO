// -*- C++ -*-
// $Id$

#include "testC.h"
#include "Client_ORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"

const ACE_TCHAR *ior = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
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
  int status = 0;

  try
    {
#if TAO_HAS_INTERCEPTORS == 1
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

      ACE_Argv_Type_Converter narrow (argc, argv);
      CORBA::ORB_var orb =
        CORBA::ORB_init (narrow.get_argc (), narrow.get_ASCII_argv (), "ClientORB");

      ACE_Argv_Type_Converter wide (narrow.get_argc (), narrow.get_ASCII_argv ());
      if (::parse_args (wide.get_argc (), wide.get_TCHAR_argv ()) != 0)
        return -1;

      CORBA::Object_var object =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior));

      hello::GoodDay_var server =
        hello::GoodDay::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil\n"),
                             ior),
                            1);
        }

      for (int i = 1; i <= 3; ++i)
        {
          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("CLIENT: Issuing request %d.\n"),
                      i));

          CORBA::Short number = server->number ();

          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("CLIENT: Request %d returns %d.\n"),
                      i,
                      number));
        }

      server->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return status;
}
