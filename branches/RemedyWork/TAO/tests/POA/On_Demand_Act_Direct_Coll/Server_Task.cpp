//
// $Id$
//
#include "Server_Task.h"
#include "Servant_Activator.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Manual_Event.h"

static const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

static int
write_iors_to_file (const char *first_ior)
{
  FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
  if (output_file == 0 )
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IORs: %Cn",
                       ior_output_file),
                      -1);

  int result = ACE_OS::fprintf (output_file,
                                "%s",
                                first_ior);
  if (result <= 0
      || static_cast<size_t> (result) != ACE_OS::strlen (first_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       first_ior,
                       ior_output_file),
                      -1);

  ACE_OS::fclose (output_file);
  return 0;
}

Server_Task::Server_Task (const ACE_TCHAR *output,
                          CORBA::ORB_ptr sorb,
                          ACE_Manual_Event &me,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , output_ (output)
    , me_ (me)
    , sorb_ (CORBA::ORB::_duplicate (sorb))
{
}

int
Server_Task::svc (void)
{
 try
   {
     CORBA::Object_var poa_object =
       this->sorb_->resolve_initial_references("RootPOA");

     PortableServer::POA_var root_poa =
       PortableServer::POA::_narrow (poa_object.in ());

     if (CORBA::is_nil (root_poa.in ()))
       ACE_ERROR_RETURN ((LM_ERROR,
                          " (%P|%t) Panic: nil RootPOA\n"),
                         1);

     PortableServer::POAManager_var poa_manager =
       root_poa->the_POAManager ();


     CORBA::PolicyList policies (4);
     policies.length (4);

     // ID Assignment Policy
     policies[0] =
       root_poa->create_id_assignment_policy (PortableServer::USER_ID);

     // Lifespan Policy
     policies[1] =
       root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

     // Request Processing Policy
     policies[2] =
       root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

     PortableServer::POA_var first_poa;
     {
       // Servant Retention Policy
       policies[3] =
         root_poa->create_servant_retention_policy (PortableServer::RETAIN);

       ACE_CString name = "firstPOA";

       // Create firstPOA as the child of RootPOA with the above policies
       // firstPOA will use SERVANT_ACTIVATOR because of RETAIN policy.
       first_poa = root_poa->create_POA (name.c_str (),
                                         poa_manager.in (),
                                         policies);

     }

     // Destroy the policy objects as they have been passed to
     // create_POA and no longer needed.
     for (CORBA::ULong i = 0;
          i < policies.length ();
          ++i)
       {
         CORBA::Policy_ptr policy = policies[i];
         policy->destroy ();
       }
     // Allocate the servant activator.
     ServantActivator activator (this->sorb_.in (),  ACE_Thread::self ());
     // Set ServantActivator object as the servant_manager of
     // firstPOA.

     first_poa->set_servant_manager (&activator);
     // Create a reference with user created ID in firstPOA which uses
     // the ServantActivator.

     PortableServer::ObjectId_var first_test_oid =
         PortableServer::string_to_ObjectId ("first test");

     CORBA::Object_var first_test =
         first_poa->create_reference_with_id (first_test_oid.in (), "IDL:test:1.0");
        // Invoke object_to_string on the references created in firstPOA

     CORBA::String_var first_test_ior =
         this->sorb_->object_to_string (first_test.in ());

     // Print the ior's of first_test.
     ACE_DEBUG((LM_DEBUG,"<%C>\n",
                  first_test_ior.in ()));

     int write_result = write_iors_to_file (first_test_ior.in ());
     if (write_result != 0)
        return write_result;

     // Set the poa_manager state to active, ready to process requests.
     poa_manager->activate ();
     this->me_.signal ();
     // Run the ORB.
     this->sorb_->run ();

     ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
   }
 catch (const CORBA::Exception& ex)
   {
     ex._tao_print_exception ("Exception caught:");
     return 1;
   }

 return 0;
}
