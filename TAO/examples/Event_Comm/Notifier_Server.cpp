// $Id$

#include "Notifier_Server.h"
#include "Event_CommC.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"

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
  try
    {
      // disconnect all the consumers.
      this->servant_.disconnect ("notifier shutdown.");

      // Name the object.
      CosNaming::Name notifier_obj_name (1);
      notifier_obj_name.length (1);
      notifier_obj_name[0].id =
        CORBA::string_dup (NOTIFIER_BIND_NAME);

      this->naming_server_->unbind (notifier_obj_name);

      // Instruct the ORB to shutdown.
      this->orb_manager_.orb ()->shutdown ();
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }

    return 0;
}

// The naming service is initialized and the naming context as well as
// the object name is bound to the naming server.

int
Notifier_Server::init_naming_service (void)
{
  try
    {
      CORBA::ORB_var orb = this->orb_manager_.orb ();

      TAO_debug_level = 1;

      if (this->naming_server_.init (orb.in ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failed to initialize TAO_Naming_Server\n"),
                          -1);

      // Register the object implementation with the POA.
      Event_Comm::Notifier_var notifier_obj =
        this->servant_._this ();

      // Name the object.
      CosNaming::Name notifier_obj_name (1);
      notifier_obj_name.length (1);
      notifier_obj_name[0].id =
        CORBA::string_dup (NOTIFIER_BIND_NAME);

      // Now, attach the object name to the context.
      this->naming_server_->bind (notifier_obj_name,
                                  notifier_obj.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Notifier_Server::init_naming_service\n");
      return -1;
    }

  return 0;
}


// Initialize the server.
int
Notifier_Server::init (int argc,
                       ACE_TCHAR *argv[])
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create the child poa under the root POA.

 if (this->orb_manager_.init_child_poa (argc,
                                        argv,
                                        "child_poa") == -1)

   ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                      "init_child_poa"),
                      -1);

 this->orb_manager_.activate_poa_manager ();

 // Activate the servant in the POA.
 CORBA::String_var str =
   this->orb_manager_.activate_under_child_poa (NOTIFIER_BIND_NAME,
                                                 &this->servant_);

 return this->init_naming_service ();
}

int
Notifier_Server::run (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Running the notifier server...\n"));

  // Run the main event loop for the ORB.
  this->orb_manager_.run ();

  return 0;
}

ACE_Reactor *
Notifier_Server::reactor (void)
{
  return TAO_ORB_Core_instance ()->reactor ();
}
