#include "TestC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://shutdown.ior")};
  bool shutdown_nsmain {};
  int test_count {10};
};

int
parse_args (Client_Options &options, int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        options.shutdown_nsmain = true;
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s \n"),
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
      Client_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (options, argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object ("corbaloc:iiop:1.2@localhost:9931/NameService");

      CosNaming::NamingContext_var const root =
        CosNaming::NamingContext::_narrow (tmp.in ());

      if (CORBA::is_nil (root.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil NamingService reference\n")),
                            1);
        }

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("**** Narrowed root NamingContext\n")));

      CosNaming::Name name;
      name.length (4);
      name[0].id = CORBA::string_dup ("This");
      name[0].kind = CORBA::string_dup ("");
      name[1].id = CORBA::string_dup ("eat");
      name[1].kind = CORBA::string_dup ("");
      name[2].id = CORBA::string_dup ("my");
      name[2].kind = CORBA::string_dup ("");
      name[3].id = CORBA::string_dup ("memory");
      name[3].kind = CORBA::string_dup ("");

      for (int i {}; i < options.test_count; ++i)
        {
          try
            {
              tmp = root->resolve (name);
              ACE_DEBUG ((LM_INFO, ACE_TEXT ("**** Resolved #example/Hello\n")));

              Test::Hello_var const hello =
                Test::Hello::_narrow (tmp.in ());

              if (CORBA::is_nil (hello.in ()))
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("Nil Test::Hello reference\n")),
                                  1);
            }
          catch (CosNaming::NamingContext::NotFound const &)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Caught correct exception for iteration %d\n"),
                          i));
            }
        }

      if (options.shutdown_nsmain)
        {
          CORBA::Object_var const shutdown_tmp = orb->string_to_object (options.ior);

          Test::NsShutdown_var const shutdown =
            Test::NsShutdown::_narrow (shutdown_tmp.in ());

          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Shutdown nsmain\n")));
          shutdown->shutdown ();
        }

      orb->shutdown ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
