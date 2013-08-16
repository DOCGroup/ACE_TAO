// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Structured/StructuredProxyPushConsumer.h"
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
{
  return CosNotifyChannelAdmin::PUSH_STRUCTURED;
}

void
TAO_Notify_StructuredProxyPushConsumer::connect_structured_push_supplier (CosNotifyComm::StructuredPushSupplier_ptr push_supplier)
{
  // Convert Supplier to Base Type
  TAO_Notify_StructuredPushSupplier * supplier;
  ACE_NEW_THROW_EX (supplier,
                    TAO_Notify_StructuredPushSupplier (this),
                    CORBA::NO_MEMORY ());

  supplier->init (push_supplier);
  this->connect (supplier);
  this->self_change ();
}

void
TAO_Notify_StructuredProxyPushConsumer::push_structured_event (const CosNotification::StructuredEvent & notification)
{
  // Check if we should proceed at all.
  if (this->admin_properties().reject_new_events () == 1
      && this->admin_properties().queue_full ())
    throw CORBA::IMP_LIMIT ();

  if (this->is_connected () == 0)
    {
      throw CosEventComm::Disconnected ();
    }

  TAO_Notify_StructuredEvent_No_Copy event (notification);
  this->push_i (&event);
}

void
TAO_Notify_StructuredProxyPushConsumer::disconnect_structured_push_consumer (void)
{
  TAO_Notify_StructuredProxyPushConsumer::Ptr guard( this );
  this->destroy ();
  this->send_deletion_change ();
}

const char *
TAO_Notify_StructuredProxyPushConsumer::get_proxy_type_name (void) const
{
  return "structured_proxy_push_consumer";
}

void
TAO_Notify_StructuredProxyPushConsumer::validate ()
{
  TAO_Notify_Supplier* sup = this->supplier ();
  if (sup != 0 && ! sup->is_alive (true))
  {
    if (TAO_debug_level > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) TAO_Notify_StructuredProxyPushConsumer::validate(%d)")
                  ACE_TEXT("disconnecting \n"), this->id ()));
    }

    this->disconnect_structured_push_consumer ();
  }
}

void
TAO_Notify_StructuredProxyPushConsumer::load_attrs (const TAO_Notify::NVPList& attrs)
{
  SuperClass::load_attrs(attrs);
  ACE_CString ior;
  if (attrs.load("PeerIOR", ior))
  {
    CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
    try
    {
      CosNotifyComm::StructuredPushSupplier_var ps = CosNotifyComm::StructuredPushSupplier::_nil();
      if ( ior.length() > 0 )
      {
        CORBA::Object_var obj = orb->string_to_object(ior.c_str());
        ps = CosNotifyComm::StructuredPushSupplier::_unchecked_narrow(obj.in());
      }
      // minor hack: suppress generating subscription updates during reload.
      bool save_updates = this->updates_off_;
      this->updates_off_ = true;
      this->connect_structured_push_supplier(ps.in());
      this->updates_off_ = save_updates;
    }
    catch (const CORBA::Exception&)
    {
      // if we can't reconnect to peer, tough...
    }
  }
}

void
TAO_Notify_StructuredProxyPushConsumer::configure(
  TAO_Notify_SupplierAdmin & /*admin*/,
  CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/)
{
  // Nothing to do.
  // This virtual method was added to support Notification MC
}



TAO_END_VERSIONED_NAMESPACE_DECL
