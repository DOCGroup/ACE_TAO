// $Id$

#include "ImR_Locator_i.h"
#include "ImR_Activator_i.h"

#include "Forwarder.h"
#include "Adapter_Activator.h"

#include "orbsvcs/orbsvcs/IOR_Multicast.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/POA.h"

#include "tao/ORB_Core.h"
#include "tao/default_ports.h"

#include "ace/Get_Opt.h"
#include "ace/Process_Manager.h"

ImR_Locator_i::ImR_Locator_i ()
  : first_timer_ (0),
    debug_ (0),
    multicast_ (0),
    ior_output_file_ (0),
    forwarder_impl_ (0),
    activator_ (0)
{
}

ImR_Locator_i::~ImR_Locator_i (void)
{
  // Destructor
}

int
ImR_Locator_i::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opts (argc, argv, "o:md");
  int c;

    while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
	this->ior_output_file_ = get_opts.optarg;
	break;

      case 'm':
        this->multicast_ = 1;
        break;

      case 'd':
        this->debug_ = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
			   "-o <iorfile>"
                           "-m"
                           "-d"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ImR_Locator_i::init (int argc, char *argv [] ACE_ENV_ARG_DECL)
{
  // As always, initialize the ORB.
  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv,
                                        ""
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Parse the commandline parameters.
  if (this->parse_args (argc, argv) != 0)
    return -1;

  // Get a reference to the RootPOA.
  CORBA::Object_var obj =
    orb->resolve_initial_references ("RootPOA"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Narrow
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Check for nil references
  if (CORBA::is_nil (root_poa.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to obtain RootPOA reference.\n"),
                      -1);

  // Get poa_manager reference
  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Activate it.
  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // We are going to set the policies to be used with the child
  // POA.
  CORBA::PolicyList policies (2);
  policies.length (2);

  // Id Assignment policy
  policies[0] =
    root_poa->create_id_assignment_policy (PortableServer::USER_ID
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Lifespan policy
  policies[1] =
    root_poa->create_lifespan_policy (PortableServer::PERSISTENT
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Create a child POA with name ImR_Locator.
  PortableServer::POA_var locator_poa =
    root_poa->create_POA ("ImplRepoService",
                          poa_manager.in (),
                          policies
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Creation of the new POAs over, so destroy the Policy_ptr's as
  // we dont need them anymore.
  for (CORBA::ULong i = 0; i < policies.length (); ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  // Register 'this' with the ImR_Forwarder
  ACE_NEW_RETURN (this->forwarder_impl_,
                  ImR_Forwarder (this, orb.in ()),
                  -1);

  // Get the ObjectID for the string child POA name.
  PortableServer::ObjectId_var locator_id =
    PortableServer::string_to_ObjectId ("ImplRepoService");

  // Activate the object with the given id.
  locator_poa->activate_object_with_id (locator_id.in (),
                                        this
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get the locator object reference
  CORBA::Object_var locator_obj =
    locator_poa->id_to_reference (locator_id.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get the reference to IORTable.
  CORBA::Object_var table_object =
    orb->resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Narrow it down to the correct type
  IORTable::Table_var adapter =
    IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::String_var ior;

  if (CORBA::is_nil (adapter.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
    }
  else
    {
      // Get the stringified format of the locator's object
      // reference.
      ior = orb->object_to_string (locator_obj.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      // Finally, bind this ior to the child POA's name in the
      // IORTable.
      adapter->bind ("ImplRepoService",
                     ior.in ()
                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  // If the ior_output_file exists, output the ior to it
  if (this->ior_output_file_ != 0)
    {
      FILE *output_file= ACE_OS::fopen (this->ior_output_file_, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           this->ior_output_file_),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
    }

  // ImR_Adapter_Activator
  ACE_NEW_RETURN (this->activator_,
                  ImR_Adapter_Activator (this->forwarder_impl_),
                  -1);

  // Register the Adapter_Activator reference to be the RootPOA's
  // Adapter Activator.
  root_poa->the_activator (this->activator_
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get reactor instance from TAO.
  ACE_Reactor *reactor = orb->orb_core ()->reactor ();

  // = Set up the process manager

  // Init the Process Manager.
  ACE_Process_Manager process_manager;
  process_manager.open (ACE_Process_Manager::DEFAULT_SIZE, reactor);

  // Set up multicast.
  if (this->multicast_)
    {
      // Do the multicast setup
      if (this->setup_multicast (reactor, ior.in ()) != 0)
        return -1;
    }
  else
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "ImR_Activator: "
                      "Multicast Handler disabled.\n"));
        }
    }

  // Activate the POA Manager.
  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Run the ORB.
  orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

/**
 * First, figure out if we should set up a multicast handler (based on command
 * line settings) and which port to set it up (based on the environment,
 * command line, or the default IMPLREPOSERVICE port).  Then register the
 * handler with the @param reactor.
 *
 * @retval  0 ok
 * @retval -1 failed
 */
int
ImR_Locator_i::setup_multicast (ACE_Reactor *reactor,
                                const char *ior)
{
  ACE_ASSERT (reactor);
#if defined (ACE_HAS_IP_MULTICAST)

  // See if the -ORBMulticastDiscoveryEndpoint option was specified.
  ACE_CString mde (TAO_ORB_Core_instance ()->orb_params ()->mcast_discovery_endpoint ());

  // First, see if the user has given us a multicast port number
  // on the command-line;
  u_short port =
    TAO_ORB_Core_instance ()->orb_params ()->service_port (IMPLREPOSERVICE);

  if (port == 0)
    {
      // Check environment var. for multicast port.
      const char *port_number = ACE_OS::getenv ("ImplRepoServicePort");

      if (port_number != 0)
        port = ACE_OS::atoi (port_number);
    }

  // Port wasn't specified on the command-line or in environment -
  // use the default.
  if (port == 0)
    port = TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT;

  // Instantiate a handler which will handle client requests for
  // the ImplRepoService ior, received on the multicast port.
  TAO_IOR_Multicast *ior_multicast;

  ACE_NEW_RETURN (ior_multicast,
                  TAO_IOR_Multicast (),
                  -1);

  if (mde.length () != 0)
    {
      // initializes
      if (ior_multicast->init (ior,
                               mde.c_str (),
                               TAO_SERVICEID_IMPLREPOSERVICE) == -1)
        return -1;
    }
  else
    {
      if (ior_multicast->init (ior,
                               port,
                               ACE_DEFAULT_MULTICAST_ADDR,
                               TAO_SERVICEID_IMPLREPOSERVICE) == -1)
        return -1;
    }

  // Register event handler for the ior multicast.
  if (reactor->register_handler (ior_multicast,
                                 ACE_Event_Handler::READ_MASK) == -1)
    {
      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    "Implementation Repository: "
                    "cannot register Event handler\n"));
      return -1;
    }

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                "Implementation Repository: Multicast Handler enabled.\n"));

  return 0;
#else /* ACE_HAS_IP_MULTICAST */

  ACE_UNUSED_ARG (reactor);
  ACE_UNUSED_ARG (ior);
  ACE_DEBUG ((LM_WARNING,
              "Implementation Repository: "
              "ACE_HAS_IP_MULTICAST is not defined. "
              "Multicast support disabled.\n"));

  return 0;
#endif /* ACE_HAS_IP_MULTICAST */
}

CORBA::ULong
ImR_Locator_i::register_activator (const char *location,
                                   CORBA::Object_ptr object_ref
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ImplementationRepository::AlreadyRegistered,
                   CORBA::SystemException))
{
  // Insert the activator into the table_
  CORBA::ULong bind_return =
    this->table_.bind (location,
                       CORBA::Object::_duplicate (object_ref));

  if (bind_return == 1)
   {
     ACE_THROW_RETURN (ImplementationRepository::AlreadyRegistered (),
                       1);
   }
  else
    {
      // Saving these values will help avoid choosing activators
      // using server name in cases if there is only one activator.
      if (this->first_timer_ == 0)
        {
          this->first_timer_ = 1;

          this->default_admin_ref_ =
            ImplementationRepository::Administration::_narrow (
                 CORBA::Object::_duplicate (object_ref)
                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (1);
        }
      return 0;
    }
}

// Remove a activator when it no longer wants to receive requests.
CORBA::ULong
ImR_Locator_i::unregister_activator (const char *location,
                                     CORBA::Object_ptr object_ref
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ImplementationRepository::NotFound,
                   CORBA::SystemException))
{
  CORBA::ULong bind_return =
    this->table_.unbind (location,
                         object_ref);

  if (bind_return == 1)
    {
     ACE_THROW_RETURN (ImplementationRepository::NotFound (),
                       1);
    }
  else
    {
      // We duplicated the object ref while binding it. Decrement the
      // counter.
      object_ref->_remove_ref ();
    }
      return 0;
}

// Starts up the server <server> at all the locations that it is
// registered, if not already running.
void
ImR_Locator_i::activate_server (const char *server
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
       CORBA::SystemException,
       ImplementationRepository::NotFound,
       ImplementationRepository::CannotActivate))
{
  ImplementationRepository::Administration_var admin_ref = 0;

  if (this->table_.current_size () != 1)
    {
      Table_Entry *next_entry = 0;

      for (Table_Iterator iterator (this->table_);
           iterator.next (next_entry) != 0;
           iterator.advance ())
        {
          // the ImR_Activator that will do this activation.
          admin_ref =
            this->helper_for_choosing_activators (server,
                                                  next_entry->ext_id_.c_str ()
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (admin_ref.in () != 0)
            {
              // Invoke the required operation on that
              admin_ref->activate_server (server
                                          ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              return;
            }
        }

      // All the activator entries are tried but didnt find the server
      // related activator.
      ACE_ERROR ((LM_ERROR,
                  "Couldnt get the startup activator for the server\n"));

      ACE_THROW (ImplementationRepository::NotFound ());
    }
  else
    {
      // There is only one activator known to this locator.
      this->default_admin_ref_->activate_server (server
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

// Starts up the server <server> at the locations that it is
// registered, if not already running and returns the IOR of the
// activated server.
char *
ImR_Locator_i::activate_server_with_startup (const char *server,
                                             int check_startup
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound,
                   ImplementationRepository::CannotActivate))
{
  ImplementationRepository::Administration_var admin_ref = 0;

  ACE_CString ior;
  if (this->table_.current_size () != 1)
    {
      Table_Entry *next_entry = 0;

      for (Table_Iterator iterator (this->table_);
           iterator.next (next_entry) != 0;
           iterator.advance ())
        {
          // name of the activator that will do this activation.
          admin_ref =
            this->helper_for_choosing_activators (server,
                                                  next_entry->ext_id_.c_str ()
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          if (admin_ref.in () != 0)
            {
              // Invoke the required operation on that
              ior =
                admin_ref->activate_server_with_startup (server,
                                                         check_startup
                                                         ACE_ENV_ARG_PARAMETER);
              ACE_CHECK_RETURN (0);

              return CORBA::string_dup (ior.c_str ());
            }
        }

      // The control came out of the for loop and came here .. implies
      // didnt find the correct activator. Something wrong ..
      ACE_ERROR ((LM_ERROR,
                  "Couldnt get a reference to an activator at %s\n",
                  next_entry->ext_id_.c_str ()));
      ACE_THROW_RETURN (ImplementationRepository::NotFound (),
                        0);
    }
  else
    {
      // There is only one activator known to this locator.
      ior =
        this->default_admin_ref_->activate_server_with_startup (server,
                                                                check_startup
                                                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return CORBA::string_dup (ior.c_str ());
}


// Starts up the server <server> if not already running in the
// specified location.
void
ImR_Locator_i::activate_server_in_location (const char *server,
                                            const char *location
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound,
                   ImplementationRepository::CannotActivate))
{

  // name of the activator that will do this activation.
  ImplementationRepository::Administration_var admin_ref =
    this->choose_activator_using_location (location
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (admin_ref.in () != 0)
    {
      // Invoke the required operation on that reference.
      admin_ref->activate_server (server
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "Couldnt get a reference to the activator on %s\n:",
                  location));
    }
}

// Adds the server to the repository and registers the startup information
// about the server <server>.
void
ImR_Locator_i::register_server (const char *server,
                                const ImplementationRepository::StartupOptions &options
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::AlreadyRegistered))
{
  // name of the activator that will do this activation.
  ImplementationRepository::Administration_var admin_ref =
    this->choose_activator_using_location (options.location
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (admin_ref.in () != 0)
    {
      // Invoke the required operation on that reference.
      admin_ref->register_server (server,
                                  options
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "Couldnt get a reference to an activator\n"));
    }
}

// Updates the startup information about the server <server>.
void
ImR_Locator_i::reregister_server (
      const char *server,
      const ImplementationRepository::StartupOptions &options
      ACE_ENV_ARG_DECL )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
   ImplementationRepository::Administration_var admin_ref = 0;

  if (this->table_.current_size () != 1)
    {
      Table_Entry *next_entry = 0;

      for (Table_Iterator iterator (this->table_);
           iterator.next (next_entry) != 0;
           iterator.advance ())
        {
          // name of the activator that will do this activation.
          admin_ref =
            this->helper_for_choosing_activators (server,
                                                  next_entry->ext_id_.c_str ()
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (admin_ref.in () != 0)
            {
              // Invoke the required operation on that
              admin_ref->reregister_server (server,
                                            options
                                            ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
     ACE_ERROR ((LM_ERROR,
                  "Couldnt get a reference to an activator at %s\n",
                  next_entry->ext_id_.c_str ()));
    }
  else
    {
      // There is only one activator known to this locator.
      this->default_admin_ref_->reregister_server (server,
                                                   options
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

// Removes the server <server> from the repository.
void
ImR_Locator_i::remove_server (const char *server
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound))
{
  ImplementationRepository::Administration_var admin_ref = 0;

  if (this->table_.current_size () != 1)
    {
      Table_Entry *next_entry = 0;

      for (Table_Iterator iterator (this->table_);
           iterator.next (next_entry) != 0;
           iterator.advance ())
        {
          // name of the activator that will do this activation.
          admin_ref =
            this->helper_for_choosing_activators (server,
                                                  next_entry->ext_id_.c_str ()
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (admin_ref.in () != 0)
            {
              // Invoke the required operation on that
              admin_ref->remove_server (server
                                        ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
      ACE_ERROR ((LM_ERROR,
                  "Couldnt get a reference to an activator at %s\n",
                  next_entry->ext_id_.c_str ()));

      ACE_THROW (ImplementationRepository::NotFound ());
    }
  else
    {
      // There is only one activator known to this locator.
      this->default_admin_ref_->remove_server (server
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

// Removes the server <server> from the repository.
void
ImR_Locator_i::remove_server_in_location (const char *server,
                                          const char *location
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound))
{
  // name of the activator that will do this activation.
  ImplementationRepository::Administration_var admin_ref =
    this->choose_activator_using_location (location
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (admin_ref.in () != 0)
    {
      // Invoke the required operation on that reference.
      admin_ref->remove_server (server
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "Couldnt get a reference to the activator on %s\n:",
                  location));
    }
}

// Attempts to gracefully shut down the server, at all locations that
// it is running.
void
ImR_Locator_i::shutdown_server (
      const char *server
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound))
{
  ImplementationRepository::Administration_var admin_ref = 0;

  if (this->table_.current_size () != 1)
    {
      // If there are more activators registered than 1

      Table_Entry *next_entry = 0;

      for (Table_Iterator iterator (this->table_);
           iterator.next (next_entry) != 0;
           iterator.advance ())
        {
          // name of the activator that will do this activation.
          admin_ref =
            this->helper_for_choosing_activators (server,
                                                  next_entry->ext_id_.c_str ()
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (admin_ref.in () != 0)
            {
              // Invoke the required operation on that
              admin_ref->shutdown_server (server
                                          ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          "Couldnt get a reference to an activator at %s\n",
                          next_entry->ext_id_.c_str ()));

              //ACE_THROW (ImplementationRepository::NotFound ());
            }
        }
    }
  else
    {
      // There is only one activator known to this locator.
      this->default_admin_ref_->shutdown_server (server
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

// Attempts to gracefully shut down the server,
void
ImR_Locator_i::shutdown_server_in_location (
      const char *server,
      const char *location
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound))
{
  // name of the activator that will do this activation.
  ImplementationRepository::Administration_var admin_ref =
    this->choose_activator_using_location (location
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (admin_ref.in () != 0)
    {
      // Invoke the required operation on that reference.
      admin_ref->shutdown_server (server
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "Couldnt get a reference to the activator on %s\n:",
                  location));
    }
}

// Called by the server to update transient information such as current
// location of the <server> and its ServerObject.
char *
ImR_Locator_i::server_is_running (const char *server,
                                  const char *location,
                                  ImplementationRepository::ServerObject_ptr server_object
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound))
{
  // Get the hostname from the location passed.
  ACE_CString hostname = ACE_OS::strstr (location,
                                         "@");

  hostname = ACE_OS::strtok (ACE_const_cast (char *,
                                             hostname.c_str ()),
                             "@");

  hostname = ACE_OS::strtok (ACE_const_cast (char *,
                                             hostname.c_str ()),
                             ":");

  hostname = ACE_OS::strtok (ACE_const_cast (char *,
                                             hostname.c_str ()),
                             "/");

  // name of the activator that will do this activation.
  ImplementationRepository::Administration_var admin_ref =
    this->choose_activator_using_location (hostname.c_str ()
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Invoke the required operation on that reference.
  char *return_value =
    admin_ref->server_is_running (server,
                                  location,
                                  server_object
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return return_value;
}

// What the server should call before it shuts down.
void
ImR_Locator_i::server_is_shutting_down (const char *server
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound))
{
  ImplementationRepository::Administration_var admin_ref = 0;

  if (this->table_.current_size () != 1)
    {
      Table_Entry *next_entry = 0;

      for (Table_Iterator iterator (this->table_);
           iterator.next (next_entry) != 0;
           iterator.advance ())
        {
          // name of the activator that will do this activation.
          admin_ref =
            this->helper_for_choosing_activators (server,
                                                  next_entry->ext_id_.c_str ()
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (admin_ref.in () != 0)
            {
              // Invoke the required operation on that
              admin_ref->server_is_shutting_down (server
                                                  ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          "Couldnt get a reference to an activator at %s\n",
                          next_entry->ext_id_.c_str ()));

              //ACE_THROW (ImplementationRepository::NotFound ());
            }
        }
    }
  else
    {
      // There is only one activator known to this locator.
        this->default_admin_ref_->server_is_shutting_down (server
                                                 ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
    }
}

// What the server should call before it shuts down.
void
ImR_Locator_i::server_is_shutting_down_in_location (
      const char *server,
      const char *location
      ACE_ENV_ARG_DECL
    ) ACE_THROW_SPEC ((CORBA::SystemException,
                       ImplementationRepository::NotFound))
{
  // name of the activator that will do this activation.
  ImplementationRepository::Administration_var admin_ref =
    this->choose_activator_using_location (location
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (admin_ref.in () != 0)
    {
      // Invoke the required operation on that reference.
      admin_ref->server_is_shutting_down (server
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "Couldnt get a reference to the activator on %s\n:",
                  location));
    }
}

// Returns the startup information for a server
void
ImR_Locator_i::find (
      const char *server,
      ImplementationRepository::ServerInformation_out info
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound))
{
  ImplementationRepository::Administration_var admin_ref = 0;

  if (this->table_.current_size () != 1)
    {
      Table_Entry *next_entry = 0;

      for (Table_Iterator iterator (this->table_);
           iterator.next (next_entry) != 0;
           iterator.advance ())
        {
          // name of the activator that will do this activation.
          admin_ref =
            this->helper_for_choosing_activators (server,
                                                  next_entry->ext_id_.c_str ()
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (admin_ref.in () != 0)
            {
              // Invoke the required operation on that
              admin_ref->find (server,
                               info
                               ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          "Couldnt get a reference to an activator at %s\n",
                          next_entry->ext_id_.c_str ()));

              //ACE_THROW (ImplementationRepository::NotFound ());
            }
        }
    }
  else
    {
      // There is only one activator known to this locator.
      this->default_admin_ref_->find (server,
                                      info
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

// Used to access the list of servers registered.  May also return an
// iterator which can be used to access more than <how_many> of them.
void
ImR_Locator_i::list (CORBA::ULong how_many,
                     ImplementationRepository::ServerInformationList_out server_list,
                     ImplementationRepository::ServerInformationIterator_out server_iterator
                     ACE_ENV_ARG_DECL
                     ) ACE_THROW_SPEC ((CORBA::SystemException))
{
  ImplementationRepository::Administration_var admin_ref = 0;

  if (this->table_.current_size () != 1)
    {
      Table_Entry *next_entry = 0;

      for (Table_Iterator iterator (this->table_);
           iterator.next (next_entry) != 0;
           iterator.advance ())
        {
          // name of the activator that will do this activation.
          admin_ref =
            this->choose_activator_using_location (next_entry->ext_id_.c_str ()
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (admin_ref.in () != 0)
            {
              // Invoke the required operation on that reference.
              admin_ref->list (how_many,
                               server_list,
                               server_iterator
                               ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          "Couldnt get a reference to an activator at %s\n",
                          next_entry->ext_id_.c_str ()));

              //ACE_THROW (ImplementationRepository::NotFound ());
            }
        }
    }
  else
    {
      // There is only one activator known to this locator.
      this->default_admin_ref_->list (how_many,
                               server_list,
                               server_iterator
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

ImplementationRepository::Administration_ptr
ImR_Locator_i::helper_for_choosing_activators (const char *server,
                                               const char *location
                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ImplementationRepository::NotFound,
                   CORBA::SystemException))
{
  ImplementationRepository::ServerInformation_var server_info;

  ImplementationRepository::Administration_var admin_var =
    this->choose_activator_using_location (location
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (admin_var.in () != 0)
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          admin_var->find (server,
                           server_info
                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (ImplementationRepository::NotFound, ex)
        {
          // An exception is thrown. Implies the server is not
          // registered with this activator. Return back.
          return 0;
        }
      ACE_ENDTRY;

      if (ACE_OS::strcmp (server_info->server, server) == 0)
        {
          // Double check: Found a match
          return ImplementationRepository::Administration::_duplicate (admin_var.in ());
        }
    }

  // If it comes here, means it couldnt find one.
  return 0;
}

ImplementationRepository::Administration_ptr
ImR_Locator_i::choose_activator_using_location (const char *location
                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ImplementationRepository::NotFound,
                   CORBA::SystemException))
{
  CORBA::Object_var object_ref = 0;

  if (this->table_.find (location,
                         object_ref) == -1)
    {
      ACE_THROW_RETURN (ImplementationRepository::NotFound (),
                        0);
    }

  ImplementationRepository::Administration_var admin_var =
    ImplementationRepository::Administration::_narrow (CORBA::Object::_duplicate (object_ref.in ())
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (admin_var.in () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "admin var is nil\n"));
    }

  return ImplementationRepository::Administration::_duplicate (admin_var.in ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager<ACE_CString, CORBA::Object_ptr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, CORBA::Object_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString, CORBA::Object_ptr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, CORBA::Object_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_CString, CORBA::Object_ptr>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, CORBA::Object_ptr, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, CORBA::Object_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, CORBA::Object_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,CORBA::Object_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, CORBA::Object_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,CORBA::Object_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, CORBA::Object_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, CORBA::Object_ptr>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, CORBA::Object_ptr, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, CORBA::Object_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, CORBA::Object_ptr, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
