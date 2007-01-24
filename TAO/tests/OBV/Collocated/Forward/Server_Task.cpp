//
// $Id$
//
#include "Server_Task.h"
#include "TreeBaseS.h"
#include "TreeControllerS.h"
#include "TreeNodeS.h"
#include "Test_impl.h"

#include "ace/Manual_Event.h"

Server_Task::Server_Task (const char *output,
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
 ACE_TRY_NEW_ENV
   {
      // All factories are kindly provided by
      // compiler so we just to put everything in a right order.

      // Create and register factory for BaseNode.
      BaseNode_init *bn_factory = 0;
      ACE_NEW_RETURN (bn_factory,
                      BaseNode_init,
                      1);

      this->sorb_->register_value_factory (bn_factory->tao_repository_id (),
                                   bn_factory
                                   ACE_ENV_ARG_PARAMETER);
      bn_factory->_remove_ref (); // release ownership

      // Create and register factory for TreeController.
      TreeController_init *tc_factory = 0;
      ACE_NEW_RETURN (tc_factory,
                      TreeController_init,
                      1);

      this->sorb_->register_value_factory (tc_factory->tao_repository_id (),
                                   tc_factory
                                   ACE_ENV_ARG_PARAMETER);
      tc_factory->_remove_ref (); // release ownership

      // Create and register factory for StringNode.
      StringNode_init *sn_factory = 0;
      ACE_NEW_RETURN (sn_factory,
                      StringNode_init,
                      1);

      this->sorb_->register_value_factory (sn_factory->tao_repository_id (),
                                   sn_factory
                                   ACE_ENV_ARG_PARAMETER);
      sn_factory->_remove_ref (); // release ownership

      //Well, done with factories.

      CORBA::Object_var poa_object =
        this->sorb_->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Test_impl *test_impl;
      ACE_NEW_RETURN (test_impl,
                      Test_impl (this->sorb_.in ()),
                      1);

      PortableServer::ServantBase_var owner_transfer(test_impl);

      Test_var test = test_impl->_this ();

      CORBA::String_var ior =
        this->sorb_->object_to_string (test.in () ACE_ENV_ARG_PARAMETER);

      // If the this->output_ exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (this->output_, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           this->output_),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      this->me_.signal ();

      this->sorb_->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

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
