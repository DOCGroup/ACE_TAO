// server.cpp
// This version uses the Implementation Repository.

#include "Test_i.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ImR_Client/ImR_Client.h"

#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"

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
  int ec = 0;
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    int max_run = 0;
    int server_num = 0;
    int init_delay_secs = 0;
    const ACE_TCHAR *ior_file_name = ACE_TEXT ("");
    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("d:m:n:o:?"));
    int c;

    while ((c = get_opts ()) != -1)
      switch (c)
        {
        case 'd':
          init_delay_secs = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'm':
          max_run = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'n':
          server_num = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'o':
          ior_file_name = get_opts.opt_arg ();
          break;
        case '?':
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("usage: %s ")
                      ACE_TEXT ("-d <seconds to delay before initializing POA> ")
                      ACE_TEXT ("-n Number of the server\n"),
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

    Test_i *impl = new Test_i(orb.in());
    PortableServer::Servant_var<Test_i> test_servant = impl;

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

    if (ACE_OS::strlen (ior_file_name) > 0)
      {
        obj = test_poa->id_to_reference (object_id.in ());
        test_ior = orb->object_to_string (obj.in ());
        FILE *f = ACE_OS::fopen (ior_file_name, ACE_TEXT ("w"));
        ACE_OS::fprintf (f, "%s", test_ior.in ());
        ACE_OS::fclose (f);
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
                ACE_TEXT ("Started Server <%C> pid = %d\n"),
                poa_name.c_str(), ACE_OS::getpid ()));

    ACE_CString pid_file = "server.pid";
    {
      ofstream out(pid_file.c_str (), ios_base::app);
      out << ACE_OS::getpid () << endl;
      out.close ();
    }
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Server <%C> pid = %d closed pidfile\n"),
                poa_name.c_str(), ACE_OS::getpid ()));

    if (max_run > 0)
      {
        ACE_Time_Value tv (max_run,0);
        orb->run (tv);
      }
    else
      {
        orb->run ();
      }
    ec = impl->exit_code ();
    root_poa->destroy(1,1);
    orb->destroy();

    //    ACE_OS::unlink (pid_file.c_str ());
  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception (ACE_TEXT ("Server main()"));
    return 1;
  }

  return ec;
}
