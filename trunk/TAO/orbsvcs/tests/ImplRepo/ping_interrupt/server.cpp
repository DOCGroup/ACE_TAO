// $Id$

// server.cpp
// This version uses the Implementation Repository.

#include "Test_i.h"
#include "Server_ORBInitializer.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ImR_Client/ImR_Client.h"
#include "tao/ORBInitializer_Registry.h"

#include "ace/streams.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_unistd.h"

namespace
{
  ACE_CString toStr(int n)
  {
    char buf[20];
    return ACE_OS::itoa(n, buf, 10);
  }
}

PortableServer::POA_ptr
createPOA (PortableServer::POA_ptr root_poa,
           bool share_mgr,
           const char* poa_name)
{
  PortableServer::LifespanPolicy_var life =
    root_poa->create_lifespan_policy(PortableServer::PERSISTENT);

  PortableServer::IdAssignmentPolicy_var assign =
    root_poa->create_id_assignment_policy(PortableServer::USER_ID);

  CORBA::PolicyList pols;
  pols.length(2);
  pols[0] = PortableServer::LifespanPolicy::_duplicate(life.in());
  pols[1] = PortableServer::IdAssignmentPolicy::_duplicate(assign.in());

  PortableServer::POAManager_var mgr = PortableServer::POAManager::_nil();
  if (share_mgr)
    {
      mgr = root_poa->the_POAManager();
    }
  PortableServer::POA_var poa =
    root_poa->create_POA(poa_name, mgr.in(), pols);

  life->destroy();
  assign->destroy();

  return poa._retn();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      int server_num = 0;
      int die_on_ping = 1;
      int ping_count = 0;

      for (int i = 1; i < argc; i++)
        {
          ACE_TCHAR *c = argv[i];
          if (ACE_OS::strcasecmp (ACE_TEXT ("-n"), c) == 0)
            {
              server_num = ACE_OS::atoi (argv[++i]);
            }
          else if (ACE_OS::strcasecmp (ACE_TEXT ("-?"),c) == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("usage: %C ")
                          ACE_TEXT ("-n Number of the server\n"),
                          argv[0]));
              return 1;
            }
        }

      Server_ORBInitializer * temp_initializer;

      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer (&ping_count),
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in ());

      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj.in());

      PortableServer::POAManager_var mgr = root_poa->the_POAManager();

      ACE_CString poa_name_base = ACE_CString("TestObject_") + toStr (server_num);
      PortableServer::POA_var test_poa;
      test_poa = createPOA(root_poa.in (), true, poa_name_base.c_str ());
      temp_initializer->set_poa (test_poa.in());

      mgr->activate();

      PortableServer::Servant_var<Test_i> test_servant =
        new Test_i(server_num);

      PortableServer::ObjectId_var object_id =
        PortableServer::string_to_ObjectId("test_object");

    //
    // Activate the servant with the test POA,
    // obtain its object reference, and get a
    // stringified IOR.
    //
    test_poa->activate_object_with_id(object_id.in(), test_servant.in());

    //
    // Create binding between "TestService" and
    // the test object reference in the IOR Table.
    // Use a TAO extension to get the non imrified poa
    // to avoid forwarding requests back to the ImR.

    TAO_Root_POA* tpoa = dynamic_cast<TAO_Root_POA*>(test_poa.in());
    obj = tpoa->id_to_reference_i(object_id.in(), false);
    CORBA::String_var test_ior = orb->object_to_string(obj.in());
    obj = orb->resolve_initial_references("IORTable");
    IORTable::Table_var table = IORTable::Table::_narrow(obj.in());
    table->bind(poa_name_base.c_str (), test_ior.in());

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Started Server %C \n"),
                poa_name_base.c_str()));

    {
      ACE_CString status_file = poa_name_base + ACE_CString(".status");
      ofstream out(status_file.c_str ());
      out << "started" << endl;
    }

    while (ping_count < die_on_ping)
      {
        orb->perform_work ();
      }

    root_poa->destroy(1,1);
    orb->destroy();

  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception (ACE_TEXT("Server main()"));
    return 1;
  }

  return 0;
}
