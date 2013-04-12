// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Sequence/SequenceProxyPushConsumer.h"
#include "tao/debug.h"
#include "orbsvcs/Notify/Sequence/SequencePushSupplier.h"
#include "orbsvcs/Notify/AdminProperties.h"
#include "orbsvcs/Notify/Structured/StructuredEvent.h"
#include "orbsvcs/Notify/Properties.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_SequenceProxyPushConsumer::TAO_Notify_SequenceProxyPushConsumer (void)
:pacing_interval_ (CosNotification::PacingInterval)
{
}

TAO_Notify_SequenceProxyPushConsumer::~TAO_Notify_SequenceProxyPushConsumer ()
{
}

void
TAO_Notify_SequenceProxyPushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

CosNotifyChannelAdmin::ProxyType
TAO_Notify_SequenceProxyPushConsumer::MyType (void)
{
  return CosNotifyChannelAdmin::PUSH_SEQUENCE;
}

void
TAO_Notify_SequenceProxyPushConsumer::connect_sequence_push_supplier (CosNotifyComm::SequencePushSupplier_ptr push_supplier)
{
  // Convert Supplier to Base Type
  TAO_Notify_SequencePushSupplier *supplier;
  ACE_NEW_THROW_EX (supplier,
                    TAO_Notify_SequencePushSupplier (this),
                    CORBA::NO_MEMORY ());

  supplier->init (push_supplier);

  this->connect (supplier);
  this->self_change ();
}

void
TAO_Notify_SequenceProxyPushConsumer::push_structured_events (const CosNotification::EventBatch& event_batch)
{
  // Check if we should proceed at all.
  if (this->admin_properties().reject_new_events () == 1 && this->admin_properties().queue_full ())
    throw CORBA::IMP_LIMIT ();

  if (this->is_connected () == 0)
    {
      throw CosEventComm::Disconnected ();
    }

  for (CORBA::ULong i = 0; i < event_batch.length (); ++i)
    {
      const CosNotification::StructuredEvent& notification = event_batch[i];

      TAO_Notify_StructuredEvent_No_Copy event (notification);
      this->push_i (&event);
    }
}

void
TAO_Notify_SequenceProxyPushConsumer::disconnect_sequence_push_consumer (void)
{
  TAO_Notify_SequenceProxyPushConsumer::Ptr guard( this );
  this->destroy ();
  this->self_change ();
}

const char *
TAO_Notify_SequenceProxyPushConsumer::get_proxy_type_name (void) const
{
  return "sequence_proxy_push_consumer";
}

void
TAO_Notify_SequenceProxyPushConsumer::validate ()
{
  TAO_Notify_Supplier* sup = this->supplier ();
  if (sup != 0 && ! sup->is_alive (true))
  {
    if (TAO_debug_level > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) TAO_Notify_SequenceProxyPushConsumer::validate(%d)")
                  ACE_TEXT ("disconnecting \n"), this->id ()));
    }

    this->disconnect_sequence_push_consumer ();
  }
}

void
TAO_Notify_SequenceProxyPushConsumer::load_attrs (const TAO_Notify::NVPList& attrs)
{
  SuperClass::load_attrs(attrs);
  ACE_CString ior;
  if (attrs.load("PeerIOR", ior))
  {
    CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
    try
    {
      CosNotifyComm::SequencePushSupplier_var ps = CosNotifyComm::SequencePushSupplier::_nil();
      if ( ior.length() > 0 )
      {
        CORBA::Object_var obj = orb->string_to_object(ior.c_str());
        ps = CosNotifyComm::SequencePushSupplier::_unchecked_narrow(obj.in());
      }
      // minor hack: suppress generating subscription updates during reload.
      bool save_updates = this->updates_off_;
      this->updates_off_ = true;
      this->connect_sequence_push_supplier(ps.in());
      this->updates_off_ = save_updates;
    }
    catch (const CORBA::Exception&)
    {
      ACE_ASSERT(0);
    }
  }
}

void
TAO_Notify_SequenceProxyPushConsumer::configure(
  TAO_Notify_SupplierAdmin & /*admin*/,
  CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/)
{
  // Nothing to do.
  // This virtual method was added to support Notification MC
}


TAO_END_VERSIONED_NAMESPACE_DECL
