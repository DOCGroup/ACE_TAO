// $Id$

#include "Container_Base.h"
#include "Cookies.h"
#include "ace/DLL.h"

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
CIAO::Container::_ciao_the_POA (void)
{
  return this->poa_.in ();
}

CORBA::ORB_ptr
CIAO::Container::_ciao_the_ORB ()
{
  return this->orb_.in ();
}

::Components::Cookie * CIAO::Container::_ciao_specify_event_service (
    const char * event_name,
    const char * publisher_name,
    const char * service_name
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::_ciao_specify_event_service\n"));

  CIAO::EventServiceInfo service_info;

  if (ACE_OS::strcmp (service_name, "EC") == 0)
  {
    service_info.service_type = EC;
  }
  else if (ACE_OS::strcmp (service_name, "RTEC") == 0)
  {
    service_info.service_type = RTEC;
  }
  else if (ACE_OS::strcmp (service_name, "NS") == 0)
  {
    service_info.service_type = NS;
  }
  else if (ACE_OS::strcmp (service_name, "RTNS") == 0)
  {
    service_info.service_type = RTNS;
  }
  else if (ACE_OS::strcmp (service_name, "DIRECT") == 0)
  {
    service_info.service_type = DIRECT;
  }

  if (this->ciao_event_types_map_.find (event_name, service_info.event_type_id) == -1)
    {
      service_info.event_type_id =
        ACE_ES_EVENT_ANY + 1 + this->ciao_event_types_map_.current_size ();
      this->ciao_event_types_map_.bind (event_name, service_info.event_type_id);
    }

  if (this->ciao_publishers_map_.find (publisher_name, service_info.event_source_id) == -1)
    {
      service_info.event_source_id =
        ACE_ES_EVENT_SOURCE_ANY + 1 + this->ciao_publishers_map_.current_size ();
      this->ciao_publishers_map_.bind (publisher_name, service_info.event_source_id);
    }

  ACE_DEBUG ((LM_DEBUG, "service: %i\n", service_info.service_type));
  ACE_DEBUG ((LM_DEBUG, "event: %i\n", service_info.event_type_id));
  ACE_DEBUG ((LM_DEBUG, "source: %i\n", service_info.event_source_id));

  ::Components::Cookie * return_cookie;
  ACE_NEW_RETURN (return_cookie,
                 ::CIAO::EventServiceInfo_Cookie (service_info),
                 0);
  return return_cookie;

}


::Components::Cookie * CIAO::Container::_ciao_connect_event_consumer (
    ::Components::EventConsumerBase_ptr c,
    ::Components::Cookie *ck
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException ))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::_ciao_connect_event_consumer\n"));

  CIAO::EventServiceInfo service_info;

  if (CORBA::is_nil (c))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
    }

  if (ck == 0 || ::CIAO::EventServiceInfo_Cookie::extract (ck, service_info) == -1)
    {
      ACE_THROW (
      ::Components::InvalidConnection ());
    }

  ACE_DEBUG ((LM_DEBUG, "service: %i\n", service_info.service_type));
  ACE_DEBUG ((LM_DEBUG, "event: %i\n", service_info.event_type_id));
  ACE_DEBUG ((LM_DEBUG, "source: %i\n", service_info.event_source_id));

  switch (service_info.service_type)
  {
  case EC:
    break;
  case RTEC:
    return this->connect_rt_event_consumer (c, service_info);
    break;
  case NS:
    break;
  case RTNS:
    break;
  case DIRECT:
    break;
  }

  return 0;

}


::Components::Cookie * CIAO::Container::_ciao_connect_event_supplier (
    ::Components::Cookie *ck
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::_ciao_connect_event_supplier\n"));

  CIAO::EventServiceInfo service_info;

  if (ck == 0 || ::CIAO::EventServiceInfo_Cookie::extract (ck, service_info) == -1)
    {
      ACE_THROW (
      ::Components::InvalidConnection ());
    }

  switch (service_info.service_type)
  {
  case EC:
    break;
  case RTEC:
    return this->connect_rt_event_supplier (service_info);
    break;
  case NS:
    break;
  case RTNS:
    break;
  case DIRECT:
    break;
  }

  return 0;

}

