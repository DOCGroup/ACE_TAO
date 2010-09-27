// $Id$
#include "TestS.h"
#include "tao/Stub.h"
#include "ace/OS_NS_string.h"

class Hello : public virtual POA_Test::Hello
{
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, " (%P|%t) Panic: nil RootPOA\n"), 1);

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      PortableServer::LifespanPolicy_var life =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      PortableServer::IdAssignmentPolicy_var assign =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      CORBA::PolicyList pols;
      pols.length (2);
      pols[0] = PortableServer::LifespanPolicy::_duplicate (life.in ());
      pols[1] = PortableServer::IdAssignmentPolicy::_duplicate (assign.in ());

      PortableServer::POA_var poa =
        root_poa->create_POA("ImRified POA", poa_manager.in (), pols);
      life->destroy();
      assign->destroy();

      Hello *hello_impl = 0;
      ACE_NEW_RETURN (hello_impl, Hello, 1);
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("Test 3891 Object");

      poa->activate_object_with_id (id.in (), hello_impl);

      CORBA::Object_var obj = poa->id_to_reference(id.in ());

      if (!obj->_stubobj ()->type_id.in () ||
          ACE_OS::strcmp (obj->_stubobj ()->type_id,
                          hello_impl->_repository_id ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR, "ERROR: type_id is incorrect\n"), 1);
        }

      root_poa->destroy (1, 1);
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
