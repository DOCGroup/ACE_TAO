// $Id$

#include "StructuredProxyPushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_Notify_StructuredProxyPushConsumer, "$Id$")

#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Auto_Ptr.h"
#include "tao/debug.h"
#include "StructuredPushSupplier.h"
#include "StructuredEvent.h"
#include "../AdminProperties.h"
#include "../Properties.h"

TAO_Notify_StructuredProxyPushConsumer::TAO_Notify_StructuredProxyPushConsumer (void)
{
}

TAO_Notify_StructuredProxyPushConsumer::~TAO_Notify_StructuredProxyPushConsumer ()
{
}

void
TAO_Notify_StructuredProxyPushConsumer::release (void)
{
  if (this->supplier_)
    this->supplier_->release ();

  delete this;
  //@@ inform factory
}

CosNotifyChannelAdmin::ProxyType
TAO_Notify_StructuredProxyPushConsumer::MyType (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::PUSH_STRUCTURED;
}

void
TAO_Notify_StructuredProxyPushConsumer::connect_structured_push_supplier (CosNotifyComm::StructuredPushSupplier_ptr push_supplier ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   ))
{
  // Convert Supplier to Base Type
  TAO_Notify_StructuredPushSupplier * supplier;
  ACE_NEW_THROW_EX (supplier,
                    TAO_Notify_StructuredPushSupplier (this),
                    CORBA::NO_MEMORY ());

  supplier->init (push_supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->connect (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_StructuredProxyPushConsumer::push_structured_event (const CosNotification::StructuredEvent & notification ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventComm::Disconnected
                   ))
{
  // Check if we should proceed at all.
  if (this->admin_properties_->reject_new_events () == 1
      && this->admin_properties_->queue_full ())
    ACE_THROW (CORBA::IMP_LIMIT ());

  if (this->is_connected () == 0)
    {
      ACE_THROW (CosEventComm::Disconnected ());
    }

  TAO_Notify_StructuredEvent_No_Copy event (notification);
  this->push_i (&event ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredProxyPushConsumer::disconnect_structured_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);

}
const char *
TAO_Notify_StructuredProxyPushConsumer::get_proxy_type_name (void) const
{
  return "structured_proxy_push_consumer";
}

void
TAO_Notify_StructuredProxyPushConsumer::load_attrs (const TAO_Notify::NVPList& attrs)
{
  SuperClass::load_attrs(attrs);
  ACE_CString ior;
  if (attrs.load("PeerIOR", ior) && ior.length() > 0)
  {
    CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY
    {
      CORBA::Object_var obj = orb->string_to_object(ior.c_str() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CosNotifyComm::StructuredPushSupplier_var ps =
        CosNotifyComm::StructuredPushSupplier::_unchecked_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      // minor hack: suppress generating subscription updates during reload.
      bool save_updates = this->updates_off_;
      this->updates_off_ = true;
      this->connect_structured_push_supplier(ps.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->updates_off_ = save_updates;
    }
    ACE_CATCHANY
    {
      // if we can't reconnect to peer, tough...
    }
    ACE_ENDTRY;
  }
}

void
TAO_Notify_StructuredProxyPushConsumer::init (TAO_Notify_SupplierAdmin *supplier_admin)
{
  TAO_Notify_ProxyConsumer::init (supplier_admin);
}
