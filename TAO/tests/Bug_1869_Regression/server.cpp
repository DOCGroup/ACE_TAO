#include "AMIS.h"
#include "tao/IORTable/IORTable.h"

class AdderServant
  : public POA_AMI_test::adder
{
public:
  AdderServant () = default;
  AdderServant (AdderServant const &) = delete;
  AdderServant &operator= (AdderServant const &) = delete;
  AdderServant (AdderServant &&) = delete;
  AdderServant &operator= (AdderServant &&) = delete;

  ~AdderServant () override = default;

  CORBA::Long add (CORBA::Long a, CORBA::Long b) override
  {
    return a + b;
  }
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const root_poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (root_poa_object.in ());
      PortableServer::POAManager_var const manager =
        root_poa->the_POAManager ();
      manager->activate ();

      PortableServer::LifespanPolicy_var const lifespan =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
      PortableServer::IdAssignmentPolicy_var const id_assignment =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      CORBA::PolicyList policies (2);
      policies.length (2);
      policies[0] = PortableServer::IdAssignmentPolicy::_duplicate (id_assignment.in ());
      policies[1] = PortableServer::LifespanPolicy::_duplicate (lifespan.in ());

      PortableServer::POA_var const persistent_poa =
        root_poa->create_POA ("persistentPOA", manager.in (), policies);

      id_assignment->destroy ();
      lifespan->destroy ();

      AdderServant servant;
      PortableServer::ObjectId_var const oid =
        PortableServer::string_to_ObjectId ("Adder");

      // The POA borrows this stack servant; it remains alive throughout ORB::run.
      persistent_poa->activate_object_with_id (oid.in (), &servant);

      CORBA::Object_var const reference = persistent_poa->id_to_reference (oid.in ());
      CORBA::String_var const ior = orb->object_to_string (reference.in ());

      CORBA::Object_var const table_object =
        orb->resolve_initial_references ("IORTable");
      IORTable::Table_var const table = IORTable::Table::_narrow (table_object.in ());
      table->bind ("Adder", ior.in ());

      orb->run ();
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Caught exception in server"));
      return 1;
    }
  return 0;
}

