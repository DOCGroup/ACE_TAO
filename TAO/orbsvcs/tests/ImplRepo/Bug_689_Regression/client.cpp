// $Id$

#include "bug_689_regressionC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Argv_Type_Converter.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://server.ior");

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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_Argv_Type_Converter argcon (argc, argv);
      CORBA::ORB_var orb = CORBA::ORB_init (argcon.get_argc (),
                                            argcon.get_TCHAR_argv ());

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object (ior);

      bug_689_regression_var servant = bug_689_regression::_narrow(tmp.in ());

      if (CORBA::is_nil (servant.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil bug_689_regression reference <%s>\n",
                             ior),
                            1);
        }

      servant->shutdown ();

      ACE_Time_Value tv (0, 500);
      ACE_OS::sleep (tv);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
