// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    Server_i.cpp
//
// = DESCRIPTION
//    Implementation of the Chat Server_i class.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#include "Server_i.h"
#include "ace/Get_Opt.h"

Server_i::Server_i ()
  : ior_file_name_ ("chat.ior")
{
  // No Op.
}

Server_i::~Server_i (void)
{
  // NO Op.
}

int
Server_i::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':  // get the file name to write to
       this->ior_file_name_ = get_opts.opt_arg ();
      break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
Server_i::init (int argc,
                char *argv[]
                TAO_ENV_ARG_DECL)
{
  // Parse the command line options.
  if (this-> parse_args(argc, argv) == -1)
    return -1;

  if (this->orb_manager_.init (argc,
                               argv
                               TAO_ENV_ARG_PARAMETER) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "orb manager init failed\n"),
                      -1);
  ACE_CHECK_RETURN (-1);

  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Activate the servant.
  CORBA::String_var str =
    this->orb_manager_.activate (&this->broadcaster_i_
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Write the IOR to a file.
  this->write_IOR (str.in ());
  return 0;
}

int
Server_i::run (TAO_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              "Running chat server...\n"));

  // Run the main event loop for the ORB.
  int ret = this->orb_manager_.run (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  if (ret == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Server_i::run"),
                      -1);
  return 0;
}

int
Server_i::write_IOR(const char* ior)
{
  FILE* ior_output_file_ =
    ACE_OS::fopen (this->ior_file_name_, "w");

  if (ior_output_file_)
    {
      ACE_OS::fprintf (ior_output_file_,
                       "%s",
                       ior);
      ACE_OS::fclose (ior_output_file_);
    }

  return 0;
}
