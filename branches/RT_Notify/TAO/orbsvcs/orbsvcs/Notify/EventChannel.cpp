// $Id$

#include "EventChannel.h"
#include "EventChannelFactory.h"
#include "Proxy.h"
#include "Admin.h"
#include "Event_Manager.h"
#include "Properties.h"
#include "Builder.h"
#include "ThreadPool_Task.h"
#include "QoSAdmin.h"

#if ! defined (__ACE_INLINE__)
#include "EventChannel.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_EventChannel, "$Id$")

  TAO_NS_EventChannel::TAO_NS_EventChannel (void)
{
}

TAO_NS_EventChannel::~TAO_NS_EventChannel ()
{
  delete event_manager_;
}

PortableServer::Servant
TAO_NS_EventChannel::servant (void)
{
  return this;
}

void
TAO_NS_EventChannel::_add_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  this->_incr_refcnt ();
}

void
TAO_NS_EventChannel::_remove_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  this->_decr_refcnt ();
}

void
TAO_NS_EventChannel::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_NS_EventChannel::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->inherited::destroy (this ACE_ENV_ARG_PARAMETER);
  this->event_manager_->shutdown ();
}

void
TAO_NS_EventChannel::set_qos (const CosNotification::QoSProperties & qos ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_->apply_qos (this, qos ACE_ENV_ARG_PARAMETER);
}

::CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_NS_EventChannel::MyFactory ()
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}

::CosNotifyChannelAdmin::ConsumerAdmin_ptr TAO_NS_EventChannel::default_consumer_admin (

  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}

::CosNotifyChannelAdmin::SupplierAdmin_ptr TAO_NS_EventChannel::default_supplier_admin (

  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}

::CosNotifyFilter::FilterFactory_ptr TAO_NS_EventChannel::default_filter_factory (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->parent_->get_default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NS_EventChannel::new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                        CosNotifyChannelAdmin::AdminID_out id
                                        )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  /// Builder for ConsumerAdmins
  return TAO_NS_PROPERTIES::instance()->builder ()->build_consumer_admin (this, op, id);
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
  /// Builder for SupplierAdmins
  return TAO_NS_PROPERTIES::instance()->builder ()->build_supplier_admin (this, op, id);
}

::CosNotifyChannelAdmin::ConsumerAdmin_ptr TAO_NS_EventChannel::get_consumeradmin (
                                                                                   CosNotifyChannelAdmin::AdminID id
                                                                                   )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminNotFound
                   ))

{
  //Add your implementation here
  return 0;
}

::CosNotifyChannelAdmin::SupplierAdmin_ptr TAO_NS_EventChannel::get_supplieradmin (
                                                                                   CosNotifyChannelAdmin::AdminID id
                                                                                   )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminNotFound
                   ))

{
  //Add your implementation here
  return 0;
}

::CosNotifyChannelAdmin::AdminIDSeq * TAO_NS_EventChannel::get_all_consumeradmins (

  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}

::CosNotifyChannelAdmin::AdminIDSeq * TAO_NS_EventChannel::get_all_supplieradmins (

  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}

::CosNotification::QoSProperties * TAO_NS_EventChannel::get_qos (

  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}

void TAO_NS_EventChannel::validate_qos (
                                        const CosNotification::QoSProperties & required_qos,
                                        CosNotification::NamedPropertyRangeSeq_out available_qos
                                        )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))

{
  //Add your implementation here
}

::CosNotification::AdminProperties * TAO_NS_EventChannel::get_admin (

  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}

void TAO_NS_EventChannel::set_admin (
                                     const CosNotification::AdminProperties & admin
                                     )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedAdmin
                   ))

{
  //Add your implementation here
}

::CosEventChannelAdmin::ConsumerAdmin_ptr TAO_NS_EventChannel::for_consumers (

  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}

::CosEventChannelAdmin::SupplierAdmin_ptr TAO_NS_EventChannel::for_suppliers (

  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}
