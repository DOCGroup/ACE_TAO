// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

bool expect_error = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'x':
        expect_error = true;
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

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

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      if (expect_error)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - expected exception occurred!\n%s\n",
                      ex._info().c_str()));

          return 0;
        }
      else
        {
          ex._tao_print_exception ("Exception caught:");
          return 1;
        }
    }

  if (expect_error)
    {
      ACE_DEBUG ((LM_ERROR, "(%P|%t) client - expected error did not occur!\n"));

      return 1;
    }

  return 0;
}
