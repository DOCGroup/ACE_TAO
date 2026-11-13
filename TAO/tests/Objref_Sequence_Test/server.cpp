#include "TestS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>
#include <string>

struct Options
{
  std::basic_string<ACE_TCHAR> ior_output_file {ACE_TEXT ("test.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

/// Implement the Server interface.
class ServerServant : public POA_Server
{
public:
  ServerServant (PortableServer::POA_ptr poa,
                 CORBA::ORB_ptr orb);
  ServerServant (ServerServant const &) = delete;
  ServerServant (ServerServant &&) = delete;
  ServerServant &operator= (ServerServant const &) = delete;
  ServerServant &operator= (ServerServant &&) = delete;

  void CreateExtra (CORBA::ULong length,
                    ServerSequence_out seq) override;
  void DeleteExtra (ServerSequence const &seq) override;

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override;
  //FUZZ: enable check_for_lack_ACE_OS

private:
  PortableServer::POA_var root_poa_;
  CORBA::ORB_var orb_;
};

ServerServant::ServerServant (PortableServer::POA_ptr poa,
                              CORBA::ORB_ptr orb)
  : root_poa_ (PortableServer::POA::_duplicate (poa))
  , orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
ServerServant::CreateExtra (CORBA::ULong length,
                            ServerSequence_out seq)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Create extra called with length [%u]\n",
              length));

  ACE_NEW_THROW_EX (seq,
                    ServerSequence (length),
                    CORBA::NO_MEMORY ());
  seq->length (length);

  for (CORBA::ULong count {}; count < length; ++count)
    {
      ServerServant *new_servant {};
      ACE_NEW_THROW_EX (new_servant,
                        ServerServant (this->root_poa_.in (), this->orb_.in ()),
                        CORBA::NO_MEMORY ());
      std::unique_ptr<ServerServant> servant (new_servant);
      PortableServer::ObjectId_var const id =
        this->root_poa_->activate_object (servant.get ());
      // The POA owns the servant's initial reference after activation.
      servant.release ();
      CORBA::Object_var const object = this->root_poa_->id_to_reference (id.in ());
      (*seq)[count] = Server::_narrow (object.in ());
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Returned from CreateExtra ()\n"));
}

void
ServerServant::DeleteExtra (ServerSequence const &seq)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Deleting %u sequences\n",
              seq.length ()));

  PortableServer::ObjectId_var oid;
  PortableServer::ServantBase *servant {};

  for (CORBA::ULong count {}; count < seq.length (); ++count)
    {
      oid = this->root_poa_->reference_to_id (seq[count]);
      servant = this->root_poa_->reference_to_servant (seq[count]);
      this->root_poa_->deactivate_object (oid.in ());
      servant->_remove_ref ();
      servant->_remove_ref ();
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Returned after deleting sequences\n"));
}

void
ServerServant::shutdown ()
{
  this->orb_->shutdown (false);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return -1;

      CORBA::Object_var const root_poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (root_poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();
      ServerServant *new_servant {};
      ACE_NEW_RETURN (new_servant,
                      ServerServant (root_poa.in (), orb.in ()),
                      1);
      std::unique_ptr<ServerServant> servant (new_servant);
      ServerServant *const servant_impl = servant.get ();
      PortableServer::ServantBase_var const owner_transfer (servant.release ());
      PortableServer::ObjectId_var const id = root_poa->activate_object (servant_impl);
      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());
      Server_var const server = Server::_narrow (object.in ());
      CORBA::String_var const object_reference = orb->object_to_string (server.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             options.ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", object_reference.in ());
      }

      poa_manager->activate ();
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
