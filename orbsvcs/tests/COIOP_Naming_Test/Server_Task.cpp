// $Id$

#include "Server_Task.h"
#include "TestS.h"
#include "Hello.h"

#include "ace/Manual_Event.h"

Server_Task::Server_Task (CosNaming::NamingContext_ptr root_context,
                          CORBA::ORB_ptr sorb,
                          ACE_Manual_Event &me,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , root_context_ (CosNaming::NamingContext::_duplicate (root_context))
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

     Hello *hello_impl = 0;
     ACE_NEW_RETURN (hello_impl,
                     Hello (this->sorb_.in (),
                            ACE_Thread::self ()),
                     1);

     PortableServer::ServantBase_var owner_transfer(hello_impl);

     Test::Hello_var hello =
       hello_impl->_this ();

     CosNaming::NamingContext_var example_nc;
     CosNaming::Name name;
     name.length(1);
     name[0].id = CORBA::string_dup("Hello");
     root_context_->rebind(name, hello.in());

     poa_manager->activate ();

     // Signal the main thread before we call orb->run ();
     this->me_.signal ();

     ACE_Time_Value runtime (10);
     this->sorb_->run (runtime);

     ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
   }
 catch (const CORBA::Exception& ex)
   {
     ex._tao_print_exception ("Exception caught:");
     return 1;
   }

 return 0;
}
