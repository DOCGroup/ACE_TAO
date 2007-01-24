// $Id$

#include "orbsvcs/Notify/Structured/StructuredProxyPushConsumer.h"

ACE_RCSID(RT_Notify, TAO_Notify_StructuredProxyPushConsumer, "$Id$")

#include "ace/Bound_Ptr.h"
#include "ace/Auto_Ptr.h"
#include "tao/debug.h"
#include "orbsvcs/Notify/Structured/StructuredPushSupplier.h"
#include "orbsvcs/Notify/Structured/StructuredEvent.h"
#include "orbsvcs/Notify/AdminProperties.h"
#include "orbsvcs/Notify/Properties.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_StructuredProxyPushConsumer::TAO_Notify_StructuredProxyPushConsumer (void)
{
}

TAO_Notify_StructuredProxyPushConsumer::~TAO_Notify_StructuredProxyPushConsumer ()
{
}

void
TAO_Notify_StructuredProxyPushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

CosNotifyChannelAdmin::ProxyType
TAO_Notify_StructuredProxyPushConsumer::MyType (void)
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
  this->connect (supplier ACE_ENV_ARG_PARAMETER);
  this->self_change ();
}

void
TAO_Notify_StructuredProxyPushConsumer::push_structured_event (const CosNotification::StructuredEvent & notification ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventComm::Disconnected
                   ))
{
  // Check if we should proceed at all.
  if (this->admin_properties().reject_new_events () == 1
      && this->admin_properties().queue_full ())
    ACE_THROW (CORBA::IMP_LIMIT ());

  if (this->is_connected () == 0)
    {
      ACE_THROW (CosEventComm::Disconnected ());
    }

  TAO_Notify_StructuredEvent_No_Copy event (notification);
  this->push_i (&event ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredProxyPushConsumer::disconnect_structured_push_consumer (void)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_StructuredProxyPushConsumer::Ptr guard( this );
  this->destroy ();
  this->self_change ();
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
  if (attrs.load("PeerIOR", ior))
  {
    CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY
    {
      CosNotifyComm::StructuredPushSupplier_var ps = CosNotifyComm::StructuredPushSupplier::_nil();
      if ( ior.length() > 0 )
      {
        CORBA::Object_var obj = orb->string_to_object(ior.c_str() ACE_ENV_ARG_PARAMETER);
        ps = CosNotifyComm::StructuredPushSupplier::_unchecked_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
      }
      // minor hack: suppress generating subscription updates during reload.
      bool save_updates = this->updates_off_;
      this->updates_off_ = true;
      this->connect_structured_push_supplier(ps.in() ACE_ENV_ARG_PARAMETER);
      this->updates_off_ = save_updates;
    }
    ACE_CATCHANY
    {
      // if we can't reconnect to peer, tough...
    }
    ACE_ENDTRY;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
