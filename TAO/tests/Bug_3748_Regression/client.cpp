// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Hello, client, "$Id$")

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");
const ACE_TCHAR *shutdown_ior = ACE_TEXT ("file://shutdown_test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:s:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 's':
        shutdown_ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-s <shutdown_ior>"
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

      CORBA::Object_var tmp = orb->string_to_object(ior);

      Test::Hello_var hello = Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      int encountered_failures = 0;
      for (int i = 0; i < 5000; ++i)
        {
          try
            {
              CORBA::String_var the_string = hello->get_string ();
            }
          catch (::CORBA::Exception const &)
            {
              if (++encountered_failures % 500 == 0)
                {
#if 0
                  ACE_DEBUG ((LM_DEBUG,
                              "Expected exception\n"));
#endif
                }
            }
        }

      if (encountered_failures == 0)
        ACE_ERROR ((LM_ERROR,
                    "ERROR: No expected exceptions occured.\n"));

      tmp = orb->string_to_object(shutdown_ior);

      Test::Hello_var shutdown_hello = Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (shutdown_hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             shutdown_ior),
                            1);
        }

      shutdown_hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
