// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ciao/Base/Client_init.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

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

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
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

      ::CIAO::Client_init (orb.in ());

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object(ior);

      Test::Cook_var cook = Test::Cook::_narrow(tmp.in ());

      if (CORBA::is_nil (cook.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Cook reference <%s>\n",
                             ior),
                            1);
        }

      Components::Cookie_var cookie = cook->get_cookie ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - cookie returned <%@>\n",
                  cookie.in ()));

      try
        {
          cook->check_cookie (cookie.in ());
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Client exception caught:");
          return 1;
        }

      cook->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client exception caught:");
      return 1;
    }

  return 0;
}
