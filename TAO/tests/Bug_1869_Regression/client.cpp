// $Id$

#include "AMIS.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"

class AdderCallback
  : public POA_AMI_test::AMI_adderHandler,
    public ACE_Task_Base {

  public:
    AdderCallback(CORBA::ORB_ptr orb)
      : orb(CORBA::ORB::_duplicate(orb)) {
      activate();
    }

    virtual void add (
        CORBA::Long ami_return_val
      )
      {
        ACE_DEBUG ((LM_DEBUG, "Return value: %d\n", ami_return_val));
      }

    virtual void add_excep (
        ::Messaging::ExceptionHolder *
      )
      {
        ACE_DEBUG ((LM_DEBUG, "Caught exception in _excep\n"));
      }

    virtual int svc() {
      orb->run();

      return 0;
    }

    CORBA::ORB_var orb;

};

int main (int argc, char* argv[]) {

  try {

    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    // Get reference to Root POA
    CORBA::Object_var obj
      = orb->resolve_initial_references ("RootPOA");

    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow (obj.in ());

    // Activate POA manager
    PortableServer::POAManager_var mgr
      = rootPOA->the_POAManager ();

    mgr->activate();

    // Resolve Adder Reference
    obj = orb->string_to_object("corbaloc:iiop:127.0.0.1:4711/Adder");
    AMI_test::adder_var adder = AMI_test::adder::_narrow(obj.in ());

    AdderCallback cb_servant(orb.in ());

    while(true) {
      try
      {
        adder->sendc_add(cb_servant._this(), 3, 2);
      }
      catch(...)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Caught exception after sendc_add\n"));
      }
      ACE_OS::sleep (1);
    }
  }
  catch(...)
  {
    ACE_ERROR ((LM_ERROR, "ERROR: Caught exception in main\n"));
    return 1;
  }

  return 0;
}



