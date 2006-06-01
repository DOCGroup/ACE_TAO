//
// $Id$
//
#include "Server_Task.h"
#include "TestS.h"
#include "Hello.h"

#include "ace/Manual_Event.h"

ACE_RCSID(DII_Collocation_Tests,
          Server_Task,
          "$Id$")


Server_Task::Server_Task (const char *output,
                          const char *simple_test_output,
                          CORBA::ORB_ptr sorb,
                          ACE_Manual_Event &me,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , output_ (output)
    , simple_test_output_ (simple_test_output)
    , me_ (me)
    , sorb_ (CORBA::ORB::_duplicate (sorb))
    , error_count_ (0)
{
}

int
Server_Task::svc (void)
{
 ACE_TRY_NEW_ENV
   {
     CORBA::Object_var poa_object =
       this->sorb_->resolve_initial_references("RootPOA"
                                               ACE_ENV_ARG_PARAMETER);
     ACE_TRY_CHECK;

     PortableServer::POA_var root_poa =
       PortableServer::POA::_narrow (poa_object.in ()
                                     ACE_ENV_ARG_PARAMETER);
     ACE_TRY_CHECK;

     if (CORBA::is_nil (root_poa.in ()))
       ACE_ERROR_RETURN ((LM_ERROR,
                          " (%P|%t) Panic: nil RootPOA\n"),
                         1);

     PortableServer::POAManager_var poa_manager =
       root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
     ACE_TRY_CHECK;

     Hello *hello_impl;
     ACE_NEW_RETURN (hello_impl,
                     Hello (this->sorb_.in (),
                            ACE_Thread::self ()),
                     1);

     PortableServer::ServantBase_var owner_transfer(hello_impl);

     Test::Hello_var hello =
       hello_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
     ACE_TRY_CHECK;

     CORBA::String_var ior =
       this->sorb_->object_to_string (hello.in ()
                                      ACE_ENV_ARG_PARAMETER);
     ACE_TRY_CHECK;

     // Output the IOR to the <this->output_>
     FILE *output_file= ACE_OS::fopen (this->output_,
                                       "w");
     if (output_file == 0)
       ACE_ERROR_RETURN ((LM_ERROR,
                          "Cannot open output file for writing Hello IOR: %s",
                          this->output_),
                         1);

     ACE_OS::fprintf (output_file, "%s", ior.in ());
     ACE_OS::fclose (output_file);

     Test_Simple_Test_i *simple_impl;
     ACE_NEW_RETURN (simple_impl,
                     Test_Simple_Test_i (),
                     1);

     PortableServer::ServantBase_var owner_transfer_simple(simple_impl);

     Test::Simple_Test_var simple_test =
       simple_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
     ACE_TRY_CHECK;

     CORBA::String_var simple_test_ior =
       this->sorb_->object_to_string (simple_test.in ()
                                      ACE_ENV_ARG_PARAMETER);
     ACE_TRY_CHECK;

     // Output the IOR to the <this->output_>
     FILE *simple_test_output_file= ACE_OS::fopen (this->simple_test_output_,
                                       "w");
     if (simple_test_output_file == 0)
       ACE_ERROR_RETURN ((LM_ERROR,
                          "Cannot open output file for writing Simple_Test IOR: %s",
                          this->simple_test_output_),
                         1);

     ACE_OS::fprintf (simple_test_output_file, "%s", simple_test_ior.in ());
     ACE_OS::fclose (simple_test_output_file);

     poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
     ACE_TRY_CHECK;

     // Signal the main thread before we call orb->run ();
     this->me_.signal ();

     this->sorb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
     ACE_TRY_CHECK;

     ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

     ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - verifing results\n"));

     CORBA::ULong errors = hello_impl->error_count ();

     error_count_ += errors;
   }
 ACE_CATCHANY
   {
     error_count_ ++;
     ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                          "Exception caught:");
     return 1;
   }
 ACE_CATCHALL
   {
     error_count_ ++;
     ACE_ERROR ((LM_ERROR, "(%P|%t)Server_Task::svc - caught unknown exception \n"));
     return 1;
   }
 ACE_ENDTRY;

 return 0;
}

CORBA::ULong 
Server_Task::error_count () const
{
  return error_count_;
}

