// This is a simple test of an ImR using the corba interfaces
// It uses multicast to find the ImplRepoService

#include "testS.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/Root_POA.h"

#include "ace/Log_Msg.h"

#include <atomic>

using namespace CORBA;
using namespace PortableServer;

class test_i : public virtual POA_test
{
  std::atomic<CORBA::Long> n_ {CORBA::Long {}};
public:
  test_i () = default;
  ~test_i () override = default;

  test_i (test_i const &) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i &&) = delete;

  CORBA::Long get () override
  {
    CORBA::Long const result = ++n_;
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("test_server: get() %d\n"), result));
    return result;
  }
};

POA_ptr create_persistent_poa (char const *name, POA_ptr root_poa, POAManager_ptr poaman)
{
  CORBA::PolicyList policies (2);
  policies.length (2);
  policies[0] = root_poa->create_id_assignment_policy (USER_ID);
  policies[1] = root_poa->create_lifespan_policy (PERSISTENT);
  POA_var poa = root_poa->create_POA (name, poaman, policies);
  policies[0]->destroy ();
  policies[1]->destroy ();
  return poa._retn ();
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    ORB_var orb = ORB_init (argc, argv);

    Object_var obj = orb->resolve_initial_references ("RootPOA");
    POA_var root_poa = POA::_narrow (obj.in ());
    POAManager_var poaman = root_poa->the_POAManager ();
    obj = orb->resolve_initial_references ("IORTable");
    IORTable::Table_var ior_table = IORTable::Table::_narrow (obj.in ());
    ACE_ASSERT (!is_nil (ior_table.in ()));

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("test_server: creating poas. (Registers with ImR)\n")));

    POA_var poa1 = create_persistent_poa ("TestObject1", root_poa.in (), poaman.in ());
    POA_var poa2 = create_persistent_poa ("TestObject2", root_poa.in (), poaman.in ());

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("test_server: activating objects.\n")));

    test_i *svt1 = new test_i;
    ServantBase_var scoped_svt1 (svt1);
    test_i *svt2 = new test_i;
    ServantBase_var scoped_svt2 (svt2);

    ObjectId_var id = string_to_ObjectId ("myobject");

    poa1->activate_object_with_id (id.in (), svt1);
    poa2->activate_object_with_id (id.in (), svt2);

    TAO_Root_POA *tmp_poa = dynamic_cast<TAO_Root_POA *> (poa1.in ());
    obj = tmp_poa->id_to_reference_i (id.in (), false);
    String_var ior = orb->object_to_string (obj.in ());
    ior_table->bind ("TestObject1", ior.in ());

    tmp_poa = dynamic_cast<TAO_Root_POA *> (poa2.in ());
    obj = tmp_poa->id_to_reference_i (id.in (), false);
    ior = orb->object_to_string (obj.in ());
    ior_table->bind ("TestObject2", ior.in ());

    poaman->activate ();

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("test_server: Running...\n")));

    orb->run ();

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("test_server: Shutting down...\n")));

    root_poa->destroy (true, true);
    orb->destroy ();
  } catch (CORBA::Exception const &e) {
    e._tao_print_exception ("TestServer::init()");
    return 1;
  }
  return 0;
}
