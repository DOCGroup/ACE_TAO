// $Id$

#include "AMIS.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *hostname = ACE_TEXT ("localhost");
const ACE_TCHAR *port = ACE_TEXT ("12345");

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

int
parse_args (int argc, ACE_TCHAR *argv[])
{
    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("h:p:"));
    int c;

    while ((c = get_opts ()) != -1)
      switch (c)
        {
        case 'h':
            hostname = get_opts.opt_arg ();
            break;
        case 'p':
            port = get_opts.opt_arg ();
            break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage:  %s "
                             "-h <hostname> "
                             "-p <port> "
                             "\n",
                             argv [0]),
                            -1);
        }
    // Indicates successful parsing of the command line
    return 0;
  }


int ACE_TMAIN (int argc, ACE_TCHAR* argv[]) {

ACE_TCHAR address_servant[1000];

  try {

    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0)
        return 1;
    // Get reference to Root POA
    CORBA::Object_var obj
      = orb->resolve_initial_references ("RootPOA");

    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow (obj.in ());

    // Activate POA manager
    PortableServer::POAManager_var mgr
      = rootPOA->the_POAManager ();

    mgr->activate();

    ACE_OS::sprintf(address_servant, ACE_TEXT("corbaloc:iiop:%s:%s/Adder"), hostname, port);

    // Resolve Adder Reference
    obj = orb->string_to_object(ACE_TEXT_ALWAYS_CHAR(address_servant));
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



