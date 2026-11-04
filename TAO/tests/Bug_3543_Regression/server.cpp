#include "masterS.h"
#include "ace/Get_Opt.h"
#include "tao/ORB.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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
  explicit ServerImpl (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }
  ServerImpl (ServerImpl const &) = delete;
  ServerImpl (ServerImpl &&) = delete;
  ServerImpl &operator= (ServerImpl const &) = delete;
  ServerImpl &operator= (ServerImpl &&) = delete;

  void ping () override {}

  void shutdown () override
  {
    this->orb_->shutdown (false);
  }
private:
  CORBA::ORB_var const orb_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      ServerImpl *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      ServerImpl (orb.in ()),
                      1);
      // ServantBase_var takes ownership of this pointer.
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      MasterClient::Server_var const vserver = MasterClient::Server::_narrow (object.in ());

      CORBA::Object_var const tmp = orb->string_to_object (ior);

      MasterClient::Master_var const vmaster = MasterClient::Master::_narrow (tmp.in ());

      poa_manager->activate ();

      vmaster->registerServer (vserver.in ());

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
