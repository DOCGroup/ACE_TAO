// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs/Concurrecy_Service/Concurrency_Service
//
// = FILENAME
//    Concurrency_Service.cpp
//
// = DESCRIPTION
//      This class implements a Concurrency_Service object.
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "Concurrency_Service.h"

#include "tao/debug.h"

ACE_RCSID(Concurrency_Service, Concurrency_Service, "$Id$")

// Default Constructor.

Concurrency_Service::Concurrency_Service (void)
  : use_naming_service_ (1),
    ior_output_file_ (0)
{
  ACE_DEBUG ((LM_DEBUG,
             "Concurrency_Service::Concurrency_Service (void)\n"));
}

// Constructor taking command-line arguments.

Concurrency_Service::Concurrency_Service (int argc,
                                          char** argv
                                          TAO_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              "Concurrency_Service::Concurrency_Service (...)\n"));
  this->init (argc, argv TAO_ENV_ARG_PARAMETER);
}

int
Concurrency_Service::parse_args (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Concurrency_Service::parse_args\n"));

  ACE_Get_Opt get_opts (argc_, argv_, "do:s");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd': // debug flag
        TAO_debug_level++;
        break;
      case 'o': // output the IOR to a file
        this->ior_output_file_ = ACE_OS::fopen (get_opts.opt_arg (), "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;
      case 's':
        this->use_naming_service_ = 0;
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv_[0]),
                           1);
      }
  // Indicates sucessfull persing of command line.
  return 0;
}

// Initialize the state of the Concurrency_Service object.

int
Concurrency_Service::init (int argc,
                           char **argv
                           TAO_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              "Concurrency_Service::init\n"));
  if (this->orb_manager_.init_child_poa (argc,
                                        argv,
                                        "child_poa"
                                        TAO_ENV_ARG_PARAMETER) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);
  ACE_CHECK_RETURN (-1);

  this->argc_ = argc;
  this->argv_ = argv;

  if (this->parse_args ()!=0)
    ACE_ERROR_RETURN ((LM_ERROR,
                      "Could not parse command line\n"),
                     -1);
  CORBA::String_var str =
    this->orb_manager_.activate (this->my_concurrency_server_.GetLockSetFactory ()
                                TAO_ENV_ARG_PARAMETER);
  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_, "%s", str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  if (this->use_naming_service_)
    return this->init_naming_service (TAO_ENV_SINGLE_ARG_PARAMETER);

  return 0;
}

int
Concurrency_Service::init_naming_service (TAO_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "Concurrency_Service::init_naming_service (...)\n"));
  CORBA::ORB_var orb;
  PortableServer::POA_var child_poa;

  orb = this->orb_manager_.orb ();
  child_poa = this->orb_manager_.child_poa ();

  int result = this->my_name_server_.init (orb.in (),
                                           child_poa.in ());
  if (result == -1)
    return result;
  lockset_factory_ =
    this->my_concurrency_server_.GetLockSetFactory ()->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CosNaming::Name concurrency_context_name (1);
  concurrency_context_name.length (1);
  concurrency_context_name[0].id = CORBA::string_dup ("CosConcurrency");

  this->concurrency_context_ =
    this->my_name_server_->bind_new_context (concurrency_context_name
                                             TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CosNaming::Name lockset_name (1);
  lockset_name.length (1);
  lockset_name[0].id = CORBA::string_dup ("LockSetFactory");
  this->concurrency_context_->bind (lockset_name,
                                   lockset_factory_.in ()
                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  return 0;
}

// Run the ORB event loop.

int
Concurrency_Service::run (TAO_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              "Concurrency_Service::run (...)\n"));

  int retval = this->orb_manager_.run (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  if (retval == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                      "Concurrency_Service::run"),
                     -1);
  return 0;
}

// Destructor.

Concurrency_Service::~Concurrency_Service (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Concurrency_Service::~Concurrency_Service (void)\n"));
}

int
main (int argc, char ** argv)
{
  Concurrency_Service concurrency_service;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t Concurrency Service:SERVER \n \n"));

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      int r = concurrency_service.init (argc, argv TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (r == -1)
        return 1;

      concurrency_service.run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }

  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return -1;
    }

  ACE_ENDTRY;
  return 0;
}
