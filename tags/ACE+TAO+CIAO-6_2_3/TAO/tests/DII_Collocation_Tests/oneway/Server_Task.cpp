//
// $Id$
//
#include "Server_Task.h"
#include "TestS.h"
#include "Hello.h"

#include "ace/Manual_Event.h"

Server_Task::Server_Task (const ACE_TCHAR *output,
                          const ACE_TCHAR *simple_test_output,
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

     Hello *hello_impl;
     ACE_NEW_RETURN (hello_impl,
                     Hello (this->sorb_.in (),
                            ACE_Thread::self ()),
                     1);

     PortableServer::ServantBase_var owner_transfer(hello_impl);

     PortableServer::ObjectId_var id =
       root_poa->activate_object (hello_impl);

     CORBA::Object_var object = root_poa->id_to_reference (id.in ());

     Test::Hello_var hello =
       Test::Hello::_narrow (object.in ());

     CORBA::String_var ior =
       this->sorb_->object_to_string (hello.in ());

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

     id = root_poa->activate_object (simple_impl);

     object = root_poa->id_to_reference (id.in ());

     Test::Simple_Test_var simple_test =
       Test::Simple_Test::_narrow (object.in ());

     CORBA::String_var simple_test_ior =
       this->sorb_->object_to_string (simple_test.in ());

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

     poa_manager->activate ();

     // Signal the main thread before we call orb->run ();
     this->me_.signal ();

     this->sorb_->run ();

     ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

     ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - verifing results\n"));

     CORBA::ULong errors = hello_impl->error_count ();

     error_count_ += errors;
   }
 catch (const CORBA::Exception& ex)
   {
     error_count_ ++;
     ex._tao_print_exception ("Exception caught:");
     return 1;
   }
 catch (...)
   {
     error_count_ ++;
     ACE_ERROR ((LM_ERROR, "(%P|%t)Server_Task::svc - caught unknown exception\n"));
     return 1;
   }

 return 0;
}

CORBA::ULong
Server_Task::error_count () const
{
  return error_count_;
}

