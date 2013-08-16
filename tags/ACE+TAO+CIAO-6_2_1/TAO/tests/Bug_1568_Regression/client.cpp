/**
 * @file client.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "TestC.h"

#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Baz::C_var cobject =
        Baz::C::_narrow (object.in ());

      if (CORBA::is_nil (cobject.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                                "Cannot narrow Baz::C object <%s>\n",
                                ior),
                            1);
        }

      cobject->op1 ();
      cobject->op2 ();
      cobject->op3 ();
      cobject->op4 ();
      cobject->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("");
      return 1;
    }

  return 0;
}
