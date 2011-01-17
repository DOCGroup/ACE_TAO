// $Id$

#include "TestC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"



const ACE_TCHAR *ior = ACE_TEXT("file://shutdown.ior");
bool shutdown_nsmain = false;
int test_count = 10;

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
        shutdown_nsmain = true;
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object("corbaloc:iiop:1.2@localhost:9931/NameService");

      CosNaming::NamingContext_var root =
        CosNaming::NamingContext::_narrow(tmp.in());

      if (CORBA::is_nil (root.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil NamingService reference\n"),
                            1);
        }

      ACE_DEBUG ((LM_INFO, "**** Narrowed root NamingContext\n"));

      CosNaming::Name name;
      name.length(4);
      name[0].id   = CORBA::string_dup("This");
      name[0].kind = CORBA::string_dup("");
      name[1].id   = CORBA::string_dup("eat");
      name[1].kind = CORBA::string_dup("");
      name[2].id   = CORBA::string_dup("my");
      name[2].kind = CORBA::string_dup("");
      name[3].id   = CORBA::string_dup("memory");
      name[3].kind = CORBA::string_dup("");

      for (int i = 0; i < test_count; i++)
        {
          try
            {
              tmp = root->resolve (name);
              ACE_DEBUG ((LM_INFO, "**** Resolved #example/Hello\n"));

              Test::Hello_var hello =
                Test::Hello::_narrow(tmp.in ());

              ACE_ERROR_RETURN ((LM_DEBUG,
                                 "Nil Test::Hello reference\n"),
                                 1);
            }
          catch (const CosNaming::NamingContext::NotFound&)
            {
              ACE_DEBUG ((LM_DEBUG, "Caught correct exception for iteration %d\n", i));
            }
        }

      if (shutdown_nsmain)
        {
          CORBA::Object_var shutdowntmp = orb->string_to_object(ior);

          Test::NsShutdown_var shutdown =
            Test::NsShutdown::_narrow(shutdowntmp.in ());

          ACE_DEBUG ((LM_DEBUG, "Shutdown nsmain\n"));
          shutdown->shutdown ();
        }

      orb->shutdown ();
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
