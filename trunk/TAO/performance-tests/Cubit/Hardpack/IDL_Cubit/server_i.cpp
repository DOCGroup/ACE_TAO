// $Id$

#include "server_i.h"

ACE_RCSID(IDL_Cubit, server_i, "$Id$")

Cubit_Server::Cubit_Server (void)
  : num_of_objs_ (1),
    use_naming_service_ (1),
    ior_output_file_ (0)
{
}

int
Cubit_Server::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "n:o:s");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
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
                           " [-n num of cubit objects]"
                           " [-o ior_output_file]"
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
                    char** argv)
{
  int		argct;
  char		*argvt[1];

  this->argc_ = argc;
  this->argv_ = argv;

  if (this->parse_args () == -1)
    return -1;

  try {
    // Obtain an object reference to the ORB
    argvt[0]      = "-ORBase cubit_S";
    argct         = 1;
    this->orb_ = CORBA::ORB_init(argct, argvt, ORBIDENT);
    } catch (CORBA::BAD_PARAM) {
    cout << "Bad parameter exception caught - ORB_init" << endl;
    return -1;
  } catch (CORBA::INITIALIZE) {
    cout << "Initialize exception caught - ORB_init" << endl;
    return -1;
  } catch (CORBA::INTERNAL) {
    cout << "Internal exception caught - ORB_init" << endl;
    return -1;
  } catch (CORBA::UNKNOWN) {
    cout << "Unknown exception caught - ORB_init" << endl;
    return -1;
  } catch(...) {
    cout << "An exception of unknown type was caught - ORB_init" << endl;
    return -1;
  }

  
  try {
    // Obtain an object reference to the BOA
    argct = 0;
    this->boa_ = this->orb_->BOA_init(argct, argvt, BOAIDENT);
  } catch (CORBA::UNKNOWN) {
    cout << "Unknown exception caught - ORB_init" << endl;
    return -1;
  } catch (CORBA::SystemException) {
    cout << "General system exception caught - BOA_init" << endl;
    return -1;
  } catch (...) {
    cout << "An exception of unknown type was caught - BOA_init" << endl;
    return -1;
  } 

  ACE_NEW_RETURN (this->cubit_,
                  Cubit_impl,
                  -1);

  if (this->use_naming_service_)
    return this->init_naming_service ();

  return 0;
}

// Initialisation of Naming Service and register IDL_Cubit Context and
// cubit_factory object.

int
Cubit_Server::init_naming_service (void)
{
  CosNaming::Name name;
  CosNaming::NamingContext_ptr	cubit_S_context;
  CORBA::Object_ptr		dummy_object;
  try {
    // Resolve initial references to the nameserver
    dummy_object = this->orb_->resolve_initial_references("NameService");
    this->nameserver_ = CosNaming::NamingContext::_narrow(dummy_object);
  } catch (CORBA::BAD_PARAM) {
    cout << "Bad parameter exception caught - narrow on the NameService" << endl;
    return -1;
  } catch (CORBA::SystemException) {
    cout << "A general system exception was caught - resolve_initial_references" << endl;
    return -1;
  } catch (...) {
    cout << "An exception of unknown type was caught - initializing the NameService" << endl;
    return -1;    
  }
    
  try {
    // Bind the new context of the server name to the nameserver
    name.length(1);
    name[0].id   = "cubit_S";
    name[0].kind = "ASE";
    cubit_S_context = this->nameserver_ -> bind_new_context(name);
  } catch(...) {
    cout << "Naming service exception caught - bind of cubit_S context" << endl;
    return -1;
  }

  try {
    // Bind the server interface object to the nameserver, so the client
    // can resolve this server interface object
    name[0].id   = "cubit";
    name[0].kind = "interface";
    cubit_S_context -> bind(name, this->cubit_);
  } catch (...) {
    cout << "Naming service exception caught - bind of name to cubit object" << endl;
    return -1;
  }


  return 0;
}

int
Cubit_Server::run (void)
{
  
  try {
    // Process client requests forever
    while (1) 
      select_wait(-1.0);
  } catch (...) { 
    cout << "Exception of unknown type caught when servicing requests" << endl;
    return -1;
  }
  return 0;
}

Cubit_Server::~Cubit_Server (void)
{
}
