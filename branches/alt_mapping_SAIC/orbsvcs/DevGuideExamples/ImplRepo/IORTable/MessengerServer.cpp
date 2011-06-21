// $Id$

#include "Messenger_i.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/IORTable/IORTable.h"
#include <iostream>
#include <fstream>
#include <fstream>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file1 = ACE_TEXT ("messenger1.ior");
const ACE_TCHAR *ior_output_file2 = ACE_TEXT ("messenger2.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file1 = get_opts.opt_arg ();
        break;

      case 'i':
        ior_output_file2 = get_opts.opt_arg ();
        break;

      case '?':
      default:
      ;
      }
  // Indicates successful parsing of the command line
  return 0;
}

PortableServer::POA_ptr
createPersistentPOA(PortableServer::POA_ptr root_poa,
                    const char* poa_name) {

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

void writeIORFile(const char* ior, const ACE_TCHAR* name) {
  std::ofstream out(ACE_TEXT_ALWAYS_CHAR (name));
  out << ior;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj.in());

    PortableServer::POAManager_var mgr = root_poa->the_POAManager();

    const char* poa_name = "MessengerService";

    PortableServer::POA_var poa = createPersistentPOA(root_poa.in(), poa_name);

    PortableServer::Servant_var<Messenger_i> servant1 = new Messenger_i;
    PortableServer::Servant_var<Messenger_i> servant2 = new Messenger_i;

    PortableServer::ObjectId_var id1 = PortableServer::string_to_ObjectId("Object1");
    poa->activate_object_with_id(id1.in(), servant1.in());
    PortableServer::ObjectId_var id2 = PortableServer::string_to_ObjectId("Object2");
    poa->activate_object_with_id(id2.in(), servant2.in());

    obj = poa->id_to_reference(id1.in());
    CORBA::String_var ior1 = orb->object_to_string(obj.in());
    obj = poa->id_to_reference(id2.in());
    CORBA::String_var ior2 = orb->object_to_string(obj.in());

    TAO_Root_POA* tpoa = dynamic_cast<TAO_Root_POA*>(poa.in());
    obj = tpoa->id_to_reference_i(id1.in(), false);
    CORBA::String_var direct_ior1 = orb->object_to_string(obj.in());

    obj = orb->resolve_initial_references("IORTable");
    IORTable::Table_var ior_table = IORTable::Table::_narrow(obj.in());
    ior_table->bind("MessengerService/Object1", direct_ior1.in());
    ior_table->bind("MessengerService/Object2", ior2.in());

    writeIORFile(ior1.in(), ior_output_file1);
    writeIORFile(ior2.in(), ior_output_file2);

    mgr->activate();

    std::cout << "Messenger server ready." << std::endl;

    orb->run();

    std::cout << "Messenger server shutting down." << std::endl;

    root_poa->destroy(1,1);
    orb->destroy();

    return 0;
  } catch(const CORBA::Exception& ex) {
    std::cerr << "Server main() Caught Exception" << ex << std::endl;
  }
  return 1;
}
