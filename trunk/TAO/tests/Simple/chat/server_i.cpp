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

#include "server_i.h"

Server_i::Server_i (char *ior_file_name)
  : ior_file_name_ (ior_file_name)
{
}

Server_i::~Server_i (void)
{
}

int
Server_i::init (int argc,
		char *argv[],
		CORBA::Environment &env)
{
  if (this->orb_manager_.init (argc,
			       argv,
			       env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "init_child_poa"),
		      -1);

  TAO_CHECK_ENV_RETURN (env, -1);

  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Activate the servant in its own child POA.

  CORBA::String_var str  =
    this->orb_manager_.activate (&(this->broadcaster_i_),
				 env);
  this->writeIOR (str.in());
  return 0;
}

int
Server_i::run (CORBA::Environment &env)
{
  // Run the main event loop for the ORB.
  if (this->orb_manager_.run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "Server_i::run"),
		      -1);
  return 0;
}

int
Server_i::writeIOR(const char* ior)
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
