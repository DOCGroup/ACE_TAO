// $Id$

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Notify_EventChannel_i.h"
#include "Notify_EventChannelFactory_i.h"
#include "Notify_SupplierAdmin_i.h"
#include "Notify_ConsumerAdmin_i.h"
#include "Notify_Event_Manager.h"
#include "Notify_Channel_Objects_Factory.h"
#include "Notify_POA_Factory.h"
#include "Notify_Factory.h"
#include "Notify_Event_Manager_Objects_Factory.h"
#include "Notify_Collection_Factory.h"
#include "Notify_AdminProperties.h"

#include "tao/debug.h"

ACE_RCSID(Notify, Notify_EventChannel_i, "$Id$")

TAO_Notify_EventChannel_i::TAO_Notify_EventChannel_i (TAO_Notify_EventChannelFactory_i* my_factory)
  :lock_ (0),
   destory_child_POAs_ (0),
   channel_factory_ (my_factory),
   channel_objects_factory_ (TAO_Notify_Factory::
                             get_channel_objects_factory ()),
   poa_factory_ (TAO_Notify_Factory::get_poa_factory ()),
   event_manager_objects_factory_ (TAO_Notify_Factory::create_event_manager_objects_factory ()),
   default_op_ (CosNotifyChannelAdmin::OR_OP),
   default_id_ (0),
   event_listener_list_ (0)
{
  channel_factory_->_add_ref ();
}

// Implementation skeleton destructor
TAO_Notify_EventChannel_i::~TAO_Notify_EventChannel_i (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,"in EC dtor\n"));
  // Cleanup all resources..

  delete this->event_manager_;
  delete this->lock_;
  delete this->event_listener_list_;

  this->channel_factory_->event_channel_destroyed (this->channel_id_);
  channel_factory_->_remove_ref ();

  delete event_manager_objects_factory_;
}

