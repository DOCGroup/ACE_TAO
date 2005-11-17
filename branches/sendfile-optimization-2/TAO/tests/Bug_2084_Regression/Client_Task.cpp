//
// $Id$
//

#include "Client_Task.h"
#include "TestC.h"
#include "Hello.h"
#include "tao/ORB_Core.h"
#include "tao/PortableServer/PortableServer.h"

ACE_RCSID(Bug_2084_Regression,
          Client_Task,
          "$Id$")

Client_Task::Client_Task (const char *ior,
			  CORBA::ORB_ptr corb,
			  ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , input_ (ior)
    , corb_ (CORBA::ORB::_duplicate (corb))

{
}

int
Client_Task::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var poa_object =
        this->corb_->resolve_initial_references("RootPOA"
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

      Hello *hello_impl = 0;
      ACE_NEW_RETURN (hello_impl,
                      Hello (this->corb_.in (),
                             ACE_Thread::self ()),
                      1);

      PortableServer::ServantBase_var owner_transfer(hello_impl);

      Test::Hello_var hello_servant = hello_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);

      ACE_DEBUG((LM_DEBUG,"Client (%t) optimize_collocation_objects=%d use_global_collocation=%d\n",
                           corb_->orb_core()->optimize_collocation_objects(),
                           corb_->orb_core()->use_global_collocation ()));

      CORBA::Object_var tmp =
        this->corb_->string_to_object (input_
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::EventNode_var evNode=
        Test::EventNode::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (evNode.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::EventNode reference <%s>\n",
                             input_),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Client starting\n"));

      evNode->registerHello( hello_servant.in() ACE_ENV_ARG_PARAMETER );
      ACE_TRY_CHECK;

      evNode->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
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
