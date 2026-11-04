#include "AMIS.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"
  ACE_TCHAR const *hostname = ACE_TEXT ("localhost");
  ACE_TCHAR const *port = ACE_TEXT ("12345");

class AdderCallback
  : public POA_AMI_test::AMI_adderHandler,
    public ACE_Task_Base
{
public:
  explicit AdderCallback (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
    this->activate ();
  }
  AdderCallback (AdderCallback const &) = delete;
  AdderCallback &operator= (AdderCallback const &) = delete;
  AdderCallback (AdderCallback &&) = delete;
  AdderCallback &operator= (AdderCallback &&) = delete;

  void add (CORBA::Long ami_return_val) override
  {
    ACE_DEBUG ((LM_DEBUG, "Return value: %d\n", ami_return_val));
  }

  void add_excep (::Messaging::ExceptionHolder *) override
  {
    ACE_DEBUG ((LM_DEBUG, "Caught exception in _excep\n"));
  }

  int svc () override
  {
    this->orb_->run ();
    return 0;
  }

private:
  CORBA::ORB_var const orb_;
};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("h:p:"));
  int c {};

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
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR address_servant[1000] {};
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var const root_poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (root_poa_object.in ());
      PortableServer::POAManager_var const manager =
        root_poa->the_POAManager ();
      manager->activate ();

      ACE_OS::sprintf (address_servant,
                       ACE_TEXT ("corbaloc:iiop:%s:%s/Adder"),
                       hostname,
                       port);

      CORBA::Object_var const object =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (address_servant));
      AMI_test::adder_var const adder = AMI_test::adder::_narrow (object.in ());
      AdderCallback callback (orb.in ());

      while (true)
        {
          try
            {
              adder->sendc_add (callback._this (), 3, 2);
            }
          catch (...)
            {
              ACE_ERROR ((LM_ERROR, "ERROR: Caught exception after sendc_add\n"));
            }
          ACE_OS::sleep (1);
        }
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Caught exception in main\n"));
      return 1;
    }
}
