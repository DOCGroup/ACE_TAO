#include "Notifier_Server.h"

ACE_RCSID(Echo, Server_i, "$Id$")

// Constructor.

Notifier_Server::Notifier_Server (void)
{
  // no-op.
}

// Destructor.

Notifier_Server::~Notifier_Server (void)
{
  // no-op.
}

int
Notifier_Server::close (void)
{
  TAO_TRY
    {
      // Name the object.
      CosNaming::Name notifier_obj_name (1);
      notifier_obj_name.length (1);
      notifier_obj_name[0].id =
	CORBA::string_dup (NOTIFIER_BIND_NAME);

      this->naming_server_->unbind (notifier_obj_name,
				    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Instruct the ORB to shutdown.
      this->orb_manager_.orb ()->shutdown ();
    }
  TAO_CATCHANY
    {
      return -1;
    }
  TAO_ENDTRY;

    return 0;
}

// The naming service is initialized and the naming context as well as
// the object name is bound to the naming server.

int
Notifier_Server::init_naming_service (CORBA::Environment &TAO_TRY_ENV)
{
  TAO_TRY
    {
      CORBA::ORB_var orb = this->orb_manager_.orb ();

      PortableServer::POA_var child_poa
	= this->orb_manager_.child_poa ();

      TAO_debug_level = 1;

      if (this->naming_server_.init (orb.in (),
				     child_poa.in ()) == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "Failed to initialize TAO_Naming_Server\n"),
			  -1);

      // Register the object implementation with the POA.
      Notifier_var notifier_obj =
	this->servant_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Name the object.
      CosNaming::Name notifier_obj_name (1);
      notifier_obj_name.length (1);
      notifier_obj_name[0].id =
	CORBA::string_dup (NOTIFIER_BIND_NAME);
      TAO_CHECK_ENV;

      // Now, attach the object name to the context.
      this->naming_server_->bind (notifier_obj_name,
				  notifier_obj.in (),
				  TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Notifier_Server::init_naming_service\n");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}


// Initialize the server.
int
Notifier_Server::init (int argc,
		       char *argv[],
		       CORBA::Environment &TAO_TRY_ENV)
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create the child poa under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
					 argv,
					 "child_poa",
					 TAO_TRY_ENV) == -1)
   ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);
  TAO_CHECK_ENV_RETURN (TAO_TRY_ENV,
			-1);

  CORBA::ORB_var orb =
    this->orb_manager_.orb ();

  // Activate the servant in the POA.
  CORBA::String_var str =
    this->orb_manager_.activate_under_child_poa (NOTIFIER_BIND_NAME,
						 &this->servant_,
						 TAO_TRY_ENV);
  return this->init_naming_service (TAO_TRY_ENV);
}

int
Notifier_Server::run (CORBA::Environment &TAO_TRY_ENV)
{
  // Run the main event loop for the ORB.
  if (this->orb_manager_.run (TAO_TRY_ENV) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Notifier_Server::run"),
                      -1);
  return 0;
}

ACE_Reactor *
Notifier_Server::reactor (void)
{
  return TAO_ORB_Core_instance ()->reactor ();
}
