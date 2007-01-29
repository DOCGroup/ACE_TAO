// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Hello, client, "$Id$")

const char *ior = "file://test.ior";
int bad_ior = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:b");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'b':
        bad_ior = 1;
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
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb;
  CORBA::Object_var tmp;

  try
    {
      orb = CORBA::ORB_init (argc, argv, "");

      if (parse_args (argc, argv) != 0)
        return 1;

      tmp = orb->string_to_object(ior);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  try
    {
      ACE_DEBUG ((LM_DEBUG,"client: Invocation expecting to %s\n",
                  (bad_ior ? "fail" : "work")));
      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      CORBA::String_var the_string =
        hello->get_string ();

      if (bad_ior)
        {
          ACE_DEBUG ((LM_DEBUG,"client: Error: was expecting an exception.\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "client: success!\n"));

      hello->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      if (!bad_ior)
        {
          ex._tao_print_exception ("client: Exception caught:");
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, "client: success!\n"));
    }

  orb->destroy ();

  return 0;
}
