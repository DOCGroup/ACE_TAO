#include "HelloC.h"

#include "tao/Messaging/Messaging.h"

#include "tao/Transport_Cache_Manager.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/Transport.h"

#include "ace/Get_Opt.h"

ACE_TCHAR const *ior = ACE_TEXT("file://test.ior");

enum class Policy_Level
{
  object,
  thread,
  orb
};

enum class Synchronization
{
  none,
  delayed
};

Policy_Level policy_level = Policy_Level::object;
Synchronization synchronization = Synchronization::none;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:l:s:"));
  int c {};
  policy_level = Policy_Level::object;
  synchronization = Synchronization::none;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'l':
        {
          ACE_TCHAR const *level = get_opts.opt_arg ();
          if (ACE_OS::strcmp (level, ACE_TEXT("orb")) == 0)
            policy_level = Policy_Level::orb;
          else if (ACE_OS::strcmp (level, ACE_TEXT("thread")) == 0)
            policy_level = Policy_Level::thread;
          else
            policy_level = Policy_Level::object;
          break;
        }
      case 's':
        {
          ACE_TCHAR const *synch = get_opts.opt_arg ();
          if (ACE_OS::strcmp (synch, ACE_TEXT("delayed")) == 0)
            synchronization = Synchronization::delayed;
          else
            synchronization = Synchronization::none;
          break;
        }

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
         "-l <obj|orb|thread> "
         "-s <none|delayed> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "test");

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object (ior);

      if (CORBA::is_nil (tmp.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Invalid IOR.\n")
                            ,1);
        }

      {
        // Set the Synch Scopes

        CORBA::Any scope_as_any;

        if (synchronization == Synchronization::none)
          {
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Applying SYNC_NONE.\n"));
            scope_as_any <<= Messaging::SYNC_NONE;
          }
        else if (synchronization == Synchronization::delayed)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) - Applying SYNC_DELAYED_BUFFERING.\n"));
            scope_as_any <<= TAO::SYNC_DELAYED_BUFFERING;
          }

        CORBA::PolicyList policies (1);
        policies.length (1);
        policies[0] =
          orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                              scope_as_any);

        if (policy_level == Policy_Level::thread)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) - Applying Synch Scope at thread level.\n"));

            CORBA::Object_var object =
              orb->resolve_initial_references ("PolicyCurrent");
            CORBA::PolicyCurrent_var policy_current =
              CORBA::PolicyCurrent::_narrow (object.in ());

            policy_current->set_policy_overrides (policies,
                                                  CORBA::ADD_OVERRIDE);
          }
        else if (policy_level == Policy_Level::orb)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) - Applying Synch Scope at orb level.\n"));

            CORBA::Object_var obj =
              orb->resolve_initial_references ("ORBPolicyManager");
            CORBA::PolicyManager_var policy_manager =
              CORBA::PolicyManager::_narrow (obj.in ());

            policy_manager->set_policy_overrides (policies,
                                                  CORBA::ADD_OVERRIDE);
          }
        else if (policy_level == Policy_Level::object)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) - Applying Synch Scope at Object level.\n"));

            tmp = tmp->_set_policy_overrides (policies, CORBA::SET_OVERRIDE);
          }

        policies[0]->destroy ();
      }

      Test::Hello_var hello =
        Test::Hello::_unchecked_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      // If a blocking connection is initiated in the next step
      // the test will get an exception indicating test failure.
      // Be sure you IOR is for the localhost endpoint, else
      // grab a book and wait around for timeout.
      hello->get_string ();

      TAO::Transport_Cache_Manager &tcm =
        hello->orb_core ()->lane_resources ().transport_cache ();

      TAO_Base_Transport_Property desc (hello->_stubobj ()->profile_in_use ()->endpoint ());

      if (tcm.current_size () == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
           "The Transport Cache shouldn't be empty here.\n"),
          1);
        }

      size_t busy_count {};
      TAO_Transport *transport = nullptr;
      TAO::Transport_Cache_Manager::Find_Result find_result =
        tcm.find_transport (&desc, transport, busy_count);
      // We don't need this transport any more. Release the ownership.
      transport->remove_reference ();

      switch (find_result)
        {
        case TAO::Transport_Cache_Manager::CACHE_FOUND_NONE:
          {
            ACE_ERROR_RETURN ((LM_ERROR,
              ACE_TEXT("Expected to find a transport in the cache.\n")
              ),1);
          }
        case TAO::Transport_Cache_Manager::CACHE_FOUND_CONNECTING:
          {
            ACE_DEBUG (( LM_DEBUG,
              ACE_TEXT ("Transport Cache contains connecting entry as expected.\n")
              ));
            break; // that's what we expected
          }
        case TAO::Transport_Cache_Manager::CACHE_FOUND_BUSY:
          {
            ACE_ERROR_RETURN ((LM_ERROR,
              ACE_TEXT("Cached Transport is busy.  Should not happen because there's no server.\n")
              ),1);
          }
        case TAO::Transport_Cache_Manager::CACHE_FOUND_AVAILABLE:
          {
            ACE_ERROR_RETURN ((LM_ERROR,
              ACE_TEXT("Cached Transport is available.  Should not happen because there's no server.\n")
              ),1);
          }
        default:
          {
            ACE_ERROR_RETURN ((LM_ERROR,
              ACE_TEXT("Transport_Cache::find returned unknown status.\n")
              ),1);
          }
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Exception caught: %s\"%s\"\n"
      , ex._name(), ex._rep_id ()));
      return 1;
    }

  return 0;
}
