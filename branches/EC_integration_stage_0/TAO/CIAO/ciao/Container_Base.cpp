// $Id$

#include "Container_Base.h"
#include "Cookies.h"
#include "ace/DLL.h"

#if !defined (__ACE_INLINE__)
# include "Container_Base.inl"
#endif /* __ACE_INLINE__ */

////////////////////////////////////////////////////////////////

CIAO::Container::Container (CORBA::ORB_ptr o) :
  orb_ (CORBA::ORB::_duplicate (o)),
  events_manager_ (o)
{
}

CIAO::Container::~Container ()
{
}

PortableServer::POA_ptr
CIAO::Container::_ciao_the_POA (void)
{
  return this->poa_.in ();
}

CORBA::ORB_ptr
CIAO::Container::_ciao_the_ORB ()
{
  return this->orb_.in ();
}

/// Events_Manager creates the appropriate servant for Consumer_Config.
CIAO_Events::Consumer_Config_ptr CIAO::Container::_ciao_create_event_consumer_config (
        const char * service_type
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException))
{
  // @@ George Why did the exception specification dropped in these places. ?
  return this->events_manager_.create_consumer_config (service_type);
}

/// Events_Manager creates the appropriate servant for Supplier_Config
// @@ George, Should these pointers be exposed to the glue code at all? I
//don't see a reason why it should be done.
CIAO_Events::Supplier_Config_ptr
CIAO::Container::_ciao_create_event_supplier_config (
        const char * service_type
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException))
{
  return this->events_manager_.create_supplier_config (service_type);
}


