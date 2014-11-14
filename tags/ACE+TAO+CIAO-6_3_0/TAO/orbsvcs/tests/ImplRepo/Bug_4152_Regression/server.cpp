// $Id$

// server.cpp
// This version uses the Implementation Repository.

#include "Test_i.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ImR_Client/ImR_Client.h"

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"

class ORB_Runner : public ACE_Task_Base
{
public:
  ORB_Runner (CORBA::ORB_var orb) : orb_(orb) {}
  int svc (void)
  {
    this->orb_->run ();
    return 0;
  }

private:
  CORBA::ORB_var orb_;
};


PortableServer::POA_var root_poa;
PortableServer::POA_var poa_a;

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
}

void
activatePOAs (void)
{
  PortableServer::POAManager_var mgr = root_poa->the_POAManager ();
  mgr->activate ();
  mgr = poa_a->the_POAManager ();
  mgr->activate ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
  ORB_Runner *runner = new ORB_Runner (orb);
  int poa_delay = 10;

  try
    {
      ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("p:?"));
      int c;

      while ((c = get_opts ()) != -1)
        switch (c)
          {
          case 'p':
            poa_delay = ACE_OS::atoi (get_opts.opt_arg ());
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

      ACE_OS::sleep (poa_delay);
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      root_poa = PortableServer::POA::_narrow (obj.in ());

      runner->activate ();

      ACE_CString base = ACE_CString ("TestObject");
      createPOAs (base);

      PortableServer::Servant_var<Test_i> test_servant = new Test_i;

      PortableServer::ObjectId_var object_id =
        PortableServer::string_to_ObjectId (base.c_str());

      poa_a->activate_object_with_id (object_id.in(), test_servant.in ());
      obj = poa_a->id_to_reference (object_id.in());

      Test_var tva = Test::_narrow (obj.in());

      ACE_DEBUG ((LM_DEBUG, "Started Server pid = %d poa delay %d\n", ACE_OS::getpid (), poa_delay));

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
      ACE_Time_Value tv (poa_delay);
      ACE_OS::sleep (tv);
      activatePOAs ();

      ACE_DEBUG ((LM_DEBUG, "Activated POA pid = %d \n", ACE_OS::getpid ()));

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

      runner->wait ();
      root_poa->destroy(1,1);
      orb->destroy();
    }
  catch(const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Server main()");
      return 1;
    }

  delete runner;
  orb = CORBA::ORB::_nil ();
  ACE_DEBUG ((LM_DEBUG, "Exiting Server pid = %d \n",
              ACE_OS::getpid ()));

  return 0;
}
