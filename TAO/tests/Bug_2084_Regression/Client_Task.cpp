
#include "Client_Task.h"
#include "TestC.h"
#include "Hello.h"
#include "tao/ORB_Core.h"
#include "tao/PortableServer/PortableServer.h"

Client_Task::Client_Task (ACE_TCHAR const *ior,
                          CORBA::ORB_ptr orb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , input_ (ior)
  , corb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Client_Task::svc ()
{
  try
    {
      CORBA::Object_var const poa_object =
        this->corb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      PortableServer::ServantBase_var const hello_impl (
        new Hello (this->corb_.in ()));

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (hello_impl.in ());

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::Hello_var const hello_servant = Test::Hello::_narrow (object.in ());
      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG,
                  "Client (%t) optimize_collocation_objects=%d use_global_collocation=%d\n",
                  this->corb_->orb_core ()->optimize_collocation_objects (),
                  this->corb_->orb_core ()->use_global_collocation ()));

      CORBA::Object_var const tmp =
        this->corb_->string_to_object (input_);

      Test::EventNode_var const event_node =
        Test::EventNode::_narrow (tmp.in ());

      if (CORBA::is_nil (event_node.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::EventNode reference <%s>\n",
                             input_),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Client starting\n"));

      event_node->registerHello (hello_servant.in ());

      event_node->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
