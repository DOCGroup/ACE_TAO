// $Id$

#include "airplane_server_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(ImplRepo, airplane_server_i, "$Id$")

// The server name of the Aiprlane Server
const char SERVER_NAME[] = "airplane_server";

Airplane_Server_i::Airplane_Server_i (void)
  : server_impl_ (0),
    ior_output_file_ (0),
    ir_helper_ (0),
    use_ir_ (0)
{
  // Nothing
}

int
Airplane_Server_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "do:i");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'i':  // Use the IR
        this->use_ir_ = 1;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-i]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Airplane_Server_i::init (int argc, char** argv, CORBA::Environment &ACE_TRY_ENV)
{
  // Since the Implementation Repository keys off of the POA name, we need
  // to use the SERVER_NAME as the POA's name.
  const char *poa_name = SERVER_NAME;

  ACE_TRY
    {
      // Initialize the ORB
      this->orb_ = CORBA::ORB_init (argc, argv, 0, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Save pointers to the command line arguments
      this->argc_ = argc;
      this->argv_ = argv;

      // Now check the arguments for our options
      int retval = this->parse_args ();

      if (retval != 0)
        return retval;

      // Get the POA from the ORB.
      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references ("RootPOA",
                                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Check the POA object.
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Airplane_i::init(): Unable to initialize the POA.\n"),
                          -1);

      // Narrow the object to a POA.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the POA_Manager.
      this->poa_manager_ = root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      // We now need to create a POA with the persistent and user_id policies,
      // since they are need for use with the Implementation Repository.
      
      CORBA::PolicyList policies (2);
      policies.length (2);
    
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID,
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK

      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->airplane_poa_ =
        root_poa->create_POA (poa_name,
                              this->poa_manager_.in (),
                              policies,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Creation of the new POA is over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      // Make sure the POA manager is activated.
      this->poa_manager_->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_NEW_RETURN (this->server_impl_, 
                      Airplane_i (this->use_ir_), 
                      -1);

      PortableServer::ObjectId_var server_id =
        PortableServer::string_to_ObjectId ("server");

      this->airplane_poa_->activate_object_with_id (server_id.in (),
                                                    this->server_impl_,
                                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var server_obj =
        this->airplane_poa_->id_to_reference (server_id.in (),
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;


      if (this->use_ir_ == 1)
        {
          ACE_NEW_RETURN (this->ir_helper_, IR_Helper (SERVER_NAME,
                                                       this->airplane_poa_,
                                                       this->orb_,
                                                       TAO_debug_level),
                          -1);
          this->ir_helper_->change_object (server_obj.inout (), ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      
      // Create an IOR from the server object.
      CORBA::String_var server_str =
        this->orb_->object_to_string (server_obj.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "The IOR is: <%s>\n", server_str.in ()));

      if (this->ior_output_file_)
        {
          ACE_OS::fprintf (this->ior_output_file_, "%s", server_str.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Airplane_i::init");
      ACE_RETHROW;
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Airplane_Server_i::run (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      if (this->use_ir_ == 1)
        {
          this->ir_helper_->notify_startup (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      this->orb_->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (this->use_ir_ == 1)
        {
          this->ir_helper_->notify_shutdown (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Airplane_Server_i::run");
      ACE_RETHROW;
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (-1);

  return 0;
}

Airplane_Server_i::~Airplane_Server_i (void)
{
  ACE_TRY_NEW_ENV
    {
      if (!CORBA::is_nil (this->airplane_poa_.in ()))
        {
          this->airplane_poa_->destroy (1, 1, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      // ignore exceptions
    }
  ACE_ENDTRY;

  delete this->ir_helper_;
  delete this->server_impl_;
}
