// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Structured/StructuredProxyPushSupplier.h"

#include "tao/PortableServer/Servant_Base.h"
#include "tao/debug.h"

#include "orbsvcs/Notify/Structured/StructuredPushConsumer.h"
#include "orbsvcs/Notify/Properties.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_StructuredProxyPushSupplier::TAO_Notify_StructuredProxyPushSupplier (void)
{
}

TAO_Notify_StructuredProxyPushSupplier::~TAO_Notify_StructuredProxyPushSupplier ()
{
}

void
TAO_Notify_StructuredProxyPushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}

CosNotifyChannelAdmin::ProxyType
TAO_Notify_StructuredProxyPushSupplier::MyType (void)
{
  return CosNotifyChannelAdmin::PUSH_STRUCTURED;
}

void
TAO_Notify_StructuredProxyPushSupplier::connect_structured_push_consumer (CosNotifyComm::StructuredPushConsumer_ptr push_consumer)
{
  // Convert Consumer to Base Type
  TAO_Notify_StructuredPushConsumer* consumer;
  ACE_NEW_THROW_EX (consumer,
                    TAO_Notify_StructuredPushConsumer (this),
                    CORBA::NO_MEMORY ());

  consumer->init (push_consumer);

  this->connect (consumer);
  this->self_change ();
}

void
TAO_Notify_StructuredProxyPushSupplier::disconnect_structured_push_supplier (void)

{
  TAO_Notify_StructuredProxyPushSupplier::Ptr guard( this );
  this->destroy ();
  this->send_deletion_change ();
}

const char *
TAO_Notify_StructuredProxyPushSupplier::get_proxy_type_name (void) const
{
  return "structured_proxy_push_supplier";
}

void
TAO_Notify_StructuredProxyPushSupplier::validate ()
{
  TAO_Notify_Consumer* con = this->consumer ();
  if (con != 0 && ! con->is_alive (true))
  {
    if (TAO_debug_level > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) TAO_Notify_StructuredProxyPushSupplier::validate(%d)")
                  ACE_TEXT ("disconnecting \n"), this->id ()));
    }

    this->disconnect_structured_push_supplier ();
  }
}

void
TAO_Notify_StructuredProxyPushSupplier::load_attrs (const TAO_Notify::NVPList& attrs)
{
  SuperClass::load_attrs(attrs);
  ACE_CString ior;
  if (attrs.load("PeerIOR", ior))
  {
    CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
    try
    {
      CosNotifyComm::StructuredPushConsumer_var pc = CosNotifyComm::StructuredPushConsumer::_nil();
      if (ior.length() > 0)
      {
        CORBA::Object_var obj = orb->string_to_object(ior.c_str());
        pc = CosNotifyComm::StructuredPushConsumer::_unchecked_narrow(obj.in());
      }
      this->connect_structured_push_consumer(pc.in());
    }
    catch (const CORBA::Exception&)
    {
      // if we can't connect... tough
    }
  }
}

void
TAO_Notify_StructuredProxyPushSupplier::configure(
  TAO_Notify_ConsumerAdmin & /*admin*/,
  CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/)
{
  // Nothing to do.
  // This virtual method was added to support Notification MC
}

TAO_END_VERSIONED_NAMESPACE_DECL
