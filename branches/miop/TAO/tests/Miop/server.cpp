// $Id$

#include "testS.h"
#include "tao/PortableGroup/miopC.h"
#include "tao/PortableGroup/PortableGroupC.h"
#include "ace/Get_Opt.h"

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  Test_i (CORBA::ORB_ptr orb, int i);
  // ctor

  // = The Test methods.
  void test_method (CORBA::Short priority,
                    CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void test_long_sequence (const Test::long_seq & input,
                           CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  // The ORB.
  CORBA::ORB_var orb_;

  // The instance number.
  int instance_;
};

Test_i::Test_i (CORBA::ORB_ptr orb, int i)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     instance_ (i)
{
}

void
Test_i::test_method (CORBA::Short priority,
                     CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Hello from instance <%d>.  Got %d\n", 
              this->instance_,
              priority));
}

void 
Test_i::test_long_sequence (const Test::long_seq & input,
                            CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Hello from instance <%d>.  Got long sequence\n", 
              this->instance_));
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

      CORBA::Object_var MIOP =
        orb->resolve_initial_references (TAO_OBJID_MIOP,
                                          0,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      MIOP::MulticastObjectGroupFactory_var mogf =
               MIOP::MulticastObjectGroupFactory::_narrow (MIOP.in (),
                                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create an instance of our servant.
      Test_i server_impl (orb.in (), 0);

#if 1
      // Create a multicast group IOR
      CORBA::Octet class_d_address[4]={225,1,1,1};

      CORBA::Object_var group1 = mogf->create_group (server_impl._interface_repository_id (),
                                                     class_d_address,
                                                     1234, 
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Print Object IOR.
      CORBA::String_var ior =
        orb->object_to_string (group1.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
#else
      CORBA::String_var ior = CORBA::string_dup ("corbaloc:miop:1.0@1.0-MyLittleDomain-1/225.1.1.8:5000");
      CORBA::Object_var group1 = orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;
#endif

      ACE_DEBUG ((LM_DEBUG, "group1's IOR is <%s>\n\n", ior.in ()));

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

      CORBA::Object_var object =
        orb->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id =
        root_poa->create_id_for_reference (group1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      root_poa->activate_object_with_id (id, &server_impl, ACE_TRY_ENV);
      ACE_TRY_CHECK;

#if 0
      // Create a second servant for the same group
      Test_i server_impl2 (orb.in (), 1);

      PortableServer::ObjectId_var id2 =
        root_poa->create_id_for_reference (group1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      root_poa->activate_object_with_id (id2, &server_impl2, ACE_TRY_ENV);
      ACE_TRY_CHECK;
#endif

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Run ORB Event loop.
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n"));
      

#if 0
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
#endif
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

