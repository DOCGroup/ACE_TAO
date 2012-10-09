// $Id$

#include "masterS.h"
#include "ace/Get_Opt.h"
#include "tao/ORB.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

class ServerImpl : public POA_MasterClient::Server
{
public:
    ServerImpl (CORBA::ORB_ptr orb) : orb_ (CORBA::ORB::_duplicate (orb))
    {
    }

    virtual void ping()
    {
    }

    virtual void shutdown ()
    {
      this->orb_->shutdown (0);
    }
private:
    CORBA::ORB_var orb_;
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object = orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow(poa_object.in());
      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      ServerImpl *hello_impl = 0;
      ACE_NEW_RETURN (hello_impl,
                      ServerImpl (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      MasterClient::Server_var vserver = MasterClient::Server::_narrow (object.in ());

      CORBA::Object_var tmp = orb->string_to_object(ior);

      MasterClient::Master_var vmaster = MasterClient::Master::_narrow(tmp.in ());

      poa_manager->activate ();

      vmaster->registerServer(vserver.in());

      orb->run();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