void CIAO::Container::_ciao_disconnect_event_consumer
      (::Components::Cookie *ck
       ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::InvalidName,
                       ::Components::InvalidConnection))
{
  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::_ciao_unsubscribe_event\n"));

  CORBA::Object_var obj = CORBA::Object::_nil ();
  PortableServer::ObjectId oid;

  if (ck == 0 || ::CIAO::ObjectId_Cookie::extract (ck, oid) == -1)
    {
      ACE_THROW (
      ::Components::InvalidConnection ());
    }

  obj = this->root_poa_->id_to_reference (oid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventComm::PushConsumer_var push_consumer =
    RtecEventComm::PushConsumer::_narrow (obj.in ());
  if (CORBA::is_nil (push_consumer.in ()))
    {
      ACE_THROW (
        ::Components::InvalidConnection ());
    }
  push_consumer->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
	ACE_CHECK;
  
}

void CIAO::Container::_ciao_disconnect_event_supplier
      (::Components::Cookie *ck
       ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::InvalidName,
                       ::Components::InvalidConnection))
{

}

void CIAO::Container::_ciao_push_event (::Components::EventBase *ev,
                                       ::Components::Cookie *ck
                                       ACE_ENV_ARG_DECL)
                                      ACE_THROW_SPEC
                                      ((CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO::Container::_ciao_push_event\n"));

  RtecEventComm::EventSet events (1);
  events.length (1);
  events[0].header.source = ACE_ES_EVENT_SOURCE_ANY;
  events[0].header.type = ACE_ES_EVENT_ANY;
  events[0].data.any_value <<= ev;
  ev->_add_ref ();

  CORBA::Object_var obj = CORBA::Object::_nil ();
  PortableServer::ObjectId oid;

  if (ck == 0 || ::CIAO::ObjectId_Cookie::extract (ck, oid) == -1)
    {
      ACE_THROW (
      ::Components::InvalidConnection ());
    }

  obj = this->root_poa_->id_to_reference (oid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer =
    RtecEventChannelAdmin::ProxyPushConsumer::_narrow (obj.in ());
  if (CORBA::is_nil (proxy_consumer.in ()))
    {
      ACE_THROW (
        ::Components::InvalidConnection ());
    }
  proxy_consumer->push (events ACE_ENV_ARG_PARAMETER);
	ACE_CHECK;
}

void CIAO::Container::create_rt_event_channel (void)
{
  TAO_EC_Event_Channel_Attributes attributes (this->root_poa_.in (),
                                              this->root_poa_.in ());
  TAO_EC_Event_Channel * ec_servant;
  ACE_NEW (ec_servant, TAO_EC_Event_Channel (attributes));
  ec_servant->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->ciao_rt_event_channel_ = ec_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

::Components::Cookie * CIAO::Container::connect_rt_event_consumer (
    ::Components::EventConsumerBase_ptr c,
    ::CIAO::EventServiceInfo service_info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
  if (CORBA::is_nil (this->ciao_rt_event_channel_.in ()))
    {
      this->create_rt_event_channel ();
    }

  ::Components::EventConsumerBase_var sub =
    ::Components::EventConsumerBase::_duplicate (c);

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->ciao_rt_event_channel_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier =
    consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create and register consumer servant
  CIAO::RTEventServiceConsumer_impl * consumer_servant;
  ACE_NEW_RETURN (consumer_servant,
                  CIAO::RTEventServiceConsumer_impl (orb_.in (), sub.in ()),
                  0);
  RtecEventComm::PushConsumer_var push_consumer =
    consumer_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Set QoS properties and connect
  ACE_ConsumerQOS_Factory qos;
  qos.start_logical_and_group (2);
  qos.insert_type (service_info.event_type_id,
                   0);
  qos.insert_source (service_info.event_source_id,
                     0);
  proxy_supplier->connect_push_consumer (push_consumer.in (),
                                         qos.get_ConsumerQOS ()
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var oid = this->root_poa_->reference_to_id (push_consumer.in ());

  ::Components::Cookie * return_cookie;
  ACE_NEW_RETURN (return_cookie,
                 ::CIAO::ObjectId_Cookie (oid.in ()),
                 0);
  return return_cookie;
}

::Components::Cookie * CIAO::Container::connect_rt_event_supplier (
    ::CIAO::EventServiceInfo service_info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
  if (CORBA::is_nil (this->ciao_rt_event_channel_.in ()))
    {
      this->create_rt_event_channel ();
    }

  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->ciao_rt_event_channel_->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer =
    supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create and register supplier servant
  CIAO::RTEventServiceSupplier_impl * supplier_servant;
  ACE_NEW_RETURN (supplier_servant,
                  CIAO::RTEventServiceSupplier_impl (orb_.in ()),
                  0);
  RtecEventComm::PushSupplier_var push_supplier =
    supplier_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Set QoS properties and connect
  ACE_SupplierQOS_Factory qos;
  qos.insert (service_info.event_source_id,
              service_info.event_type_id,
              0,
              1);

  proxy_consumer->connect_push_supplier (push_supplier.in (),
                                         qos.get_SupplierQOS ()
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var oid = this->root_poa_->reference_to_id (proxy_consumer.in ());

    ::Components::Cookie * return_cookie;
  ACE_NEW_RETURN (return_cookie,
                 ::CIAO::ObjectId_Cookie (oid.in ()),
                 0);
  return return_cookie;

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

   this->root_poa_ =
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

   this->poa_ = this->root_poa_->create_POA (name,
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

CIAO::RTEventServiceSupplier_impl::RTEventServiceSupplier_impl (void)
{
}

CIAO::RTEventServiceSupplier_impl::RTEventServiceSupplier_impl (CORBA::ORB_ptr orb) :
  orb_ (CORBA::ORB::_duplicate (orb))
{
}
  
void CIAO::RTEventServiceSupplier_impl::disconnect_push_supplier (void)
{
  CORBA::Object_var poa_object =
    orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (CORBA::is_nil (root_poa.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));
  PortableServer::ObjectId_var oid = root_poa->servant_to_id (this);
  root_poa->deactivate_object (oid);
}

CIAO::RTEventServiceConsumer_impl::RTEventServiceConsumer_impl (void)
{
}

CIAO::RTEventServiceConsumer_impl::RTEventServiceConsumer_impl
  (CORBA::ORB_ptr orb,
   Components::EventConsumerBase_ptr consumer) :
    orb_ (CORBA::ORB::_duplicate (orb)),
    event_consumer_ (Components::EventConsumerBase::_duplicate (consumer))
{
}

void CIAO::RTEventServiceConsumer_impl::push (const RtecEventComm::EventSet& events)
{
  ACE_DEBUG ((LM_DEBUG, "CIAO::RTEventServiceConsumer_impl::push\n"));
  for (unsigned long i = 0; i < events.length (); ++i)
    {
      ::Components::EventBase * ev;
      if (events[i].data.any_value >>= ev)
        {
          this->event_consumer_->push_event (ev
                                           ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

void CIAO::RTEventServiceConsumer_impl::disconnect_push_consumer (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "CIAO::RTEventServiceConsumer_impl::disconnect_push_consumer\n"));

  CORBA::Object_var poa_object =
    orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (CORBA::is_nil (root_poa.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));
  PortableServer::ObjectId_var oid = root_poa->servant_to_id (this);
  root_poa->deactivate_object (oid);
}
