// $Id$

#include <string>
#include <vector>
#include "TriggerC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *prefix = ACE_TEXT ("");
std::vector <std::string> applications;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  if (argc > 1)
    prefix = argv[1];

  if (argc > 2)
    {
      for (int i = 2; i < argc; ++i)
        {
          applications.push_back (argv[i]);
        }
    }
  else
    return -1;

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

      std::vector <Trigger_var> triggers;

      for (std::vector<std::string>::iterator it = applications.begin ();
           it != applications.end ();
           ++it)
        {
          CORBA::Object_var tmp = orb->string_to_object(it->c_str ());

          Trigger_var trigger = Trigger::_narrow(tmp.in ());

          if (CORBA::is_nil (trigger.in ()))
            {
              ACE_ERROR_RETURN ((LM_DEBUG,
                                 "Nil Trigger reference <%s>\n",
                                 it->c_str ()),
                                1);
            }
          
          triggers.push_back (trigger);
        }

      for (std::vector<Trigger_var>::iterator it2 = triggers.begin ();
           it2 != triggers.end ();
           ++it2)
        {
          (*it2)->start (CORBA::string_dup (prefix));
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
