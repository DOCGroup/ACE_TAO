#include "ace/Log_Msg.h"
#include "tao/corba.h"
#include "tao/TimeBaseC.h"
#include "tao/Messaging/Messaging.h"
#include "orbsvcs/CosNamingC.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int retval {};
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      constexpr TimeBase::TimeT timeout {5 * 10000000};

      CORBA::Object_var obj {orb->resolve_initial_references ("ORBPolicyManager")};
      CORBA::PolicyManager_var const policy_manager =
        CORBA::PolicyManager::_narrow (obj.in ());

      CORBA::PolicyList policy_list {};
      policy_manager->set_policy_overrides (policy_list, CORBA::SET_OVERRIDE);

      CORBA::Any orb_timeout {};
      orb_timeout <<= timeout;
      policy_list.length (1);
      policy_list[0] = orb->create_policy (
        Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE, orb_timeout);

      policy_manager->set_policy_overrides (policy_list, CORBA::SET_OVERRIDE);

      // Now try to contact a server.
      // Use a remote machine that is unreachable on the network.
      constexpr char obj_ref[] {"corbaloc:iiop:1.0@10.123.123.123:4567/NameService"};

      obj = orb->string_to_object (obj_ref);

      CosNaming::NamingContext_var inc {};
      if (!CORBA::is_nil (obj.in ()))
        {
          ACE_DEBUG ((LM_INFO, "Attempting to contact %C\n", obj_ref));
          try
            {
              inc = CosNaming::NamingContext::_narrow (obj.in ());
              ACE_DEBUG ((LM_INFO, "OK\n"));
            }
          catch (CORBA::TRANSIENT const &)
            {
              ACE_ERROR ((LM_ERROR, "Error, caught transient exception\n"));
              retval = 1;
            }
          catch (CORBA::TIMEOUT const &)
            {
              ACE_DEBUG ((LM_DEBUG, "Caught correct timeout\n"));
            }
        }
    }
  catch (::CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("ERROR : unexpected CORBA exception caught :");
      ++retval;
    }

  return retval;
}