/// Connect up an event sink.
void CIAO::Container::_ciao_connect_event_consumer (
    CIAO_Events::Consumer_Config_ptr consumer_config
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::_ciao_connect_event_consumer\n"));

  // Look up the supplier's event service implementation.
  CIAO_Events::EventServiceBase * event_service = 0;
  if (this->event_service_map_.find (consumer_config->get_supplier_id (), event_service) != 0)
    {
      ACE_THROW (
      ::Components::InvalidConnection ());
    }

  /// Connect to the supplier's event service implementation
  CIAO_Events::EventServiceInfo service_info =
    event_service->connect_event_consumer (consumer_config ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  /// Save the consumer's disconnect info in a map.
  this->event_info_map_.bind (consumer_config->get_consumer_id (),
                              service_info);
}

/// Connect up an event source.
void CIAO::Container::_ciao_connect_event_supplier (
    CIAO_Events::Supplier_Config_ptr supplier_config
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::_ciao_connect_event_supplier\n"));

  /// Use Events_Manager factory method to create the appropriate implementation of
  /// EventServiceBase
  CIAO_Events::EventServiceBase * event_service =
    this->events_manager_.create_supplier (supplier_config);

  /// Connect the supplier
  event_service->connect_event_supplier (supplier_config ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  /// Save a pointer to the event service implementation in a map.
  this->event_service_map_.bind (supplier_config->get_supplier_id (),
                                 event_service);
}

void CIAO::Container::_ciao_disconnect_event_consumer
      (CIAO_Events::CONNECTION_ID connection_id
       ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::InvalidName,
                       ::Components::InvalidConnection))
{
  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::_ciao_disconnect_event_consumer\n"));

  CIAO_Events::EventServiceInfo service_info;

  if (this->event_info_map_.unbind (connection_id, service_info) != 0)
    {
      ACE_THROW (
      ::Components::InvalidConnection ());
    }

  service_info.service->disconnect_event_consumer (service_info ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

void CIAO::Container::_ciao_disconnect_event_supplier
      (CIAO_Events::CONNECTION_ID connection_id
       ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::InvalidName,
                       ::Components::InvalidConnection))
{
  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::_ciao_disconnect_event_supplier\n"));

  CIAO_Events::EventServiceBase * event_service;

  if (this->event_service_map_.unbind (connection_id, event_service) != 0)
    {
      ACE_THROW (
      ::Components::InvalidConnection ());
    }

  event_service->disconnect_event_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void CIAO::Container::_ciao_push_event (Components::EventBase *ev,
                                        CIAO_Events::CONNECTION_ID connection_id
                                        ACE_ENV_ARG_DECL)
                                      ACE_THROW_SPEC ((
                                        CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::_ciao_push_event\n"));

  /// Pushing an event requires a map lookup.
  CIAO_Events::EventServiceBase * event_service;
  if (this->event_service_map_.find (connection_id, event_service) != 0)
    {
      ACE_THROW (
      ::Components::InvalidConnection ());
    }

  event_service->push_event (ev ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

///////////////////////////////////////////////////////////////

ACE_Atomic_Op <ACE_Thread_Mutex, long>
CIAO::Session_Container::serial_number_ (0);

CIAO::Session_Container::Session_Container (CORBA::ORB_ptr o)
  : Container (o),
    number_ (0)
{
}

CIAO::Session_Container::~Session_Container ()
{
}

int
CIAO::Session_Container::init (const char *name,
                               const CORBA::PolicyList *more_policies
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO::Session_Container::init [%P|%t]\n"));

  char buffer[MAXPATHLEN];

   if (name == 0)
     {
       this->number_ = ++CIAO::Session_Container::serial_number_;
       ACE_OS::sprintf (buffer, "CIAO::Session_Container-%ld",
                        this->number_);
       name = buffer;
     }

   CORBA::Object_var poa_object =
     this->orb_->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   if (CORBA::is_nil (poa_object.in ()))
     ACE_ERROR_RETURN ((LM_ERROR,
                        " (%P|%t) Unable to initialize the POA.\n"),
                       -1);

   PortableServer::POA_var root_poa =
     PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   // Set up proper poa policies here.  Default policies seems to be
   // fine for session container.  If you add some other default
   // policies here, then you need to "add" more_policies below
   // instead of simply assigning more_policies to the init policy
   // list.
   CORBA::PolicyList policies (0);

   if (more_policies != 0)
     policies = *more_policies;

   this->poa_ = root_poa->create_POA (name,
                                      PortableServer::POAManager::_nil (),
                                      policies
                                      ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   PortableServer::POAManager_var poa_manager =
     this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   return 0;
}

CORBA::Object_ptr
CIAO::Session_Container::install_servant (PortableServer::Servant p
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid
    = this->poa_->activate_object (p
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var objref
    = this->poa_->id_to_reference (oid.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return objref._retn ();
}

CORBA::Object_ptr
CIAO::Session_Container::install_component (PortableServer::Servant p,
                                            PortableServer::ObjectId_out oid
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var id
    = this->poa_->activate_object (p
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var objref
    = this->poa_->id_to_reference (id.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  oid = id._retn ();

  return objref._retn ();
}

Components::CCMHome_ptr
CIAO::Session_Container::ciao_install_home (const char *exe_dll_name,
                                            const char *exe_entrypt,
                                            const char *sv_dll_name,
                                            const char *sv_entrypt
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::UnknownImplId,
                   Components::Deployment::ImplEntryPointNotFound,
                   Components::Deployment::InstallationFailure))
{
  ACE_DLL executor_dll, servant_dll;

  if (exe_dll_name == 0 || sv_dll_name == 0)
    ACE_THROW_RETURN (Components::Deployment::UnknownImplId (), 0);

  if (executor_dll.open (exe_dll_name,
                         ACE_DEFAULT_SHLIB_MODE,
                         0) != 0)
    ACE_THROW_RETURN (Components::Deployment::UnknownImplId (), 0);

  if (servant_dll.open (sv_dll_name,
                        ACE_DEFAULT_SHLIB_MODE,
                        0) != 0)
    {
      executor_dll.close ();
      ACE_THROW_RETURN (Components::Deployment::UnknownImplId (), 0);
    }

  if (exe_entrypt == 0 || sv_entrypt == 0)
    ACE_THROW_RETURN (Components::Deployment::ImplEntryPointNotFound (), 0);

  HomeFactory hcreator = (HomeFactory) executor_dll.symbol (exe_entrypt);
  ServantFactory screator = (ServantFactory) servant_dll.symbol (sv_entrypt);

  if (hcreator == 0 || screator == 0)
    ACE_THROW_RETURN (Components::Deployment::ImplEntryPointNotFound (), 0);

  Components::HomeExecutorBase_var home_executor = hcreator ();
  if (CORBA::is_nil (home_executor.in ()))
    ACE_THROW_RETURN (Components::Deployment::InstallationFailure (), 0);

  PortableServer::Servant home_servant = screator (home_executor.in (),
                                                   this
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (home_servant == 0)
    ACE_THROW_RETURN (Components::Deployment::InstallationFailure (), 0);

  PortableServer::ServantBase_var safe (home_servant);

  CORBA::Object_var objref = this->install_servant (home_servant
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  Components::CCMHome_var homeref = Components::CCMHome::_narrow (objref.in ()
                                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return homeref._retn ();

}

void
CIAO::Session_Container::ciao_uninstall_home (Components::CCMHome_ptr homeref
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->uninstall (homeref
                   ACE_ENV_ARG_PARAMETER);
}

void
CIAO::Session_Container::uninstall (CORBA::Object_ptr objref
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid
    = this->poa_->reference_to_id (objref
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_->deactivate_object (oid.in ()
                                 ACE_ENV_ARG_PARAMETER);
}

void
CIAO::Session_Container::uninstall (PortableServer::Servant svt
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid
    = this->poa_->servant_to_id (svt
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_->deactivate_object (oid.in ()
                                 ACE_ENV_ARG_PARAMETER);
}

void
CIAO::Session_Container::uninstall_component (CORBA::Object_ptr objref,
                                              PortableServer::ObjectId_out oid
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var id
    = this->poa_->reference_to_id (objref
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_->deactivate_object (id.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  oid = id._retn ();
}
