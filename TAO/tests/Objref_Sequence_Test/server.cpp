//$Id$
#include "TestS.h"
#include "ace/Get_Opt.h"

/// Implement the Server Interface
class ServerServant :
  public POA_Server, public PortableServer::RefCountServantBase
{
public:
  /// Ctor
  ServerServant (PortableServer::POA_ptr poa,
                 CORBA::ORB_ptr orb);

  void CreateExtra (CORBA::ULong length,
                    ServerSequence_out seq
                    TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void DeleteExtra (const ServerSequence &seq
                    TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

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
                            ServerSequence_out seq
                            TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Create extra called with "
              " length [%d] \n", len));

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

      //      PortableServer::ServantBase_var owner_transfer(servant);
      (*seq) [cnt] = servant->_this ();
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Returned from CreateExtra () \n"));
}



void
ServerServant::DeleteExtra (const ServerSequence &seq
                            TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Deleting sequences \n"));

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
              "(%P|%t) Returned after deleting sequences \n"));

}

void
ServerServant::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}

/******************************************************/
const char *ior_output_file = "test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
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
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv [])

{
  TAO_ENV_DECLARE_NEW_ENV;

  if (parse_args (argc, argv) == -1)
    return -1;

  ACE_TRY
    {
      // Initialize the broker
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var vRootPOABase =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (vRootPOABase.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_ptr pRootPOAManager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Instantiate the server
      ServerServant *servant = 0;

      ACE_NEW_RETURN (servant,
                      ServerServant (root_poa.in (),
                                     orb.in ()),
                      1);

      PortableServer::ServantBase_var owner_transfer(servant);

      Server_var server =
        servant->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Announce the server
      CORBA::String_var obj_ref =
        orb->object_to_string (server.in ()
                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", obj_ref.in ());
      ACE_OS::fclose (output_file);

      pRootPOAManager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
