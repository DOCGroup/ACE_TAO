// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/Messaging/Messaging.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");
const ACE_TCHAR *loop_forever_ior = ACE_TEXT ("file://forward_forever.ior");
bool shutdown_only = false;


int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:l:s"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'l':
        loop_forever_ior = get_opts.opt_arg ();
        break;
      case 's':
        shutdown_only = true;
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - ORB_init()\n"));
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp;
      Test::Hello_var hello;

      if (!shutdown_only)
        {
          tmp = orb->string_to_object (loop_forever_ior);

          if (CORBA::is_nil (tmp.in ()))
            {
              ACE_ERROR_RETURN ((LM_DEBUG,
                                "ERROR: Nil object from string_to_object of <%s>\n",
                                loop_forever_ior),
                                1);
            }

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - create timeout policy\n"));
          // Three second relative roundtrip timeout.
          TimeBase::TimeT timeout_period = 30000000u;

          CORBA::Any timeout_as_any;
          timeout_as_any <<= timeout_period;

          CORBA::PolicyList policy_list (1);
          policy_list.length (1);
          policy_list[0] =
            orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                      timeout_as_any);
          tmp = tmp->_set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE);

          for (CORBA::ULong i = 0;
              i < policy_list.length ();
              ++i)
            {
              policy_list[i]->destroy ();
            }

          try
            {
              ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - loop_forever narrow\n"));

              // This should LOCATION_FORWARD loop until the timeout
              hello = Test::Hello::_narrow(tmp.in ());

              if (CORBA::is_nil (hello.in ()))
                {
                  ACE_ERROR_RETURN ((LM_DEBUG,
                                "ERROR: Nil hello from <%s>\n",
                                loop_forever_ior),
                                1);
                }

              ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - loop_forever call\n"));
              CORBA::String_var the_string = hello->get_string ();

              ACE_ERROR_RETURN ((LM_ERROR,
                                  "ERROR - We have managed to invoke a server method. \n"),
                                1);
            }
          catch (const CORBA::TIMEOUT &)
            {
              // Hooray - all OK
              ACE_DEBUG ((LM_DEBUG, "Test OK - RELATIVE_RT_TIMEOUT ends location forward loop.\n"));
            }
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - shutdown server\n"));
      tmp = orb->string_to_object (ior);

      hello = Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                        "ERROR: Nil hello from <%s>\n",
                        ior),
                        1);
        }

      hello->shutdown ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - End\n"));
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ERROR: Exception caught:");
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - Test SUCCESSFULL!\n"));
  return 0;
}
