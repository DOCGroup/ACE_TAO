#include "Server_Task.h"
#include "TestS.h"
#include "Hello.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

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
{
}

int
Server_Task::svc ()
{
 try
   {
     CORBA::Object_var const poa_object =
       this->sorb_->resolve_initial_references ("RootPOA");

     PortableServer::POA_var const root_poa =
       PortableServer::POA::_narrow (poa_object.in ());

     if (CORBA::is_nil (root_poa.in ()))
       ACE_ERROR_RETURN ((LM_ERROR,
                          " (%P|%t) Panic: nil RootPOA\n"),
                         1);

     PortableServer::POAManager_var const poa_manager =
       root_poa->the_POAManager ();

     Hello *hello_impl {};
     ACE_NEW_RETURN (hello_impl,
                     Hello (this->sorb_.in (),
                            ACE_Thread::self ()),
                     1);

     // The servant wrapper owns hello_impl; keep the raw pointer as a borrowed alias.
     PortableServer::ServantBase_var const owner_transfer (hello_impl);

     PortableServer::ObjectId_var id =
       root_poa->activate_object (hello_impl);

     CORBA::Object_var object = root_poa->id_to_reference (id.in ());

     Test::Hello_var hello =
       Test::Hello::_narrow (object.in ());

     CORBA::String_var const ior =
       this->sorb_->object_to_string (hello.in ());

     // Output the IOR to the <this->output_>
     {
       std::unique_ptr<FILE, int (*) (FILE *)> output_file (
         ACE_OS::fopen (this->output_.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
       if (!output_file)
         ACE_ERROR_RETURN ((LM_ERROR,
                            "Cannot open output file for writing Hello IOR: %s",
                            this->output_.c_str ()),
                           1);

       ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
     }

     Test_Simple_Test_i *simple_impl {};
     ACE_NEW_RETURN (simple_impl,
                     Test_Simple_Test_i (),
                     1);

     // This wrapper owns simple_impl; the raw pointer remains a borrowed alias.
     PortableServer::ServantBase_var const owner_transfer_simple (simple_impl);

     id = root_poa->activate_object (simple_impl);

     object = root_poa->id_to_reference (id.in ());

     Test::Simple_Test_var const simple_test =
       Test::Simple_Test::_narrow (object.in ());

     CORBA::String_var const simple_test_ior =
       this->sorb_->object_to_string (simple_test.in ());

     // Output the IOR to the <this->output_>
     {
       std::unique_ptr<FILE, int (*) (FILE *)> simple_test_output_file (
         ACE_OS::fopen (this->simple_test_output_.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
       if (!simple_test_output_file)
         ACE_ERROR_RETURN ((LM_ERROR,
                            "Cannot open output file for writing Simple_Test IOR: %s",
                            this->simple_test_output_.c_str ()),
                           1);

       ACE_OS::fprintf (simple_test_output_file.get (), "%s", simple_test_ior.in ());
     }

     poa_manager->activate ();

     // Signal the main thread before we call orb->run ();
     this->me_.signal ();

     this->sorb_->run ();

     ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

     ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - verifing results\n"));

     CORBA::ULong errors = hello_impl->error_count ();

     error_count_ += errors;
   }
 catch (CORBA::Exception const &ex)
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

