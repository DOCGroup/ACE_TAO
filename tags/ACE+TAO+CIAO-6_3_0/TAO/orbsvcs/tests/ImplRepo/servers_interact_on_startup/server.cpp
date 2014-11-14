// $Id$

// server.cpp
// This version uses the Implementation Repository.

#include "Test_i.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ImR_Client/ImR_Client.h"

#include "ace/Get_Opt.h"
#include "ace/streams.h"

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

    ACE_TCHAR *other_server_ior = 0;

    int server_num = 0;
    int reply_delay_secs = 0;

    bool expect_transient = false;

    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("es:d:n:?"));
    int c;

    while ((c = get_opts ()) != -1)
      switch (c)
        {
        case 'e':
          expect_transient = true;
          break;

        case 's':
          other_server_ior = get_opts.opt_arg ();
          break;

        case 'd':
          reply_delay_secs = ACE_OS::atoi (get_opts.opt_arg ());
          break;

        case 'n':
          server_num = ACE_OS::atoi (get_opts.opt_arg ());
          break;

        case '?':
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("usage: %C ")
                      ACE_TEXT ("-e ")
                      ACE_TEXT ("-s <ior-of-server-to-invoke> ")
                      ACE_TEXT ("-d <seconds to delay before initializing POA> ")
                      ACE_TEXT ("-n Number of the server\n"),
                      argv[0]));
          return 1;
          break;
        }

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj.in());

    PortableServer::POAManager_var mgr = root_poa->the_POAManager();

    ACE_CString poa_name = ACE_CString("TestObject_") + toStr (server_num);

    PortableServer::POA_var test_poa = createPOA(root_poa.in(), poa_name.c_str ());

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t|%T) Server %d created POA %C\n"),
                server_num, poa_name.c_str ()));

    PortableServer::Servant_var<Test_i> test_servant =
      new Test_i(server_num, reply_delay_secs);

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

    // Make invocation to other server before POA is activated.
    if (other_server_ior != 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t|%T) Server %d sending request to %s\n"),
                    server_num, other_server_ior));
        try
          {
            CORBA::Object_var other_server =
              orb->string_to_object (other_server_ior);
            ACE_ASSERT (!CORBA::is_nil (other_server.in ()));
            Test_var test = Test::_narrow (other_server.in());
            ACE_ASSERT (!CORBA::is_nil (test.in()));
            CORBA::Short n = test->get_server_num ();
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t|%T) Server %d received reply from server %d\n"),
                        server_num, n));
          }
        catch (CORBA::TRANSIENT &)
          {
            if (expect_transient)
              {
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%P|%t|%T) Server %d got expected transient exception\n"),
                            server_num));
              }
            else
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("ERROR: (%P|%t|%T) Server %d got unexpected Transent\n"),
                            server_num));
              }
          }
      }


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
                ACE_TEXT ("(%P|%t|%T) Server %d started serving %C\n"),
                server_num, poa_name.c_str()));

    {
      ACE_CString status_file = poa_name + ACE_CString(".status");
      ofstream out(status_file.c_str (), ios_base::app);
      out << "started" << endl;
    }

    orb->run();

    root_poa->destroy(1,1);
    orb->destroy();

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t|%T) Server %d ending serving of %C\n"),
                server_num, poa_name.c_str ()));

  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception (ACE_TEXT ("Server main()"));
    return 1;
  }

  return 0;
}
