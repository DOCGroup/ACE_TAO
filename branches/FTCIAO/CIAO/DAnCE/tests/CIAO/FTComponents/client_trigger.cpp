// $Id$

#include "TriggerC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *first_ior = ACE_TEXT ("file://first.ior");
const ACE_TCHAR *prefix = ACE_TEXT ("");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        first_ior = get_opts.opt_arg ();
        break;
      case 'n':
        prefix = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "-k <ior> "
                           "-n <prefix>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
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

      CORBA::Object_var tmp = orb->string_to_object(first_ior);

      Trigger_var trigger = Trigger::_narrow(tmp.in ());

      if (CORBA::is_nil (trigger.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Trigger reference <%s>\n",
                             first_ior),
                            1);
        }

      trigger->start (CORBA::string_dup (prefix));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
