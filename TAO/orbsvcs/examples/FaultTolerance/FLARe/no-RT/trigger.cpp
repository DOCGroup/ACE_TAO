// $Id$

#include "TriggerC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Trigger, client, "$Id$")

const ACE_TCHAR *first_ior = ACE_TEXT ("file://first.ior");
bool shut_down = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:s"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        first_ior = get_opts.opt_arg ();
        break;
      case 's':
        shut_down = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "-k <ior> "
			   "-s"
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

      Test::Trigger_var trigger = Test::Trigger::_narrow(tmp.in ());

      if (CORBA::is_nil (trigger.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Trigger reference <%s>\n",
                             first_ior),
                            1);
        }

      if (!shut_down)
	trigger->start ();
      else
	trigger->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
