// $Id$

#ifndef IMR_LOCATOR_I_H
#define IMR_LOCATOR_I_H

#include "tao/PortableServer/ImR_LocatorS.h"

#include "ace/Hash_Map_Manager.h"

#include "ace/pre.h"

typedef ACE_Hash_Map_Manager_Ex <ACE_CString,
                                 CORBA::Object_ptr,
                                 ACE_Hash<ACE_CString>,
                                 ACE_Equal_To<ACE_CString>,
                                 ACE_Null_Mutex> HASH_MAP_MANAGER;

typedef ACE_Hash_Map_Entry <ACE_CString, CORBA::Object_ptr> Table_Entry;

typedef ACE_Hash_Map_Iterator_Ex <ACE_CString,
                                  CORBA::Object_ptr,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex> Table_Iterator;

// Forward declarations.
class ImR_Adapter_Activator;
class ImR_Forwarder;


// Gets a request from a client and depending on the POA name,
// requests an activator to take care of activating the
// corresponding server and raises a forward exception to the
// client pointing to the correct server.
class ImR_Locator_i : public virtual POA_ImplementationRepository::Locator
{

 public:

  ImR_Locator_i ();

  ~ImR_Locator_i ();

  // Initialize and gets the ImR_Locator running and ready to accept
  // requests.
  int init (int argc, char *argv[] ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  // Register an activator with the locator. Raises an
  // AlreadyRegistered Exception if it is already in the
  // list.
  virtual CORBA::ULong register_activator (const char *activator,
                                           CORBA::Object_ptr obj_ref
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((ImplementationRepository::AlreadyRegistered,
                     CORBA::SystemException));

  // UnRegister an activator with the locator.
  // Raises a NotFound Exception if there is no activator at the
  // specified location.
  virtual CORBA::ULong unregister_activator (const char *activator,
                                             CORBA::Object_ptr obj_ref
                                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((ImplementationRepository::NotFound,
                     CORBA::SystemException));

  // Starts up the server <server> if not already running.
  virtual void activate_server (const char *server
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound,
                     ImplementationRepository::CannotActivate));

  // Starts up the server <server> if not already running.
  virtual void activate_server_in_location (const char *server,
                                            const char *location
                                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound,
                     ImplementationRepository::CannotActivate));

  // Adds the server to the repository and registers the startup information
  // about the server <server>.
  virtual void register_server (const char *server,
                                const ImplementationRepository::StartupOptions &options
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::AlreadyRegistered));

  // Updates the startup information about the server <server>.
  virtual void reregister_server (const char *server,
                                  const ImplementationRepository::StartupOptions &options
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Removes the server <server> from the repository.
  virtual void remove_server (const char *server
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound));

  // Removes the server <server> from the repository.
  virtual void remove_server_in_location (const char *server,
                                          const char *location
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound));

  // Attempts to gracefully shut down the server,
  virtual void shutdown_server (const char *server
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound));

  // Attempts to gracefully shut down the server <server> in location
  // <location>
  virtual void shutdown_server_in_location (const char *server,
                                            const char *location
                                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound));

  // Invoked by the server to update transient information such as current
  // location of the <server> and its ServerObject.
  virtual char *server_is_running (const char *server,
                                   const char *location,
                                   ImplementationRepository::ServerObject_ptr server_object
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound));

  // What the server should call before it shuts down.
  virtual void server_is_shutting_down (const char *server
                                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound));

  // What the server <server> should call before it shuts down from
  // location <location>
  virtual void server_is_shutting_down_in_location (const char *server,
                                                    const char *location
                                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound));

  // Returns the startup information for a server
  virtual void find (const char *server,
                     ImplementationRepository::ServerInformation_out info
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound));

  // Used to access the list of servers registered.  May also return an
  // iterator which can be used to access more than <how_many> of them.
  virtual void list (
      CORBA::ULong how_many,
      ImplementationRepository::ServerInformationList_out server_list,
      ImplementationRepository::ServerInformationIterator_out server_iterator
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Starts up the server <server> if not already running.
  char *activate_server_with_startup (const char *server,
                                      int check_startup
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound,
                     ImplementationRepository::CannotActivate));

  // Set up the multicast related if 'm' is passed on the command
  // line.
  int setup_multicast (ACE_Reactor *reactor,
                       const char *ior);

 private:

  // Parse the commandline paramters.
  int parse_args (int argc, char *argv[]);

  // As the name suggests, this methods helps choose the activator
  // based on the location passed.
  ImplementationRepository::Administration_ptr
    choose_activator_using_location (const char *location
                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((ImplementationRepository::NotFound,
                     CORBA::SystemException));

  // Helper method for choosing the activators.
  ImplementationRepository::Administration_ptr
    helper_for_choosing_activators (const char *server,
                                    const char *location
                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((ImplementationRepository::NotFound,
                     CORBA::SystemException));

  // Pointer to the default administration interface. Helps
  // unnecessary search when only one ImR_Activator is registered with
  // the ImR_Locator.
  ImplementationRepository::Administration_var default_admin_ref_;

  // check if this is the first time an activator is getting
  // registered with the ImR_Locator. To be used while setting
  // default_admin_ref_ value.
  CORBA::Boolean first_timer_;

  // Table that maintains the Location to Object Reference of the
  // Activator running in that location.
  HASH_MAP_MANAGER table_;

  // variable to check if debug information is requested.
  int debug_;

  // Variable to check if multicast is enabled.
  int multicast_;

  // IOR file
  const char *ior_output_file_;

  // The class that handles the forwarding.
  ImR_Forwarder *forwarder_impl_;

  // Used for the forwarding of any type of POA.
  ImR_Adapter_Activator *activator_;
};

#include "ace/post.h"
#endif /* IMR_LOCATOR_I_H */
