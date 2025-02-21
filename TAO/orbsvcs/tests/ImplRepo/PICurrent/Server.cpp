#include "Test_i.h"
#include "Terminator.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ImR_Client/ImR_Client.h"

#include <iostream>

PortableServer::POA_ptr
createPOA(PortableServer::POA_ptr root_poa, const char* poa_name)
{
  PortableServer::LifespanPolicy_var life =
    root_poa->create_lifespan_policy(PortableServer::PERSISTENT);

  PortableServer::IdAssignmentPolicy_var assign =
    root_poa->create_id_assignment_policy(PortableServer::USER_ID);

  CORBA::PolicyList pols;
  pols.length(2);
  pols[0] = PortableServer::LifespanPolicy::_duplicate(life.in());
  pols[1] = PortableServer::IdAssignmentPolicy::_duplicate(assign.in());

  PortableServer::POAManager_var mgr = root_poa->the_POAManager();
  PortableServer::POA_var poa =
    root_poa->create_POA(poa_name, mgr.in(), pols);

  life->destroy();
  assign->destroy();

  return poa._retn();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj.in());

    PortableServer::POAManager_var mgr = root_poa->the_POAManager();

    const char* poa_name = "TestService";

    PortableServer::POA_var test_poa = createPOA(root_poa.in(), poa_name);

    Terminator terminator;
    if (terminator.open (0) == -1)
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("main Error opening terminator\n")),-1);

    PortableServer::Servant_var<Test_i> test_servant =
      new Test_i(terminator);

    PortableServer::ObjectId_var object_id =
      PortableServer::string_to_ObjectId("test_object");

    test_poa->activate_object_with_id(object_id.in(), test_servant.in());

    TAO_Root_POA* tpoa = dynamic_cast<TAO_Root_POA*>(test_poa.in());
    obj = tpoa->id_to_reference_i(object_id.in(), false);
    CORBA::String_var test_ior = orb->object_to_string(obj.in());
    obj = orb->resolve_initial_references("IORTable");
    IORTable::Table_var table = IORTable::Table::_narrow(obj.in());
    table->bind(poa_name, test_ior.in());

    mgr->activate();

    std::cout << "Test server ready." << std::endl;

    orb->run();

    std::cout << "Test server shutting down." << std::endl;

    root_poa->destroy(1,1);
    orb->destroy();

    ACE_Message_Block *mb;
    ACE_NEW_RETURN(mb, ACE_Message_Block(0, ACE_Message_Block::MB_HANGUP), -1);
    terminator.putq(mb);
    terminator.wait();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Server main() Caught CORBA::Exception" << ex << std::endl;
    return 1;
  }

  return 0;
}
