// $Id$

#include "testS.h"
#include "ace/Get_Opt.h"

#include "ace/Service_Config.h"
#include "tao/Strategies/TAO_Strategies_Internal.h"

#if (TAO_HAS_RT_CORBA == 1)

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  Test_i (CORBA::ORB_ptr orb);
  // ctor

  // = The Test methods.
  void test_method (CORBA::Short priority,
                    CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method (CORBA::Short priority,
                     CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Use RTCurrent to find out the CORBA priority of the current
  // thread.

  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RTCurrent", ACE_TRY_ENV);
  ACE_CHECK;

  RTCORBA::Current_var current =
    RTCORBA::Current::_narrow (obj.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (CORBA::is_nil (obj.in ()))
    ACE_THROW (CORBA::INTERNAL ());

  CORBA::Short servant_thread_priority =
    current->the_priority (ACE_TRY_ENV);
  ACE_CHECK;

  // Print out the info.
  if (servant_thread_priority != priority)
    ACE_DEBUG ((LM_DEBUG,
                "ERROR: servant thread priority is not equal"
                "to method argument.\n"));

  ACE_DEBUG ((LM_DEBUG,
              "Client priority: %d  "
              "Servant thread priority: %d\n",
              priority, servant_thread_priority));
}

void
Test_i::shutdown (CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
}

//*************************************************************************

const char *ior_output_file = "test.ior";

// Parse command-line arguments.
int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Standard initialization:
      // parse arguments and get all the references (ORB,
      // RootPOA, RTORB, RTCurrent, POAManager).
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      object = orb->resolve_initial_references ("RTORB", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in (),
                                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("RTCurrent", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create POA with CLIENT_PROPAGATED PriorityModelPolicy,
      // and register Test object with it.
      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (1);
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              0,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("Child_POA",
                              poa_manager.in (),
                              poa_policy_list,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_i server_impl (orb.in ());

      PortableServer::ObjectId_var id =
        child_poa->activate_object (&server_impl, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var server =
        child_poa->id_to_reference (id.in (),
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Print Object IOR.
      CORBA::String_var ior =
        orb->object_to_string (server.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n\n", ior.in ()));

      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      // Get the initial priority of the current thread.
      CORBA::Short initial_thread_priority =
        current->the_priority (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Run ORB Event loop.
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n"));

      // Get the final priority of the current thread.
      CORBA::Short final_thread_priority =
        current->the_priority (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (final_thread_priority != initial_thread_priority)
        ACE_DEBUG ((LM_DEBUG,
                    "ERROR: Priority of the servant thread"
                    "has been permanently changed!\n"
                    "Initial priority: %d  Final priority: %d\n",
                    initial_thread_priority, final_thread_priority));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "Final priority of the servant thread"
                    " = its initial priority\n"));

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

#else /* TAO_HAS_RT_CORBA == 1 */

int
main (int argc, char *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR_RETURN ((LM_ERROR,
                     "\nRTCORBA must be enabled to run this test!\n"),
                    1);
}

#endif /* TAO_HAS_RT_CORBA == 1 */
