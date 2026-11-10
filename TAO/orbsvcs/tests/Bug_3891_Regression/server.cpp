#include "TestS.h"
#include "tao/Stub.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#ifdef TAO_AS_STATIC_LIBS
# include "tao/ImR_Client/ImR_Client.h"
#endif

#include <cstring>

class Hello : public virtual POA_Test::Hello
{
public:
  Hello () = default;

  Hello (Hello const &) = delete;
  Hello (Hello &&) = delete;
  Hello &operator= (Hello const &) = delete;
  Hello &operator= (Hello &&) = delete;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Panic: nil RootPOA\n")),
                          1);

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      PortableServer::POA_var poa;
      {
        TAO::Utils::PolicyList_Destroyer policies (2);
        policies.length (2);
        policies[0] = root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
        policies[1] = root_poa->create_id_assignment_policy (PortableServer::USER_ID);
        poa = root_poa->create_POA ("ImRified POA", poa_manager.in (), policies);
      }

      Hello *hello_impl {};
      ACE_NEW_RETURN (hello_impl, Hello, 1);
      // ServantBase_var adopts the initial reference returned by ACE_NEW_RETURN.
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      PortableServer::ObjectId_var const id =
        PortableServer::string_to_ObjectId ("Test 3891 Object");

      poa->activate_object_with_id (id.in (), hello_impl);

      CORBA::Object_var const obj = poa->id_to_reference (id.in ());
      char const * const type_id = obj->_stubobj ()->type_id.in ();

      if (type_id == nullptr ||
          std::strcmp (type_id, hello_impl->_repository_id ()) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("ERROR: type_id is incorrect\n")), 1);
        }

      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
