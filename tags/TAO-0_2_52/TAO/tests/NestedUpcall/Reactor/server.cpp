// $Id$

#include "server.h"

ACE_RCSID(Reactor, server, "$Id$")

NestedUpCalls_Server::NestedUpCalls_Server (void)
  : use_naming_service_ (1),
    ior_output_file_ (0)
{
}

int
NestedUpCalls_Server::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn:o:s");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
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
NestedUpCalls_Server::init (int argc,
                    char** argv,
                    CORBA::Environment& env)
{
  // Call the init of TAO_ORB_Manager to create a child POA
  // under the root POA.
  this->orb_manager_.init_child_poa (argc,
                                     argv,
                                     "child_poa",
                                     env);

  TAO_CHECK_ENV_RETURN (env,-1);
  this->argc_ = argc;
  this->argv_ = argv;

  this->parse_args ();
  // ~~ check for the return value here

  CORBA::String_var str  =
    this->orb_manager_.activate_under_child_poa ("reactor",
                                                 &this->reactor_impl_,
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
NestedUpCalls_Server::init_naming_service (CORBA::Environment& env)
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
  reactor_ = this->reactor_impl_._this (env);
  TAO_CHECK_ENV_RETURN (env,-1);

  //Register the nested_up_calls_reactor name with the NestedUpCalls Naming
  //Context...
  CosNaming::Name nested_up_calls_context_name (1);
  nested_up_calls_context_name.length (1);
  nested_up_calls_context_name[0].id = CORBA::string_dup ("NestedUpCalls");
  this->naming_context_ =
    this->my_name_server_->bind_new_context (nested_up_calls_context_name,
                                             env);
  TAO_CHECK_ENV_RETURN (env,-1);

  CosNaming::Name reactor_name (1);
  reactor_name.length (1);
  reactor_name[0].id = CORBA::string_dup ("nested_up_calls_reactor");
  this->naming_context_->bind (reactor_name,
                                                           reactor_.in (),
                                           env);
  TAO_CHECK_ENV_RETURN (env,-1);
  return 0;
}

int
NestedUpCalls_Server::run (CORBA::Environment& env)
{
  if (this->orb_manager_.run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "NestedUpCalls_Server::run"),
                      -1);
  return 0;
}

NestedUpCalls_Server::~NestedUpCalls_Server (void)
{
}

int
main (int argc, char *argv[])
{
  NestedUpCalls_Server nested_up_calls_server;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t NestedUpCalls:SERVER \n \n"));
  TAO_TRY
    {
      if (nested_up_calls_server.init (argc,argv,TAO_TRY_ENV) == -1)
        return 1;
      else
        {
          nested_up_calls_server.run (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}
