// $Id$

#include "Notifier_Server.h"
#include "Event_CommC.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"

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
  ACE_TRY_NEW_ENV
    {
      // disconnect all the consumers.
      this->servant_.disconnect ("notifier shutdown."
                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Name the object.
      CosNaming::Name notifier_obj_name (1);
      notifier_obj_name.length (1);
      notifier_obj_name[0].id =
        CORBA::string_dup (NOTIFIER_BIND_NAME);

      this->naming_server_->unbind (notifier_obj_name
                                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Instruct the ORB to shutdown.
      this->orb_manager_.orb ()->shutdown ();
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

    return 0;
}

// The naming service is initialized and the naming context as well as
// the object name is bound to the naming server.

int
Notifier_Server::init_naming_service (TAO_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
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
      Event_Comm::Notifier_var notifier_obj =
        this->servant_._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Name the object.
      CosNaming::Name notifier_obj_name (1);
      notifier_obj_name.length (1);
      notifier_obj_name[0].id =
        CORBA::string_dup (NOTIFIER_BIND_NAME);
      ACE_TRY_CHECK;

      // Now, attach the object name to the context.
      this->naming_server_->bind (notifier_obj_name,
                                  notifier_obj.in ()
                                  TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Notifier_Server::init_naming_service\n");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}


// Initialize the server.
int
Notifier_Server::init (int argc,
                       char *argv[]
                       TAO_ENV_ARG_DECL)
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create the child poa under the root POA.

 if (this->orb_manager_.init_child_poa (argc,
                                        argv,
                                        "child_poa"
                                        TAO_ENV_ARG_PARAMETER) == -1)

   ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                      "init_child_poa"),
                      -1);
 ACE_CHECK_RETURN (-1);

 this->orb_manager_.activate_poa_manager (TAO_ENV_SINGLE_ARG_PARAMETER);
 ACE_CHECK_RETURN (-1);

 // Activate the servant in the POA.
 CORBA::String_var str =
   this->orb_manager_.activate_under_child_poa (NOTIFIER_BIND_NAME,
                                                 &this->servant_
                                                TAO_ENV_ARG_PARAMETER);

 return this->init_naming_service (TAO_ENV_SINGLE_ARG_PARAMETER);
}

int
Notifier_Server::run (TAO_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              "Running the notifier server...\n"));

  // Run the main event loop for the ORB.
  this->orb_manager_.run (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

ACE_Reactor *
Notifier_Server::reactor (void)
{
  return TAO_ORB_Core_instance ()->reactor ();
}
