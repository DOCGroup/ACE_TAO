// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Sequence/SequenceProxyPushSupplier.h"
#include "tao/debug.h"
#include "orbsvcs/Notify/Sequence/SequencePushConsumer.h"
#include "orbsvcs/Notify/Properties.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_SequenceProxyPushSupplier::TAO_Notify_SequenceProxyPushSupplier (void)
{
}

TAO_Notify_SequenceProxyPushSupplier::~TAO_Notify_SequenceProxyPushSupplier ()
{
}

void
TAO_Notify_SequenceProxyPushSupplier::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_SequenceProxyPushSupplier::connect_sequence_push_consumer (CosNotifyComm::SequencePushConsumer_ptr push_consumer)
{
  // Convert Consumer to Base Type
  TAO_Notify_SequencePushConsumer* consumer;
  ACE_NEW_THROW_EX (consumer,
                    TAO_Notify_SequencePushConsumer (this),
                    CORBA::NO_MEMORY ());

  consumer->init (push_consumer);

  this->connect (consumer);
  this->self_change ();
}

void
TAO_Notify_SequenceProxyPushSupplier::disconnect_sequence_push_supplier (void)

{
  TAO_Notify_SequenceProxyPushSupplier::Ptr guard( this );
  this->destroy ();
  this->self_change ();
}

CosNotifyChannelAdmin::ProxyType
TAO_Notify_SequenceProxyPushSupplier::MyType (void)
{
  return CosNotifyChannelAdmin::PUSH_SEQUENCE;
}

const char *
TAO_Notify_SequenceProxyPushSupplier::get_proxy_type_name (void) const
{
  return "sequence_proxy_push_supplier";
}

void
TAO_Notify_SequenceProxyPushSupplier::validate ()
{
  TAO_Notify_Consumer* con = this->consumer ();
  if (con != 0 && ! con->is_alive (true))
  {
    if (TAO_debug_level > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) TAO_Notify_SequenceProxyPushSupplier::validate(%d)")
                  ACE_TEXT ("disconnecting \n"), this->id ()));
    }

    this->disconnect_sequence_push_supplier ();
  }
}


void
TAO_Notify_SequenceProxyPushSupplier::load_attrs (const TAO_Notify::NVPList& attrs)
{
  SuperClass::load_attrs(attrs);
  ACE_CString ior;
  if (attrs.load("PeerIOR", ior))
  {
    CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
    try
    {
      CosNotifyComm::SequencePushConsumer_var pc = CosNotifyComm::SequencePushConsumer::_nil();
      if (ior.length() > 0)
      {
        CORBA::Object_var obj = orb->string_to_object(ior.c_str());
        pc = CosNotifyComm::SequencePushConsumer::_unchecked_narrow(obj.in());
      }
      this->connect_sequence_push_consumer(pc.in());
    }
    catch (const CORBA::Exception&)
    {
      // if we can't reconnect, tough
    }
  }
}

void
TAO_Notify_SequenceProxyPushSupplier::configure(
  TAO_Notify_ConsumerAdmin & /*admin*/,
  CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/)
{
  // Nothing to do.
  // This virtual method was added to support Notification MC
}

TAO_END_VERSIONED_NAMESPACE_DECL
