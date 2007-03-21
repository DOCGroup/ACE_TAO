// $Id$

#include "AMIS.h"
#include "tao/IORTable/IORTable.h"

class  AdderServant :
  public POA_AMI_test::adder {

public:
  AdderServant (void) {}

  virtual ~AdderServant (void) {}

  virtual
  CORBA::Long add (
      CORBA::Long a,
      CORBA::Long b
    )
    {
      return a + b;
    }
};


int main (int argc, char* argv[]) {

  try {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    // Get reference to Root POA
    CORBA::Object_var obj
      = orb->resolve_initial_references ("RootPOA");

    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow (obj.in ());

    // Activate POA manager
    PortableServer::POAManager_var mgr
      = rootPOA->the_POAManager ();

    mgr->activate();

    // Create Persistent Lifespan Policy and User Id Policy
    PortableServer::LifespanPolicy_var lifespan =
      rootPOA->create_lifespan_policy(PortableServer::PERSISTENT);
    PortableServer::IdAssignmentPolicy_var idassignment =
      rootPOA->create_id_assignment_policy(PortableServer::USER_ID);

    // Stuff them into a policy list
    CORBA::PolicyList policies(2);
    policies.length(2);
    policies[0] = PortableServer::IdAssignmentPolicy::_duplicate(idassignment.in());
    policies[1] = PortableServer::LifespanPolicy::_duplicate(lifespan.in());

    // Create the Child POA
    PortableServer::POA_var persistentPOA =
      rootPOA->create_POA("persistentPOA", mgr.in(), policies);

    // Policies are no longer needed
    idassignment->destroy();
    lifespan->destroy();

    AdderServant servant;

    // Create an id
    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId("Adder");

    // Activate the object (with id)
    persistentPOA->activate_object_with_id(oid.in(), & servant);

    CORBA::Object_var ref = persistentPOA->id_to_reference(oid.in());
    CORBA::String_var iors = orb->object_to_string(ref.in());

    CORBA::Object_var tobj = orb->resolve_initial_references("IORTable");
    IORTable::Table_var table = IORTable::Table::_narrow(tobj.in());
    table->bind("Adder", iors.in ());

    orb->run();
  } catch (...) {
     ACE_ERROR ((LM_ERROR, "ERROR: Caught exception in server"));
     return 1;
  }
  return 0;
}

