// $Id$

#include "server_i.h"

Cubit_Server::Cubit_Server (void)
  : num_of_objs_ (1),
    use_naming_service_ (1),
    ior_output_file_ (0)
{
}

int
Cubit_Server::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn:o:s");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'n': // number of cubit objects we export
        this->num_of_objs_ = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'o': // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
      case 's': // Don't use the TAO Naming Service.
        this->use_naming_service_=0;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n] <num of cubit objects>"
                           " [-o] <ior_output_file>"
                           " [-s]"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Cubit_Server::init (int argc,
                    char** argv,
                    CORBA::Environment& env)
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create a child POA under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa",
                                         env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);

  TAO_CHECK_ENV_RETURN (env,-1);
  this->argc_ = argc;
  this->argv_ = argv;

  this->parse_args ();
  // @@ Check for the return value here.

  CORBA::String_var str  =
    this->orb_manager_.activate_under_child_poa ("factory",
                                                 &this->factory_impl_,
                                                 env);
  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  if (this->use_naming_service_)
    return this->init_naming_service (env);

  return 0;
}

// Initialisation of Naming Service and register IDL_Cubit Context and
// cubit_factory object.

int
Cubit_Server::init_naming_service (CORBA::Environment& env)
{
  int result;
  CORBA::ORB_var orb;
  PortableServer::POA_var child_poa;

  orb = this->orb_manager_.orb ();
  child_poa = this->orb_manager_.child_poa ();

  result = this->my_name_server_.init (orb.in (),
                                       child_poa.in ());
  if (result < 0)
    return result;
  factory = this->factory_impl_._this (env);
  TAO_CHECK_ENV_RETURN (env,-1);

  CosNaming::Name cubit_context_name (1);
  cubit_context_name.length (1);
  cubit_context_name[0].id = CORBA::string_dup ("IDL_Cubit");
  this->cubit_context_ =
    this->my_name_server_->bind_new_context (cubit_context_name,
                                             env);
  TAO_CHECK_ENV_RETURN (env,-1);

  //Register the cubit_factory name with the IDL_Cubit Naming
  //Context...
  CosNaming::Name factory_name (1);
  factory_name.length (1);
  factory_name[0].id = CORBA::string_dup ("cubit_factory");
  this->cubit_context_->bind (factory_name,
                              factory.in (),
                              env);
  TAO_CHECK_ENV_RETURN (env,-1);

  shutdown_ = this->shutdown_impl_._this (env);
  TAO_CHECK_ENV_RETURN (env,-1);

  CosNaming::Name shutdown_name (1);
  shutdown_name.length (1);
  shutdown_name[0].id = CORBA::string_dup ("shutdown");
  this->cubit_context_->bind (shutdown_name,
                              shutdown_.in (),
                              env);
  TAO_CHECK_ENV_RETURN (env,-1);
  return 0;
}

int
Cubit_Server::run (CORBA::Environment& env)
{
  if (this->orb_manager_.run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cubit_Server::run"),
                      -1);
  return 0;
}

Cubit_Server::~Cubit_Server (void)
{
}
