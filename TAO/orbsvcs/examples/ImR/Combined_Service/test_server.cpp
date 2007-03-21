// $Id$
// This is a simple test of an ImR using the corba interfaces
// It uses multicast to find the ImplRepoService

#include "testS.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/Root_POA.h"

#include "ace/streams.h"
#include "ace/ARGV.h"

using namespace CORBA;
using namespace PortableServer;

class test_i : public virtual POA_test {
  int n_;
public:
  test_i (void) : n_(0)
  {
  }
  virtual ~test_i (void) {
  }
  virtual CORBA::Long get (void)
  {
    ACE_DEBUG((LM_DEBUG, "dynserver: get() %d\n", ++n_));
    return n_;
  }
};

POA_ptr createPersistPOA(const char* name, POA_ptr root_poa, POAManager_ptr poaman) {
  CORBA::PolicyList policies (2);
  policies.length (2);
  policies[0] = root_poa->create_id_assignment_policy(USER_ID);
  policies[1] = root_poa->create_lifespan_policy(PERSISTENT);
  POA_var poa = root_poa->create_POA(name, poaman, policies);
  policies[0]->destroy();
  policies[1]->destroy();
  return poa._retn();
}

int main(int argc, char* argv[]) {

  try {

    ORB_var orb = ORB_init(argc, argv);

    Object_var obj = orb->resolve_initial_references("RootPOA");
    POA_var root_poa = POA::_narrow(obj.in());
    POAManager_var poaman = root_poa->the_POAManager();
    obj = orb->resolve_initial_references ("IORTable");
    IORTable::Table_var ior_table = IORTable::Table::_narrow (obj.in());
    ACE_ASSERT(! is_nil(ior_table.in()));

    ACE_DEBUG((LM_DEBUG, "test_server: creating poas. (Registers with ImR)\n"));

    POA_var poa1 = createPersistPOA("TestObject1", root_poa.in(), poaman.in());
    POA_var poa2 = createPersistPOA("TestObject2", root_poa.in(), poaman.in());

    ACE_DEBUG((LM_DEBUG, "test_server: activating objects.\n"));

    test_i svt1, svt2;

    ObjectId_var id = string_to_ObjectId("myobject");

    poa1->activate_object_with_id(id.in(), &svt1);
    poa2->activate_object_with_id(id.in(), &svt2);

    TAO_Root_POA* tmp_poa = dynamic_cast<TAO_Root_POA*>(poa1.in());
    obj = tmp_poa->id_to_reference_i (id.in(), false);
    String_var ior = orb->object_to_string(obj.in());
    ior_table->bind ("TestObject1", ior.in());

    tmp_poa = dynamic_cast<TAO_Root_POA*>(poa2.in());
    obj = tmp_poa->id_to_reference_i (id.in(), false);
    ior = orb->object_to_string(obj.in());
    ior_table->bind ("TestObject2", ior.in());

    poaman->activate();

    ACE_DEBUG((LM_DEBUG, "test_server: Running...\n"));

    orb->run();

    ACE_DEBUG((LM_DEBUG, "test_server: Shutting down...\n"));

    root_poa->destroy(1, 1);
    orb->destroy();

  } catch (CORBA::Exception& e) {
    e._tao_print_exception ("TestServer::init()");
  }
  return 0;
}
