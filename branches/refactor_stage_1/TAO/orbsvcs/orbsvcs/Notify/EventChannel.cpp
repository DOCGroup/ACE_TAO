// $Id$

#include "EventChannel.h"

#if ! defined (__ACE_INLINE__)
#include "EventChannel.inl"
#endif /* __ACE_INLINE__ */

#include "Container_T.h"
#include "EventChannelFactory.h"
#include "ConsumerAdmin.h"
#include "SupplierAdmin.h"
#include "Event_Manager.h"
#include "Properties.h"
#include "Factory.h"
#include "Builder.h"
#include "Find_Worker_T.h"
#include "Seq_Worker_T.h"

ACE_RCSID(Notify, TAO_NS_EventChannel, "$Id$")

typedef TAO_NS_Find_Worker_T<TAO_NS_ConsumerAdmin
                             , CosNotifyChannelAdmin::ConsumerAdmin
                             , CosNotifyChannelAdmin::ConsumerAdmin_ptr
                             , CosNotifyChannelAdmin::AdminNotFound>
TAO_NS_ConsumerAdmin_Find_Worker;

typedef TAO_NS_Find_Worker_T<TAO_NS_SupplierAdmin
                             , CosNotifyChannelAdmin::SupplierAdmin
                             , CosNotifyChannelAdmin::SupplierAdmin_ptr
                             , CosNotifyChannelAdmin::AdminNotFound>
TAO_NS_SupplierAdmin_Find_Worker;

typedef TAO_NS_Seq_Worker_T<TAO_NS_ConsumerAdmin> TAO_NS_ConsumerAdmin_Seq_Worker;
typedef TAO_NS_Seq_Worker_T<TAO_NS_SupplierAdmin> TAO_NS_SupplierAdmin_Seq_Worker;

TAO_NS_EventChannel::TAO_NS_EventChannel (void)
  : ecf_ (0)
  , ca_container_ (0)
  , sa_container_ (0)
{
}

TAO_NS_EventChannel::~TAO_NS_EventChannel ()
{
  delete this->event_manager_; // The EventChannel always owns the EventManager.

  this->ecf_->_decr_refcnt ();
}

