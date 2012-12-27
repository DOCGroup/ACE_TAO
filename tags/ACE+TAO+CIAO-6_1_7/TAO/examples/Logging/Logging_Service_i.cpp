// $Id$

#include "Logging_Service_i.h"
#include "tao/debug.h"

Logger_Server::Logger_Server (void)
  :service_name_ (ACE_TEXT("LoggingService"))
{
  // Do nothing
}

int
Logger_Server::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("dn:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'n':  // Set factory name to cmnd line arg
        service_name_ = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n service-name]"
                           "\n",
                           argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Logger_Server::init (int argc,
                     ACE_TCHAR *argv[])
{
  this->argc_ = argc;
  this->argv_ = argv;

  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create a child POA under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa") == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);


  this->orb_manager_.activate_poa_manager ();

  // Parse the command line arguments.
  if (this->parse_args () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "parse_args"),
                      -1);

  // Activate the logger_factory.
  CORBA::String_var str =
    this->orb_manager_.activate_under_child_poa ("logger_factory",
                                                 &this->factory_impl_);
    if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "The IOR is: <%s>\n",
                str.in ()));

    // Initialize the naming service
    int ret = this->init_naming_service ();
    if (ret != 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_naming_service"),
                      -1);
  else
    return 0;
}


// Initialisation of Naming Service and register IDL_Logger Context
// and logger_factory object.

int
Logger_Server::init_naming_service ()
{
  // Get pointers to the ORB and child POA
  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Initialize the naming service
  if (this->my_name_server_.init (orb.in ()) == -1)
    return -1;

  // Create an instance of the Logger_Factory
  Logger_Factory_var factory = this->factory_impl_._this ();

  //Register the logger_factory
  CosNaming::Name factory_name (1);
  factory_name.length (1);
  factory_name[0].id = CORBA::string_dup ("Logger_Factory");
  this->my_name_server_->bind (factory_name,
                               factory.in ());

  return 0;
}

int
Logger_Server::run (void)
{
  int ret = this->orb_manager_.run ();
  if (ret == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Logger_Server::run"),
                      -1);
  return 0;
}

Logger_Server::~Logger_Server (void)
{
  // Do nothing
}