void
TAO_Notify_EventChannel_i::init (CosNotifyChannelAdmin::ChannelID channel_id, const CosNotification::QoSProperties& initial_qos, const CosNotification::AdminProperties& initial_admin, PortableServer::POA_ptr default_POA, PortableServer::POA_ptr my_POA, CORBA::Environment &ACE_TRY_ENV)
{
  this->channel_id_ = channel_id;

  this->my_POA_ = PortableServer::POA::_duplicate (my_POA);
  this->default_POA_ = PortableServer::POA::_duplicate (default_POA);

  this->lock_ =
    this->channel_objects_factory_->create_event_channel_lock (ACE_TRY_ENV);
  ACE_CHECK;

  this->event_manager_ =
    this->event_manager_objects_factory_->create_event_manager (this, ACE_TRY_ENV);
  ACE_CHECK;

  this->event_manager_->init (ACE_TRY_ENV);
  ACE_CHECK;

  // try to set initial qos params
  this->set_qos (initial_qos, ACE_TRY_ENV);
  ACE_CHECK;

  // try to set initial admin params
  this->set_admin (initial_admin, ACE_TRY_ENV);
  ACE_CHECK;

  // Create the POA for the CA's
  this->CA_POA_ =
    this->poa_factory_->create_consumer_admin_POA (this->my_POA_.in (), channel_id,
                                                   ACE_TRY_ENV);
  ACE_CHECK;

  // Create the POA for the SA's
  this->SA_POA_ =
    this->poa_factory_->create_supplier_admin_POA (this->my_POA_.in (), channel_id,
                                                   ACE_TRY_ENV);
  ACE_CHECK;

  this->event_listener_list_ =
    TAO_Notify_Factory::get_collection_factory ()->create_event_listener_list (ACE_TRY_ENV);
  ACE_CHECK;

  // Create the default Consumer Admin. Because the ID_Pool is being used
  // the first time here, it will generate the id 0.
  CosNotifyChannelAdmin::AdminID id_unused;

  CosNotifyChannelAdmin::ConsumerAdmin_var def_consumer_admin =
    this->new_for_consumers (default_op_, id_unused, ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::SupplierAdmin_var def_supplier_admin =
    this->new_for_suppliers (default_op_, id_unused, ACE_TRY_ENV);
  ACE_CHECK;
}

TAO_Notify_Event_Manager*
TAO_Notify_EventChannel_i::get_event_manager (void)
{
  return this->event_manager_;
}

void
TAO_Notify_EventChannel_i::consumer_admin_destroyed (CosNotifyChannelAdmin::AdminID CA_ID)
{
  this->consumer_admin_ids_.put (CA_ID);
}

void
TAO_Notify_EventChannel_i::supplier_admin_destroyed (CosNotifyChannelAdmin::AdminID SA_ID)
{
  this->supplier_admin_ids_.put (SA_ID);
}

PortableServer::POA_ptr
TAO_Notify_EventChannel_i::get_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_EventChannel_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_var obj = this->poa_factory_->
    servant_to_reference (this->my_POA_.in (), this, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  return CosNotifyChannelAdmin::EventChannel::_narrow (obj.in ());
}


void
TAO_Notify_EventChannel_i::unregister_listener (TAO_Notify_EventListener* group_listener, CORBA::Environment &ACE_TRY_ENV)
{
  // UnRegister the group listener.
  this->event_listener_list_->disconnected (group_listener, ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_EventChannel_i::destroy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->event_manager_->shutdown (ACE_TRY_ENV);
  ACE_CHECK;

  // Deactivate ourselves.
  this->poa_factory_->deactivate_object (this,
                                         this->my_POA_.in (),
                                         ACE_TRY_ENV);
  ACE_CHECK;

  // shutdown consumer admins's.
  TAO_Notify_Shutdown_Worker shutdown_worker;

  this->event_listener_list_->for_each (&shutdown_worker, ACE_TRY_ENV);
  ACE_CHECK;

  // release all references.
  this->event_listener_list_->shutdown (ACE_TRY_ENV);
  ACE_CHECK;

  // @@ TODO: We need a way to send shutdown messages to the proxy consumers too.
  // (event listeners are proxy suppliers or CA's in disguise)

  this->poa_factory_->destroy_POA (this->CA_POA_.in (),
                                   ACE_TRY_ENV);
  ACE_CHECK;

  this->poa_factory_->destroy_POA (this->SA_POA_.in (),
                                   ACE_TRY_ENV);
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannel_i::MyFactory (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->channel_factory_->get_ref (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::default_consumer_admin (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->get_consumeradmin (this->default_id_, ACE_TRY_ENV);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::default_supplier_admin (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->get_supplieradmin (this->default_id_, ACE_TRY_ENV);
}

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_EventChannel_i::default_filter_factory (CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->channel_factory_->get_default_filter_factory ();
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op, CosNotifyChannelAdmin::AdminID_out id, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // @@ use auto_ptr
  TAO_Notify_ConsumerAdmin_i* consumer_admin =
    this->channel_objects_factory_->create_consumer_admin (this, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  PortableServer::ServantBase_var consumeradmin_var (consumer_admin);

  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

    id = this->consumer_admin_ids_.get ();
  }

  consumer_admin->init (id, op, this->CA_POA_.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  CORBA::Object_var obj =
    this->poa_factory_->activate_object_with_id (id,
                                                 this->CA_POA_.in (),
                                                 consumer_admin,
                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  // Register the group listener.
  this->event_listener_list_->connected (consumer_admin, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  this->consumer_admin_ids_.next ();
  return CosNotifyChannelAdmin::ConsumerAdmin::_narrow (obj.in ());
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op, CosNotifyChannelAdmin::AdminID_out id, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_SupplierAdmin_i* supplieradmin =
    this->channel_objects_factory_->create_supplier_admin (this, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  PortableServer::ServantBase_var supplieradmin_var (supplieradmin);

  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                        CORBA::INTERNAL ());
    id = this->supplier_admin_ids_.get ();
  }

  supplieradmin->init (id, op, this->SA_POA_.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  CORBA::Object_var obj = this->poa_factory_->
    activate_object_with_id (id,
                             this->SA_POA_.in (),
                             supplieradmin,
                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  this->supplier_admin_ids_.next ();
  return CosNotifyChannelAdmin::SupplierAdmin::_narrow (obj.in ());
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::get_consumeradmin (CosNotifyChannelAdmin::AdminID id, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::AdminNotFound
                   ))
{
  CORBA::Object_var obj =
    this->poa_factory_->id_to_reference (id, this->CA_POA_.in (),
                                         ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  return CosNotifyChannelAdmin::ConsumerAdmin::_narrow (obj.in ());
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::get_supplieradmin (CosNotifyChannelAdmin::AdminID id, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::AdminNotFound
                   ))
{
  CORBA::Object_var obj =
    this->poa_factory_->id_to_reference (id, this->SA_POA_.in (),
                                         ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  return CosNotifyChannelAdmin::SupplierAdmin::_narrow (obj.in ());
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_Notify_EventChannel_i::get_all_consumeradmins (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->consumer_admin_ids_.get_sequence (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_Notify_EventChannel_i::get_all_supplieradmins (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  return this->supplier_admin_ids_.get_sequence (ACE_TRY_ENV);
}

CosNotification::QoSProperties*
TAO_Notify_EventChannel_i::get_qos (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->qos_admin_.get_qos (ACE_TRY_ENV);
}

void
TAO_Notify_EventChannel_i::set_qos (const CosNotification::QoSProperties & qos,CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.set_qos (qos, ACE_TRY_ENV);
}

void
TAO_Notify_EventChannel_i::validate_qos (const CosNotification::QoSProperties & required_qos, CosNotification::NamedPropertyRangeSeq_out available_qos, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.validate_qos (required_qos, available_qos, ACE_TRY_ENV);
}

CosNotification::AdminProperties*
TAO_Notify_EventChannel_i::get_admin (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // Delegate to Event Manager.
  return this->event_manager_->admin_properties ()->get_admin (ACE_TRY_ENV);
}

void
TAO_Notify_EventChannel_i::set_admin (const CosNotification::AdminProperties & admin, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedAdmin
                   ))
{
  this->event_manager_->admin_properties ()->set_admin (admin, ACE_TRY_ENV);
}

CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::for_consumers (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // There is no way to destroy CosEventChannelAdmin::ConsumerAdmin's
  // so we just return the default Consumer Admin here.
  // TODO: find a way to disable the destroy method in the default Admin.

  CORBA::Object_var obj =
    this->get_consumeradmin (this->default_id_, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ConsumerAdmin::_nil ());

  return CosEventChannelAdmin::ConsumerAdmin::_narrow (obj.in (),
                                                       ACE_TRY_ENV);
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::for_suppliers (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CORBA::Object_var obj =
    this->get_supplieradmin (this->default_id_, ACE_TRY_ENV);

  return CosEventChannelAdmin::SupplierAdmin::_narrow (obj.in (),
                                                       ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::AdminID, CosNotifyChannelAdmin::AdminIDSeq>;
template class TAO_Notify_ID_Pool<CosNotifyChannelAdmin::AdminID>;

template class ACE_Unbounded_Set<CosNotifyChannelAdmin::AdminID>;
template class ACE_Unbounded_Set_Iterator<CosNotifyChannelAdmin::AdminID>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::AdminID, CosNotifyChannelAdmin::AdminIDSeq>
#pragma instantiate TAO_Notify_ID_Pool<CosNotifyChannelAdmin::AdminID>

#pragma instantiate ACE_Unbounded_Set<CosNotifyChannelAdmin::AdminID>
#pragma instantiate ACE_Unbounded_Set_Iterator<CosNotifyChannelAdmin::AdminID>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
