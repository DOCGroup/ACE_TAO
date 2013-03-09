// $Id$

// server.cpp
// This version uses the Implementation Repository.

#include "Test_i.h"
#include "Terminator.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/Root_POA.h"

#include "ace/Get_Opt.h"
#include "ace/streams.h"
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

    int server_num = 0;
    int init_delay_secs = 0;

    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("d:n:?"));
    int c;

    while ((c = get_opts ()) != -1)
      switch (c)
        {
        case 'd':
          init_delay_secs = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'n':
          server_num = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case '?':
          ACE_DEBUG ((LM_DEBUG,
                      "usage: %s "
                      "-d <seconds to delay before initializing POA> "
                      "-n Number of the server\n",
                      argv[0]));
          return 1;
          break;
        }

    ACE_OS::sleep (init_delay_secs);

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj.in());

    PortableServer::POAManager_var mgr = root_poa->the_POAManager();

    ACE_CString poa_name = ACE_CString("TestObject_") + toStr (server_num);

    PortableServer::POA_var test_poa = createPOA(root_poa.in(), poa_name.c_str ());

    Terminator terminator;
    if (terminator.open (0) == -1)
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("main Error opening terminator\n")),-1);

    PortableServer::Servant_var<Test_i> test_servant =
      new Test_i(server_num, terminator);

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
    table->bind(poa_name.c_str (), test_ior.in());

    //
    // This server is now ready to run.
    // This version does not create an IOR
    // file as demonstrated in the
    // Developer's Guide.  It assumes that
    // users create IORs for the client using
    // the tao_imr utility.
    //
    //
    // Stop discarding requests.
    //
    mgr->activate();

    ACE_DEBUG ((LM_DEBUG,
      "Started Server %s \n",
      poa_name.c_str()));

    {
      ACE_CString status_file = poa_name + ACE_CString(".status");
      ofstream out(status_file.c_str ());
      out << "started" << endl;
    }

    orb->run();

    root_poa->destroy(1,1);
    orb->destroy();

  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception ("Server main()");
    return 1;
  }

  return 0;
}
