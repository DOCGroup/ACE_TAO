//$Id$
#include "TestS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

/// Implement the Server Interface
class ServerServant :
  public POA_Server
{
public:
  /// Ctor
  ServerServant (PortableServer::POA_ptr poa,
                 CORBA::ORB_ptr orb);

  void CreateExtra (CORBA::ULong length,
                    ServerSequence_out seq);

  void DeleteExtra (const ServerSequence &seq);

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown (void);
  //FUZZ: enable check_for_lack_ACE_OS

private:
  /// Our root POA
  PortableServer::POA_var root_poa_;

  /// The ORB on which we are running
  CORBA::ORB_var orb_;
};

/// Ctor
ServerServant::ServerServant (PortableServer::POA_ptr poa,
                              CORBA::ORB_ptr orb)
  :root_poa_ (PortableServer::POA::_duplicate (poa)),
   orb_ (CORBA::ORB::_duplicate (orb))
{
}

/// Servant implementations
void
ServerServant::CreateExtra (CORBA::ULong len,
                            ServerSequence_out seq)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Create extra called with "
              " length [%d]\n", len));

  ACE_NEW_THROW_EX (seq,
                    ServerSequence (len),
                    CORBA::NO_MEMORY ());

  seq->length (len);

  for (CORBA::ULong cnt = 0 ;
       cnt < len ;
       cnt ++)
    {
      ServerServant *servant = 0;

      ACE_NEW_THROW_EX (servant,
                        ServerServant (this->root_poa_.in (),
                                       this->orb_.in ()),
                        CORBA::NO_MEMORY ());

      PortableServer::ServantBase_var owner_transfer(servant);
      PortableServer::ObjectId_var id =
        this->root_poa_->activate_object (servant);
      CORBA::Object_var object = this->root_poa_->id_to_reference (id.in ());
      (*seq)[cnt] = Server::_narrow (object.in ());
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Returned from CreateExtra ()\n"));
}



void
ServerServant::DeleteExtra (const ServerSequence &seq)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Deleting %d sequences\n", seq.length ()));

  PortableServer::ObjectId_var oid;
  PortableServer::ServantBase *servant = 0;

  for (CORBA::ULong cnt = 0;
       cnt < seq.length ();
       cnt++)
    {
      oid =
        this->root_poa_->reference_to_id (seq [cnt]);
      servant =
        this->root_poa_->reference_to_servant (seq [cnt]);

      this->root_poa_->deactivate_object (oid.in ());
      servant->_remove_ref ();
      servant->_remove_ref ();
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Returned after deleting sequences\n"));
}

void
ServerServant::shutdown (void)
{
  this->orb_->shutdown (0);
}

/******************************************************/
const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])

{
  try
    {
      // Initialize the broker
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == -1)
        return -1;

      CORBA::Object_var vRootPOABase =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (vRootPOABase.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_ptr pRootPOAManager =
        root_poa->the_POAManager ();

      // Instantiate the server
      ServerServant *servant = 0;

      ACE_NEW_RETURN (servant,
                      ServerServant (root_poa.in (),
                                     orb.in ()),
                      1);

      PortableServer::ServantBase_var owner_transfer(servant);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (servant);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Server_var server =
        Server::_narrow (object.in ());

      // Announce the server
      CORBA::String_var obj_ref =
        orb->object_to_string (server.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", obj_ref.in ());
      ACE_OS::fclose (output_file);

      pRootPOAManager->activate ();

      orb->run ();

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