void
TAO_NS_EventChannel::init (TAO_NS_EventChannelFactory* ecf
                           , const CosNotification::QoSProperties & initial_qos
                           , const CosNotification::AdminProperties & initial_admin
                           ACE_ENV_ARG_DECL)
{
  this->TAO_NS_Object::init (ecf);

  this->ecf_ = ecf;

  this->ecf_->_incr_refcnt ();

  // Init ca_container_
  ACE_NEW_THROW_EX (this->ca_container_,
                    TAO_NS_ConsumerAdmin_Container (),
                    CORBA::INTERNAL ());
  ACE_CHECK;

  this->ca_container_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Init ca_container_
  ACE_NEW_THROW_EX (this->sa_container_,
                    TAO_NS_SupplierAdmin_Container (),
                    CORBA::INTERNAL ());
  ACE_CHECK;

  this->sa_container_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_NS_AdminProperties* admin_properties = 0;

  // Set the admin properties.
  ACE_NEW_THROW_EX (admin_properties,
                    TAO_NS_AdminProperties (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  this->admin_properties_ = admin_properties;

  // create the event manager. @@ use factory
  ACE_NEW_THROW_EX (this->event_manager_,
                    TAO_NS_Event_Manager (),
                    CORBA::INTERNAL ());
  ACE_CHECK;

  this->event_manager_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  const CosNotification::QoSProperties &default_ec_qos =
    TAO_NS_PROPERTIES::instance ()->default_event_channel_qos_properties ();

  this->set_qos (default_ec_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->set_qos (initial_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->set_admin (initial_admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::AdminID id;

  // Set the default ConsumerAdmin.
  this->default_consumer_admin_ = this->new_for_consumers (CosNotifyChannelAdmin::OR_OP, id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Set the default SupplierAdmin.
  this->default_supplier_admin_ = this->new_for_suppliers (CosNotifyChannelAdmin::OR_OP, id ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_EventChannel::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_NS_EventChannel::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}

void
TAO_NS_EventChannel::release (void)
{
  delete this;
  //@@ inform factory
}

int
TAO_NS_EventChannel::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_NS_Object::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return 1;

  this->ca_container_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  this->sa_container_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  delete this->ca_container_;
  delete this->sa_container_;

  this->event_manager_->shutdown ();

  return 0;
}

void
TAO_NS_EventChannel::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return;

  ACE_CHECK;

  this->ecf_->remove (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_EventChannel::remove (TAO_NS_ConsumerAdmin* consumer_admin ACE_ENV_ARG_DECL)
{
  this->ca_container_->remove (consumer_admin ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_EventChannel::remove (TAO_NS_SupplierAdmin* supplier_admin ACE_ENV_ARG_DECL)
{
  this->sa_container_->remove (supplier_admin ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_EventChannel::set_qos (const CosNotification::QoSProperties & qos ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))
{
  this->TAO_NS_Object::set_qos (qos ACE_ENV_ARG_PARAMETER);
}

CosNotification::QoSProperties*
TAO_NS_EventChannel::get_qos (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->TAO_NS_Object::get_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_NS_EventChannel::MyFactory (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->ecf_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NS_EventChannel::default_consumer_admin (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (this->default_consumer_admin_.in ());
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_NS_EventChannel::default_supplier_admin (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (this->default_supplier_admin_.in ());
}

::CosNotifyFilter::FilterFactory_ptr TAO_NS_EventChannel::default_filter_factory (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->ecf_->get_default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NS_EventChannel::new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                        CosNotifyChannelAdmin::AdminID_out id ACE_ENV_ARG_DECL
                                        )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  return TAO_NS_PROPERTIES::instance()->builder()->build_consumer_admin (this, op, id ACE_ENV_ARG_PARAMETER);
}

::CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_NS_EventChannel::new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                        CosNotifyChannelAdmin::AdminID_out id
                                        ACE_ENV_ARG_DECL
                                        )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return TAO_NS_PROPERTIES::instance()->builder()->build_supplier_admin (this, op, id ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NS_EventChannel::get_consumeradmin (CosNotifyChannelAdmin::AdminID id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminNotFound
                   ))
{
  TAO_NS_ConsumerAdmin_Find_Worker find_worker;

  return find_worker.resolve (id, *this->ca_container_ ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_NS_EventChannel::get_supplieradmin (CosNotifyChannelAdmin::AdminID id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminNotFound
                   ))
{
  TAO_NS_SupplierAdmin_Find_Worker find_worker;

  return find_worker.resolve (id, *this->sa_container_ ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_NS_EventChannel::get_all_consumeradmins (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_NS_ConsumerAdmin_Seq_Worker seq_worker;

  return seq_worker.create (*this->ca_container_ ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_NS_EventChannel::get_all_supplieradmins (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_NS_SupplierAdmin_Seq_Worker seq_worker;

  return seq_worker.create (*this->sa_container_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_EventChannel::set_admin (const CosNotification::AdminProperties & admin ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedAdmin
                   ))
{
  this->admin_properties_->init (admin);
}

CosNotification::AdminProperties*
TAO_NS_EventChannel::get_admin (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotification::AdminProperties_var properties;

  ACE_NEW_THROW_EX (properties,
                    CosNotification::AdminProperties (),
                    CORBA::NO_MEMORY ());

  this->admin_properties_->populate (properties);

  return properties._retn ();
}

CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_NS_EventChannel::for_consumers (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosEventChannelAdmin::ConsumerAdmin::_duplicate (this->default_consumer_admin_.in ());
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_NS_EventChannel::for_suppliers (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosEventChannelAdmin::SupplierAdmin::_duplicate (this->default_supplier_admin_.in ());
}

void
TAO_NS_EventChannel::validate_qos (const CosNotification::QoSProperties & /*required_qos*/,
                                   CosNotification::NamedPropertyRangeSeq_out /*available_qos*/
                                   ACE_ENV_ARG_DECL
                                   )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_NS_Find_Worker_T<TAO_NS_ConsumerAdmin
                                    , CosNotifyChannelAdmin::ConsumerAdmin
                                    , CosNotifyChannelAdmin::ConsumerAdmin_ptr
                                    , CosNotifyChannelAdmin::AdminNotFound>;
template class TAO_NS_Find_Worker_T<TAO_NS_SupplierAdmin
                                    , CosNotifyChannelAdmin::SupplierAdmin
                                    , CosNotifyChannelAdmin::SupplierAdmin_ptr
                                    , CosNotifyChannelAdmin::AdminNotFound>;

template class TAO_NS_Seq_Worker_T<TAO_NS_ConsumerAdmin>;
template class TAO_NS_Seq_Worker_T<TAO_NS_SupplierAdmin>;

template class TAO_NS_Container_T <TAO_NS_ConsumerAdmin>;
template class TAO_NS_Container_T <TAO_NS_SupplierAdmin>;

template class TAO_ESF_Shutdown_Proxy<TAO_NS_ConsumerAdmin>;
template class TAO_ESF_Shutdown_Proxy<TAO_NS_SupplierAdmin>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_NS_Find_Worker_T<TAO_NS_ConsumerAdmin
                                    , CosNotifyChannelAdmin::ConsumerAdmin
                                    , CosNotifyChannelAdmin::ConsumerAdmin_ptr
                                    , CosNotifyChannelAdmin::AdminNotFound>
#pragma instantiate TAO_NS_Find_Worker_T<TAO_NS_SupplierAdmin
                                    , CosNotifyChannelAdmin::SupplierAdmin
                                    , CosNotifyChannelAdmin::SupplierAdmin_ptr
                                    , CosNotifyChannelAdmin::AdminNotFound>

#pragma instantiate TAO_NS_Seq_Worker_T<TAO_NS_ConsumerAdmin>
#pragma instantiate TAO_NS_Seq_Worker_T<TAO_NS_SupplierAdmin>

#pragma instantiate TAO_NS_Container_T <TAO_NS_ConsumerAdmin>
#pragma instantiate TAO_NS_Container_T <TAO_NS_SupplierAdmin>

#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_NS_ConsumerAdmin>
#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_NS_SupplierAdmin>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
