// $Id$

// server.cpp
// This version uses the Implementation Repository.

#include "Test_i.h"
#include "Terminator.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ImR_Client/ImR_Client.h"

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

PortableServer::POA_var root_poa;
PortableServer::POA_var poa_a;
PortableServer::POA_var poa_b;

void
createPOAs(ACE_CString &base)
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
  ACE_CString poa_name = base + ACE_CString ("_a");
  poa_a = root_poa->create_POA(poa_name.c_str(), mgr.in(), pols);
  poa_name = base + ACE_CString ("_b");
  poa_b = root_poa->create_POA(poa_name.c_str(), mgr.in(), pols);
}

void
activatePOAs(void)
{
  PortableServer::POAManager_var mgr = root_poa->the_POAManager ();
  mgr->activate ();
  mgr = poa_a->the_POAManager ();
  mgr->activate ();
  mgr = poa_b->the_POAManager ();
  mgr->activate ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    int server_num = 0;

    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("n:?"));
    int c;

    while ((c = get_opts ()) != -1)
      switch (c)
        {
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

    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    root_poa = PortableServer::POA::_narrow (obj.in ());
    //    TAO_Root_POA::imr_client_adapter_name ("Test_Imr_Adapter");

    ACE_CString base = ACE_CString ("TestObject_") + toStr (server_num);
    createPOAs (base);

    Terminator terminator;
    if (terminator.open (0) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("main Error opening terminator\n")),-1);

    PortableServer::Servant_var<Test_i> test_servant =
      new Test_i (server_num, terminator, root_poa.in (), root_poa.in ());

    PortableServer::ObjectId_var object_id =
      PortableServer::string_to_ObjectId (base.c_str());

    poa_a->activate_object_with_id (object_id.in(), test_servant.in ());
    obj = poa_a->id_to_reference (object_id.in());

    Test_var tva = Test::_narrow (obj.in());

    poa_b->activate_object_with_id (object_id.in(), test_servant.in ());
    obj = poa_b->id_to_reference (object_id.in());

    Test_var tvb = Test::_narrow (obj.in());

    //    object_id = root_poa->activate_object (test_servant.in());

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
    activatePOAs ();

    //
    // Create binding between "TestService" and
    // the test object reference in the IOR Table.
    // Use a TAO extension to get the non imrified poa
    // to avoid forwarding requests back to the ImR.

    TAO_Root_POA* tpoa = dynamic_cast<TAO_Root_POA*> (poa_a.in ());
    ACE_ASSERT (tpoa != 0);

    obj = tpoa->id_to_reference_i (object_id.in (), false);
    CORBA::String_var test_ior = orb->object_to_string (obj.in ());
    obj = orb->resolve_initial_references("IORTable");
    IORTable::Table_var table = IORTable::Table::_narrow (obj.in ());
    table->bind(base.c_str (), test_ior.in ());

    test_ior = orb->object_to_string (tva.in());
    base += "_a";
    ACE_DEBUG ((LM_DEBUG, "%s:\n%s\n", base.c_str(), test_ior.in()));
    table->bind (base.c_str (), test_ior.in ());
    base[base.length()-1] = 'b';
    test_ior = orb->object_to_string (tvb.in());
    ACE_DEBUG ((LM_DEBUG, "%s:\n%s\n", base.c_str(), test_ior.in()));
    table->bind (base.c_str (), test_ior.in ());

    ACE_DEBUG ((LM_DEBUG,
      "Started Server %s \n",
      base.c_str()));

    {
      ACE_CString status_file = base + ACE_CString(".status");
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
