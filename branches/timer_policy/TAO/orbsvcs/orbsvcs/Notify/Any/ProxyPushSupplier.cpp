// $Id$

#include "orbsvcs/Notify/Any/ProxyPushSupplier.h"
#include "tao/debug.h"
#include "orbsvcs/Notify/Any/PushConsumer.h"
#include "orbsvcs/Notify/Properties.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_ProxyPushSupplier::TAO_Notify_ProxyPushSupplier (void)
{
}

TAO_Notify_ProxyPushSupplier::~TAO_Notify_ProxyPushSupplier ()
{
}

void
TAO_Notify_ProxyPushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_ProxyPushSupplier::connect_any_push_consumer (CosEventComm::PushConsumer_ptr push_consumer)
{
  // Convert Consumer to Base Type
  TAO_Notify_PushConsumer* consumer;
  ACE_NEW_THROW_EX (consumer,
                    TAO_Notify_PushConsumer (this),
                    CORBA::NO_MEMORY ());

  consumer->init (push_consumer);

  this->connect (consumer);
  this->self_change ();
}

void
TAO_Notify_ProxyPushSupplier::disconnect_push_supplier (void)
{
  TAO_Notify_ProxyPushSupplier::Ptr guard( this );
  this->destroy ();
  this->self_change ();
}

CosNotifyChannelAdmin::ProxyType
TAO_Notify_ProxyPushSupplier::MyType (void)
{
  return CosNotifyChannelAdmin::PUSH_ANY;
}

const char *
TAO_Notify_ProxyPushSupplier::get_proxy_type_name (void) const
{
  return "proxy_push_supplier";
}

void
TAO_Notify_ProxyPushSupplier::validate ()
{
  TAO_Notify_Consumer* con = this->consumer ();
  if (con != 0 && ! con->is_alive (true))
  {
    if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) TAO_Notify_ProxyPushSupplier::validate(%d)")
                  ACE_TEXT ("disconnecting \n"), this->id ()));
    }

    this->disconnect_push_supplier ();
  }
}

void
TAO_Notify_ProxyPushSupplier::load_attrs (const TAO_Notify::NVPList& attrs)
{
  SuperClass::load_attrs(attrs);
  ACE_CString ior;
  if (attrs.load("PeerIOR", ior))
    {
      CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
      try
        {
          CosNotifyComm::PushConsumer_var pc =
            CosNotifyComm::PushConsumer::_nil();
          if (ior.length() > 0)
            {
              CORBA::Object_var obj =
                orb->string_to_object(ior.c_str());
              pc = CosNotifyComm::PushConsumer::_unchecked_narrow(obj.in());
            }
          this->connect_any_push_consumer(pc.in());
        }
      catch (const CORBA::Exception&)
        {
          // if we can't reconnect, tough...
        }
    }
}

void
TAO_Notify_ProxyPushSupplier::configure(
  TAO_Notify_ConsumerAdmin & /*admin*/,
  CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/)
{
  // presently nothing to do here.
  // this method was added to support NotificationMC
}



TAO_END_VERSIONED_NAMESPACE_DECL
