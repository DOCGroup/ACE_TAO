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

#include "ace/Argv_Type_Converter.h"
#include "tao/debug.h"

ACE_RCSID(Concurrency_Service, Concurrency_Service, "$Id$")

// Default Constructor.

Concurrency_Service::Concurrency_Service (void)
  : use_naming_service_ (1),
    ior_output_file_ (0)
{
  ACE_DEBUG ((LM_DEBUG,
             ACE_LIB_TEXT("Concurrency_Service::Concurrency_Service (void)\n")));
}

// Constructor taking command-line arguments.

Concurrency_Service::Concurrency_Service (int argc,
                                          ACE_TCHAR** argv
                                          ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT("Concurrency_Service::Concurrency_Service (...)\n")));
  this->init (argc, argv ACE_ENV_ARG_PARAMETER);
}

int
Concurrency_Service::parse_args (int argc, ACE_TCHAR** argv)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT("Concurrency_Service::parse_args\n")));

  ACE_Get_Opt get_opts (argc, argv, ACE_LIB_TEXT("do:s"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd': // debug flag
        TAO_debug_level++;
        break;
      case 'o': // output the IOR to a file
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, ACE_LIB_TEXT("w"));
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT("Unable to open %s for writing: %p\n"),
                             get_opts.opt_arg ()), -1);
        break;
      case 's':
        this->use_naming_service_ = 0;
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT("usage:  %s")
                           ACE_LIB_TEXT(" [-d]")
                           ACE_LIB_TEXT(" [-o] <ior_output_file>")
                           ACE_LIB_TEXT("\n"),
                           argv[0]),
                           1);
      }
  // Indicates sucessfull persing of command line.
  return 0;
}

// Initialize the state of the Concurrency_Service object.

int
Concurrency_Service::init (int argc,
                           ACE_TCHAR **argv
                           ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              "Concurrency_Service::init\n"));

  // Copy command line parameter.
  ACE_Argv_Type_Converter command_line(argc, argv);

  if (this->orb_manager_.init_child_poa (command_line.get_argc(),
                                         command_line.get_ASCII_argv(),
                                         "child_poa"
                                          ACE_ENV_ARG_PARAMETER) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT("%p\n"),
                       ACE_LIB_TEXT("init_child_poa")),
                      -1);
  ACE_CHECK_RETURN (-1);

  if (this->parse_args (command_line.get_argc(), command_line.get_TCHAR_argv())!=0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT("Could not parse command line\n")),
                     -1);
  CORBA::String_var str =
    this->orb_manager_.activate (this->my_concurrency_server_.GetLockSetFactory ()
                                ACE_ENV_ARG_PARAMETER);
  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              ACE_TEXT_CHAR_TO_TCHAR(str.in ())));

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_, "%s", str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  if (this->use_naming_service_)
    return this->init_naming_service (ACE_ENV_SINGLE_ARG_PARAMETER);

  return 0;
}

int
Concurrency_Service::init_naming_service (ACE_ENV_SINGLE_ARG_DECL)
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
    this->my_concurrency_server_.GetLockSetFactory ()->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CosNaming::Name concurrency_context_name (1);
  concurrency_context_name.length (1);
  concurrency_context_name[0].id = CORBA::string_dup ("CosConcurrency");

  this->concurrency_context_ =
    this->my_name_server_->bind_new_context (concurrency_context_name
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CosNaming::Name lockset_name (1);
  lockset_name.length (1);
  lockset_name[0].id = CORBA::string_dup ("LockSetFactory");
  this->concurrency_context_->bind (lockset_name,
                                   lockset_factory_.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  return 0;
}

// Run the ORB event loop.

int
Concurrency_Service::run (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              "Concurrency_Service::run (...)\n"));

  int retval = this->orb_manager_.run (ACE_ENV_SINGLE_ARG_PARAMETER);
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
ACE_TMAIN (int argc, ACE_TCHAR ** argv)
{
  Concurrency_Service concurrency_service;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t Concurrency Service:SERVER \n \n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int r = concurrency_service.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (r == -1)
        return 1;

      concurrency_service.run (ACE_ENV_SINGLE_ARG_PARAMETER);
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
