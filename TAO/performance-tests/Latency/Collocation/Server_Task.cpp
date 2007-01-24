//
// $Id$
//

#include "Server_Task.h"
#include "Roundtrip.h"

Server_Task::Server_Task (CORBA::ORB_ptr sorb,
                          ACE_Manual_Event &cond,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr),
    cond_ (cond),
    sorb_ (CORBA::ORB::_duplicate (sorb))
{
}

Test::Roundtrip *
Server_Task::get_reference ()
{
  return Test::Roundtrip::_duplicate (this->rt_var_.in ());
}

int
Server_Task::svc (void)
{
 ACE_TRY_NEW_ENV
   {
     CORBA::Object_var poa_object =
       this->sorb_->resolve_initial_references("RootPOA"
                                               ACE_ENV_ARG_PARAMETER);

     PortableServer::POA_var root_poa =
       PortableServer::POA::_narrow (poa_object.in ()
                                     ACE_ENV_ARG_PARAMETER);

     if (CORBA::is_nil (root_poa.in ()))
       ACE_ERROR_RETURN ((LM_ERROR,
                          " (%P|%t) Panic: nil RootPOA\n"),
                         1);

     PortableServer::POAManager_var poa_manager =
       root_poa->the_POAManager ();

     Roundtrip *rt_impl;

     ACE_NEW_RETURN (rt_impl,
                     Roundtrip (this->sorb_.in ()),
                     1);

     PortableServer::ServantBase_var owner_transfer(rt_impl);

     this->rt_var_ =
       rt_impl->_this ();

     if (CORBA::is_nil (this->rt_var_.in ()))
      {
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Error activating Test::Roundtrip reference\n"),
                          1);
      }

     poa_manager->activate ();

     ACE_DEBUG ((LM_DEBUG, "Server_Task: Object Activation complete\n"));

     // Signal the main thread to spawn the client
     this->cond_.signal ();

     this->sorb_->run ();

     root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);

     this->sorb_->destroy ();
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
