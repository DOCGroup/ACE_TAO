// $Id$

#include "Container_Base.h"
#include "ace/DLL.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "ace/OS_NS_stdio.h"
#include "Servant_Activator.h"

#include "ace/Dynamic_Service.h"

#if !defined (__ACE_INLINE__)
# include "Container_Base.inl"
#endif /* __ACE_INLINE__ */

////////////////////////////////////////////////////////////////

CIAO::Container::Container (CORBA::ORB_ptr o)
  : orb_ (CORBA::ORB::_duplicate (o))
{
}

CIAO::Container::~Container ()
{
}

PortableServer::POA_ptr
CIAO::Container::the_POA (void) const
{
  return this->component_poa_.in ();
}

CORBA::ORB_ptr
CIAO::Container::the_ORB (void) const
{
  return this->orb_.in ();
}

// Creates the appropriate servant for Consumer_Config object.
CIAO::Consumer_Config_ptr
CIAO::Container::create_consumer_config (
    CIAO::EventServiceType type
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
  CIAO::Consumer_Config_ptr consumer_config =
    this->event_service_factory_->create_consumer_config (type
                                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  return consumer_config;
}

// Creates the appropriate servant for Supplier_Config object.
CIAO::Supplier_Config_ptr
CIAO::Container::create_supplier_config (
    CIAO::EventServiceType type
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
  CIAO::Supplier_Config_ptr supplier_config =
    this->event_service_factory_->create_supplier_config (type
                                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  return supplier_config;
}


// Connect up an event sink.
void
CIAO::Container::connect_event_consumer (
    CIAO::Consumer_Config_ptr consumer_config
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    Components::InvalidConnection,
    CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::connect_event_consumer\n"));

  // Look up the supplier's event service implementation.
  CIAO::EventServiceBase * event_service = 0;

  ACE_CString supplier_id =
    consumer_config->supplier_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->event_service_map_.find (supplier_id.c_str (), event_service) != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "-------------Oops!!! -----------------\n"));
      ACE_THROW (Components::InvalidConnection ());
    }

  // Connect to the supplier's event service implementation
  event_service->connect_event_consumer (consumer_config ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Save a pointer to the event service implementation in a map.
  ACE_CString consumer_id =
    consumer_config->consumer_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->event_service_map_.bind (consumer_id.c_str (),
                                 event_service);

}

// Connect up an event source.
void
CIAO::Container::connect_event_supplier (
    CIAO::Supplier_Config_ptr supplier_config
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::connect_event_supplier\n"));

  // Use factory to create the appropriate implementation of EventServiceBase
  CIAO::EventServiceType type =
    supplier_config->service_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CIAO::EventServiceBase * event_service =
    this->event_service_factory_->create (type
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Connect the supplier
  event_service->connect_event_supplier (supplier_config ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Save a pointer to the event service implementation in a map.
  ACE_CString supplier_id =
    supplier_config->supplier_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->event_service_map_.bind (supplier_id.c_str (),
                                 event_service);
}

void
CIAO::Container::disconnect_event_consumer (
    const char * connection_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    Components::InvalidName,
    Components::InvalidConnection))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::disconnect_event_consumer\n"));

  CIAO::EventServiceBase * event_service;

  if (this->event_service_map_.unbind (connection_id, event_service) != 0)
    {
      ACE_THROW (Components::InvalidConnection ());
    }

  event_service->disconnect_event_consumer (connection_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

void
CIAO::Container::disconnect_event_supplier (
    const char * connection_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    Components::InvalidName,
    Components::InvalidConnection))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::disconnect_event_supplier\n"));

  CIAO::EventServiceBase * event_service;

  if (this->event_service_map_.unbind (connection_id, event_service) != 0)
    {
      ACE_THROW (Components::InvalidConnection ());
    }

  event_service->disconnect_event_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

}

