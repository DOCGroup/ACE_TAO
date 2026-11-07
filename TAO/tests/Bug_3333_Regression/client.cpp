#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/Messaging/Messaging.h"
#include "tao/Utils/PolicyList_Destroyer.h"


int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior,
            ACE_TCHAR const *&loop_forever_ior, bool &shutdown_only)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:l:s"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
  ACE_TCHAR const *loop_forever_ior = ACE_TEXT ("file://forward_forever.ior");
  bool shutdown_only {};
  try
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - ORB_init()\n"));
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior, loop_forever_ior, shutdown_only) != 0)
        return 1;

      if (!shutdown_only)
        {
          CORBA::Object_var const loop_object = orb->string_to_object (loop_forever_ior);

          if (CORBA::is_nil (loop_object.in ()))
            {
              ACE_ERROR_RETURN ((LM_DEBUG,
                                "ERROR: Nil object from string_to_object of <%s>\n",
                                loop_forever_ior),
                                1);
            }

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - create timeout policy\n"));
          // Three second relative roundtrip timeout.
          TimeBase::TimeT const timeout_period = 30000000u;

          CORBA::Any timeout_as_any;
          timeout_as_any <<= timeout_period;

          TAO::Utils::PolicyList_Destroyer policy_list (1);
          policy_list.length (1);
          policy_list[0] =
            orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                      timeout_as_any);
          CORBA::Object_var const object_with_policy =
            loop_object->_set_policy_overrides (policy_list, CORBA::SET_OVERRIDE);

          try
            {
              ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - loop_forever narrow\n"));

              // This should LOCATION_FORWARD loop until the timeout
              Test::Hello_var const hello = Test::Hello::_narrow (object_with_policy.in ());

              if (CORBA::is_nil (hello.in ()))
                {
                  ACE_ERROR_RETURN ((LM_DEBUG,
                                "ERROR: Nil hello from <%s>\n",
                                loop_forever_ior),
                                1);
                }

              ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - loop_forever call\n"));
              CORBA::String_var const the_string = hello->get_string ();

              ACE_ERROR_RETURN ((LM_ERROR,
                                  "ERROR - We have managed to invoke a server method. \n"),
                                1);
            }
          catch (CORBA::TIMEOUT const &)
            {
              // Hooray - all OK
              ACE_DEBUG ((LM_DEBUG, "Test OK - RELATIVE_RT_TIMEOUT ends location forward loop.\n"));
            }
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - shutdown server\n"));
      CORBA::Object_var const shutdown_object = orb->string_to_object (ior);

      Test::Hello_var const shutdown_hello = Test::Hello::_narrow (shutdown_object.in ());

      if (CORBA::is_nil (shutdown_hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                        "ERROR: Nil hello from <%s>\n",
                        ior),
                        1);
        }

      shutdown_hello->shutdown ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - End\n"));
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("ERROR: Exception caught:");
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - Test SUCCESSFULL!\n"));
  return 0;
}
