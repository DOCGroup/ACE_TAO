// server.cpp
// This version uses the Implementation Repository.

#include "Test_i.h"

#include "tao/PortableServer/Root_POA.h"
#include "tao/ImR_Client/ImR_Client.h"

#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"

PortableServer::POA_var root_poa;
PortableServer::POA_var poa_a;
PortableServer::POA_var poa_b;
PortableServer::POA_var poa_c;

void
createPOAs (ACE_CString &base)
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
  poa_name = base + ACE_CString ("_c");
  poa_c = root_poa->create_POA(poa_name.c_str(), mgr.in(), pols);
}

void
activatePOAs()
{
  PortableServer::POAManager_var mgr = root_poa->the_POAManager ();
  mgr->activate ();
  mgr = poa_a->the_POAManager ();
  mgr->activate ();
  mgr = poa_b->the_POAManager ();
  mgr->activate ();
  mgr = poa_c->the_POAManager ();
  mgr->activate ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    int delay = 0;
    bool write_iors = false;

    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("d:w"));
    int c;

    while ((c = get_opts ()) != -1)
      switch (c)
        {
        case 'd':
          delay = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'w':
          write_iors = true;
          break;
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "usage: %s "
                      "-d <seconds to delay before initializing POA> "
                      "-w\n",
                      argv[0]));
          return 1;
          break;
        }

    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    root_poa = PortableServer::POA::_narrow (obj.in ());

    ACE_CString base = ACE_CString ("TestObject");

    if (!write_iors)
      {
        ACE_CString status_file = base + ACE_CString(".status");
        ofstream out(status_file.c_str (), ios_base::app);
        if (!out.good())
          {
            ACE_DEBUG ((LM_DEBUG, "server did not get good bit from %s\n", status_file.c_str()));
          }
        out << ACE_OS::getpid () << endl;
        out.close ();
      }

    ACE_OS::sleep (delay);

    createPOAs (base);

    PortableServer::Servant_var<Test_i> test_servant = new Test_i;

    PortableServer::ObjectId_var object_id =
      PortableServer::string_to_ObjectId (base.c_str());

    poa_a->activate_object_with_id (object_id.in(), test_servant.in ());
    obj = poa_a->id_to_reference (object_id.in());
    Test_var tva = Test::_narrow (obj.in());

    poa_b->activate_object_with_id (object_id.in(), test_servant.in ());
    obj = poa_b->id_to_reference (object_id.in());
    Test_var tvb = Test::_narrow (obj.in());

    poa_c->activate_object_with_id (object_id.in(), test_servant.in ());
    obj = poa_c->id_to_reference (object_id.in());
    Test_var tvc = Test::_narrow (obj.in());

    if (write_iors)
      {
        ACE_DEBUG ((LM_DEBUG, "(%P) writing IOR files\n"));
        CORBA::String_var iorstr = orb->object_to_string (tva.in ());
        ACE_CString filename = base + "_a.ior";
        FILE *iorf = ACE_OS::fopen (filename.c_str(), "w");
        ACE_OS::fprintf (iorf, "%s", iorstr.in ());
        ACE_OS::fclose (iorf);

        iorstr = orb->object_to_string (tvb.in ());
        filename = base + "_b.ior";
        iorf = ACE_OS::fopen (filename.c_str(), "w");
        ACE_OS::fprintf (iorf, "%s", iorstr.in ());
        ACE_OS::fclose (iorf);

        iorstr = orb->object_to_string (tvc.in ());
        filename = base + "_c.ior";
        iorf = ACE_OS::fopen (filename.c_str(), "w");
        ACE_OS::fprintf (iorf, "%s", iorstr.in ());
        ACE_OS::fclose (iorf);
      }

    activatePOAs ();

    ACE_DEBUG ((LM_DEBUG, "Started Server %s \n", base.c_str()));

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
