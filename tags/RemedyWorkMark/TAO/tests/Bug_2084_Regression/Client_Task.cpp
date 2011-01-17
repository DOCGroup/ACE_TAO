//
// $Id$
//

#include "Client_Task.h"
#include "TestC.h"
#include "Hello.h"
#include "tao/ORB_Core.h"
#include "tao/PortableServer/PortableServer.h"

Client_Task::Client_Task (const ACE_TCHAR *ior,
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
  try
    {
      CORBA::Object_var poa_object =
        this->corb_->resolve_initial_references("RootPOA");

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
                      Hello (this->corb_.in (),
                             ACE_Thread::self ()),
                      1);

      PortableServer::ServantBase_var owner_transfer(hello_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Hello_var hello_servant = Test::Hello::_narrow (object.in ());
      poa_manager->activate ();

      ACE_DEBUG((LM_DEBUG,"Client (%t) optimize_collocation_objects=%d use_global_collocation=%d\n",
                           corb_->orb_core()->optimize_collocation_objects(),
                           corb_->orb_core()->use_global_collocation ()));

      CORBA::Object_var tmp =
        this->corb_->string_to_object (input_);

      Test::EventNode_var evNode=
        Test::EventNode::_narrow(tmp.in ());

      if (CORBA::is_nil (evNode.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::EventNode reference <%s>\n",
                             input_),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Client starting\n"));

      evNode->registerHello( hello_servant.in() );

      evNode->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;

}
