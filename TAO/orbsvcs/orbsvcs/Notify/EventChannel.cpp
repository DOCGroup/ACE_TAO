// $Id$

#include "EventChannel.h"

#if ! defined (__ACE_INLINE__)
#include "EventChannel.inl"
#endif /* __ACE_INLINE__ */

#include "Proxy.h"
#include "Admin.h"
#include "EventChannelFactory.h"
#include "Notify_Service.h"
#include "Event_Manager.h"
#include "Properties.h"
#include "Builder.h"
#include "ThreadPool_Task.h"

ACE_RCSID(RT_Notify, TAO_NS_EventChannel, "$Id$")

  TAO_NS_EventChannel::TAO_NS_EventChannel (void)
{
}

TAO_NS_EventChannel::~TAO_NS_EventChannel ()
{
  delete this->event_manager_;
  delete this->admin_properties_;
}

void
TAO_NS_EventChannel::init (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID id;

  this->default_consumer_admin_ =
    TAO_NS_PROPERTIES::instance()->builder ()->build_consumer_admin (this, CosNotifyChannelAdmin::OR_OP, id ACE_ENV_ARG_PARAMETER);

  this->default_supplier_admin_ =
    TAO_NS_PROPERTIES::instance()->builder ()->build_supplier_admin (this, CosNotifyChannelAdmin::OR_OP, id ACE_ENV_ARG_PARAMETER);
}

PortableServer::Servant
TAO_NS_EventChannel::servant (void)
{
  return this;
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

void
TAO_NS_EventChannel::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->inherited::destroy (this ACE_ENV_ARG_PARAMETER);
  this->event_manager_->shutdown ();

  /*  this->default_consumer_admin_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      this->default_supplier_admin_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);*/
}

void
TAO_NS_EventChannel::insert (TAO_NS_Admin* admin ACE_ENV_ARG_DECL)
{
  inherited::insert (admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (admin->type () == TAO_NS_Admin::CONSUMER_ADMIN)
    this->consumer_admin_id_list_.insert (admin->id ());
  else
    this->supplier_admin_id_list_.insert (admin->id ());
}

void
TAO_NS_EventChannel::remove (TAO_NS_Admin* admin ACE_ENV_ARG_DECL)
{
  inherited::remove (admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (admin->type () == TAO_NS_Admin::CONSUMER_ADMIN)
    this->consumer_admin_id_list_.remove (admin->id ());
  else
    this->supplier_admin_id_list_.remove (admin->id ());
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
  CORBA::Object_var object = this->parent_->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannelFactory::_nil ());

  CosNotifyChannelAdmin::EventChannelFactory_var ecf_ret;

  ecf_ret = CosNotifyChannelAdmin::EventChannelFactory::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannelFactory::_nil ());

  return ecf_ret._retn ();
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
  return this->parent_->get_default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NS_EventChannel::new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                        CosNotifyChannelAdmin::AdminID_out id ACE_ENV_ARG_DECL
                                        )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  /// Builder for ConsumerAdmins
  return TAO_NS_PROPERTIES::instance()->builder ()->build_consumer_admin (this, op, id ACE_ENV_ARG_PARAMETER);
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
  return TAO_NS_PROPERTIES::instance()->builder ()->build_supplier_admin (this, op, id ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NS_EventChannel::get_consumeradmin (CosNotifyChannelAdmin::AdminID id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminNotFound
                   ))
{
  TAO_NS_Object* ns_object = this->find (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  if (ns_object != 0)
    {
      CORBA::Object_var object = ns_object->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

      CosNotifyChannelAdmin::ConsumerAdmin_var ca_ret;

      ca_ret = CosNotifyChannelAdmin::ConsumerAdmin::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

      return ca_ret._retn ();
    }
  else
    ACE_THROW_RETURN (CosNotifyChannelAdmin::AdminNotFound (), CosNotifyChannelAdmin::ConsumerAdmin::_nil ());
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_NS_EventChannel::get_supplieradmin (CosNotifyChannelAdmin::AdminID id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminNotFound
                   ))
{
  TAO_NS_Object* ns_object = this->find (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  if (ns_object != 0)
    {
      CORBA::Object_var object = ns_object->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

      CosNotifyChannelAdmin::SupplierAdmin_var sa_ret;

      sa_ret = CosNotifyChannelAdmin::SupplierAdmin::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

      return sa_ret._retn ();
    }
  else
    ACE_THROW_RETURN (CosNotifyChannelAdmin::AdminNotFound (), CosNotifyChannelAdmin::SupplierAdmin::_nil ());
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_NS_EventChannel::get_all_consumeradmins (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotifyChannelAdmin::AdminIDSeq* seq_ptr;

  ACE_NEW_THROW_EX (seq_ptr,
                    CosNotifyChannelAdmin::AdminIDSeq (),
                    CORBA::NO_MEMORY ());

  CosNotifyChannelAdmin::AdminIDSeq_var seq (seq_ptr);

  seq->length (this->consumer_admin_id_list_.size ());

  TAO_NS_Object_Id_Seq::CONST_ITERATOR iter (this->consumer_admin_id_list_);

  TAO_NS_Object_Id* object_id;

  CORBA::ULong i = 0;
  for (iter.first (); iter.next (object_id); iter.advance (), ++i)
    seq[i] = *object_id;

  return seq._retn ();
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_NS_EventChannel::get_all_supplieradmins (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotifyChannelAdmin::AdminIDSeq* seq_ptr;

  ACE_NEW_THROW_EX (seq_ptr,
                    CosNotifyChannelAdmin::AdminIDSeq (),
                    CORBA::NO_MEMORY ());

  CosNotifyChannelAdmin::AdminIDSeq_var seq (seq_ptr);

  seq->length (this->supplier_admin_id_list_.size ());

  TAO_NS_Object_Id_Seq::CONST_ITERATOR iter (this->consumer_admin_id_list_);

  TAO_NS_Object_Id* object_id;

  CORBA::ULong i = 0;
  for (iter.first (); iter.next (object_id); iter.advance (), ++i)
    seq[i] = *object_id;

  return seq._retn ();
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
TAO_NS_EventChannel::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosEventChannelAdmin::ConsumerAdmin_var ca_ret;

  ca_ret = CosEventChannelAdmin::ConsumerAdmin::_narrow (this->default_consumer_admin_.in ()
                                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ConsumerAdmin::_nil ());

  return ca_ret._retn ();

}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_NS_EventChannel::for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosEventChannelAdmin::SupplierAdmin_var sa_ret =
    CosEventChannelAdmin::SupplierAdmin::_narrow (this->default_supplier_admin_.in ()
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosEventChannelAdmin::SupplierAdmin::_nil ());

  return sa_ret._retn ();
}