void
CIAO::Container::push_event (
    Components::EventBase * ev,
    const char * connection_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    Components::InvalidConnection,
    CORBA::SystemException))
{

  ////ACE_DEBUG ((LM_DEBUG, "CIAO::Container::push_event\n"));

  CIAO::EventServiceBase * event_service;
  ////ACE_DEBUG ((LM_DEBUG, "Pushing from supplier id: %s\n", connection_id));
  if (this->event_service_map_.find (connection_id, event_service) != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "-----------Invalid Connection_id: %s ----------\n", connection_id));

      ACE_THROW (Components::InvalidConnection ());
    }
  event_service->push_event (ev ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

///////////////////////////////////////////////////////////////

ACE_Atomic_Op <ACE_SYNCH_MUTEX, long>
CIAO::Session_Container::serial_number_ (0);

CIAO::Session_Container::Session_Container (CORBA::ORB_ptr o,
                                            bool static_config_flag,
                                            const Static_Config_EntryPoints_Maps* maps)
  : Container (o),
    number_ (0),
    static_config_flag_ (static_config_flag),
    static_entrypts_maps_ (maps),
    sa_ (0)
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
  char buffer[MAXPATHLEN];

  if (name == 0)
    {
      this->number_ = ++CIAO::Session_Container::serial_number_;
      ACE_OS::sprintf (buffer, "CIAO::Session_Container-%ld",
                        this->number_);
      name = buffer;
    }

   CORBA::Object_var poa_object =
     this->orb_->resolve_initial_references("RootPOA" 
                                            ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   if (CORBA::is_nil (poa_object.in ()))
     ACE_ERROR_RETURN ((LM_ERROR,
                        " (%P|%t) Unable to initialize the POA.\n"),
                       -1);

   PortableServer::POA_var root_poa =
     PortableServer::POA::_narrow (poa_object.in () 
                                   ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   this->create_component_POA (name,
                               more_policies,
                               root_poa.in ()
                               ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   this->create_facet_consumer_POA (root_poa.in ()
                                    ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   PortableServer::POAManager_var poa_manager =
     root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   this->event_service_factory_ =
     ACE_Dynamic_Service<EventService_Factory>::instance
       ("CIAO_EventService_Factory");

   this->event_service_factory_->init (this->orb_.in (), root_poa.in ());

   return 0;
}


void
CIAO::Session_Container::create_component_POA (const char *name,
                                               const CORBA::PolicyList *p,
                                               PortableServer::POA_ptr root
                                               ACE_ENV_ARG_DECL)
{
  // Set up proper poa policies here.  Default policies seems to be
  // fine for session container.  If you add some other default
  // policies here, then you need to "add" more_policies below
  // instead of simply assigning more_policies to the init policy
  // list.
  CORBA::PolicyList policies (0);

  if (p != 0)
    policies = *p;

  PortableServer::POAManager_var poa_manager =
    root->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->component_poa_ =
    root->create_POA (name,
                      poa_manager.in (),
                      policies
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
CIAO::Session_Container::create_facet_consumer_POA (
    PortableServer::POA_ptr root
    ACE_ENV_ARG_DECL)
{
  PortableServer::POAManager_var poa_manager =
    root->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO::Utils::PolicyList_Destroyer policies (3);
  policies.length (3);

  policies[0] =
    root->create_id_assignment_policy (PortableServer::USER_ID
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  policies[1] =
    root->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Servant Retention Policy
  policies[2] =
    root->create_servant_retention_policy (PortableServer::RETAIN
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->facet_cons_poa_ =
    root->create_POA ("facet_consumer_poa",
                      poa_manager.in (),
                      policies
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_NEW_THROW_EX (this->sa_,
                    Servant_Activator (this->orb_.in ()),
                    CORBA::NO_MEMORY ());

  this->facet_cons_poa_->set_servant_manager (
      this->sa_
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::Object_ptr
CIAO::Session_Container::install_servant (PortableServer::Servant p,
                                          Container::OA_Type t
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_ptr tmp = 0;

  if (t == Container::Component)
    tmp = this->component_poa_.in ();
  else
    tmp = this->facet_cons_poa_.in ();

  PortableServer::ObjectId_var oid
    = tmp->activate_object (p
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var objref
    = tmp->id_to_reference (oid.in ()
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
    = this->component_poa_->activate_object (p
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var objref
    = this->component_poa_->id_to_reference (id.in ()
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  oid = id._retn ();

  return objref._retn ();
}


//@@ Apparently we need to be cautious when handling the exception
//   thrown here. We should make sure that new DnC interfaces
//   NodeApplication/NodeApplicationManager etc will cache the new
//   exceptions--> rethrow of new exceptions is needed.
//                                            --Tao
Components::CCMHome_ptr
CIAO::Session_Container::ciao_install_home (const char *exe_dll_name,
                                            const char *exe_entrypt,
                                            const char *sv_dll_name,
                                            const char *sv_entrypt
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::UnknownImplId,
                   Deployment::ImplEntryPointNotFound,
                   Deployment::InstallationFailure))
{

  HomeFactory hcreator = 0;
  ServantFactory screator = 0;

  if (this->static_config_flag_ == 0)
    {
      ACE_DLL executor_dll, servant_dll;

      if (exe_dll_name == 0 || sv_dll_name == 0)
        ACE_THROW_RETURN (Deployment::UnknownImplId (),
			  Components::CCMHome::_nil ());

      if (executor_dll.open (exe_dll_name,
			     ACE_DEFAULT_SHLIB_MODE,
			     0) != 0
	  || servant_dll.open (sv_dll_name,
			       ACE_DEFAULT_SHLIB_MODE,
			       0) != 0)
	{
	  ACE_THROW_RETURN (Deployment::UnknownImplId (),
			    Components::CCMHome::_nil ());
	}

      if (exe_entrypt == 0 || sv_entrypt == 0)
        ACE_THROW_RETURN (Deployment::ImplEntryPointNotFound (),
			  Components::CCMHome::_nil ());

      // @@ (OO) Please use a static_cast<> here instead of a C-style
      //         cast.  ANSI C++ casts are the preferred (and modern)
      //         way of casting in ACE/TAO/CIAO.
      hcreator = (HomeFactory) executor_dll.symbol (exe_entrypt);
      screator = (ServantFactory) servant_dll.symbol (sv_entrypt);
    }
  else
    {
      if (static_entrypts_maps_ == 0 ||
          static_entrypts_maps_->home_creator_funcptr_map_ == 0 ||
          static_entrypts_maps_->home_servant_creator_funcptr_map_ == 0)
        ACE_THROW_RETURN (Deployment::ImplEntryPointNotFound (),
			  Components::CCMHome::_nil ());

      ACE_CString exe_entrypt_str (exe_entrypt);
      static_entrypts_maps_->home_creator_funcptr_map_->
        find (exe_entrypt_str, hcreator);

      ACE_CString sv_entrypt_str (sv_entrypt);
      static_entrypts_maps_->home_servant_creator_funcptr_map_->
        find (sv_entrypt_str, screator);
    }

  if (hcreator == 0 || screator == 0)
    ACE_THROW_RETURN (Deployment::ImplEntryPointNotFound (),
		      Components::CCMHome::_nil ());

  Components::HomeExecutorBase_var home_executor = hcreator ();
  if (CORBA::is_nil (home_executor.in ()))
    ACE_THROW_RETURN (Deployment::InstallationFailure (),
		      Components::CCMHome::_nil ());

  PortableServer::Servant home_servant = screator (home_executor.in (),
                                                   this
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Components::CCMHome::_nil ());

  if (home_servant == 0)
    ACE_THROW_RETURN (Deployment::InstallationFailure (),
		      Components::CCMHome::_nil ());

  PortableServer::ServantBase_var safe (home_servant);

  CORBA::Object_var objref =
    this->install_servant (home_servant,
                            Container::Component
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Components::CCMHome::_nil ());

  Components::CCMHome_var homeref =
    Components::CCMHome::_narrow (objref.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return homeref._retn ();

}

void
CIAO::Session_Container::ciao_uninstall_home (Components::CCMHome_ptr homeref
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->uninstall (homeref,
                   Container::Component
                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
CIAO::Session_Container::uninstall (CORBA::Object_ptr objref,
                                    Container::OA_Type t
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_ptr tmp = 0;

  if (t == Container::Component)
    tmp = this->component_poa_.in ();
  else
    tmp = this->facet_cons_poa_.in ();

  PortableServer::ObjectId_var oid =
    tmp->reference_to_id (objref
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  tmp->deactivate_object (oid.in ()
                          ACE_ENV_ARG_PARAMETER);
}

void
CIAO::Session_Container::uninstall (PortableServer::Servant svt,
                                    Container::OA_Type t
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_ptr tmp = 0;

  if (t == Container::Component)
    tmp = this->component_poa_.in ();
  else
    tmp = this->facet_cons_poa_.in ();

  PortableServer::ObjectId_var oid
    = tmp->servant_to_id (svt
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  tmp->deactivate_object (oid.in ()
                          ACE_ENV_ARG_PARAMETER);
}

void
CIAO::Session_Container::uninstall_component (Components::CCMObject_ptr objref,
                                              PortableServer::ObjectId_out oid
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var id
    = this->component_poa_->reference_to_id (objref
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->component_poa_->deactivate_object (id.in ()
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  oid = id._retn ();
}

CORBA::Object_ptr
CIAO::Session_Container::generate_reference (const char *obj_id,
                                             const char *repo_id,
                                             Container::OA_Type t
                                             ACE_ENV_ARG_DECL)
{
  PortableServer::POA_ptr tmp = 0;

  if (t == Container::Component)
    tmp = this->component_poa_.in ();
  else
    tmp = this->facet_cons_poa_.in ();

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (obj_id);

  CORBA::Object_var objref =
    tmp->create_reference_with_id (oid.in (),
                                    repo_id
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return objref._retn ();
}

