// $Id$

#include "orbsvcs/Notify/Sequence/SequenceProxyPushConsumer.h"

ACE_RCSID (Notify, TAO_Notify_SequenceProxyPushConsumer, "$Id$")

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
TAO_Notify_SequenceProxyPushConsumer::MyType (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::PUSH_SEQUENCE;
}

void
TAO_Notify_SequenceProxyPushConsumer::connect_sequence_push_supplier (CosNotifyComm::SequencePushSupplier_ptr push_supplier ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   ))
{
  // Convert Supplier to Base Type
  TAO_Notify_SequencePushSupplier *supplier;
  ACE_NEW_THROW_EX (supplier,
                    TAO_Notify_SequencePushSupplier (this),
                    CORBA::NO_MEMORY ());

  supplier->init (push_supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->connect (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_SequenceProxyPushConsumer::push_structured_events (const CosNotification::EventBatch& event_batch ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventComm::Disconnected
                   ))
{
  // Check if we should proceed at all.
  if (this->admin_properties().reject_new_events () == 1 && this->admin_properties().queue_full ())
    ACE_THROW (CORBA::IMP_LIMIT ());

  if (this->is_connected () == 0)
    {
      ACE_THROW (CosEventComm::Disconnected ());
    }

  for (CORBA::ULong i = 0; i < event_batch.length (); ++i)
    {
      const CosNotification::StructuredEvent& notification = event_batch[i];

      TAO_Notify_StructuredEvent_No_Copy event (notification);
      this->push_i (&event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_Notify_SequenceProxyPushConsumer::disconnect_sequence_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_SequenceProxyPushConsumer::Ptr guard( this );
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

const char *
TAO_Notify_SequenceProxyPushConsumer::get_proxy_type_name (void) const
{
  return "sequence_proxy_push_consumer";
}

void
TAO_Notify_SequenceProxyPushConsumer::load_attrs (const TAO_Notify::NVPList& attrs)
{
  SuperClass::load_attrs(attrs);
  ACE_CString ior;
  if (attrs.load("PeerIOR", ior))
  {
    CORBA::ORB_var orb = TAO_Notify_PROPERTIES::instance()->orb();
    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY
    {
      CosNotifyComm::SequencePushSupplier_var ps = CosNotifyComm::SequencePushSupplier::_nil();
      if ( ior.length() > 0 )
      {
        CORBA::Object_var obj = orb->string_to_object(ior.c_str() ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        ps = CosNotifyComm::SequencePushSupplier::_unchecked_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
      // minor hack: suppress generating subscription updates during reload.
      bool save_updates = this->updates_off_;
      this->updates_off_ = true;
      this->connect_sequence_push_supplier(ps.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->updates_off_ = save_updates;
    }
    ACE_CATCHANY
    {
      ACE_ASSERT(0);
    }
    ACE_ENDTRY;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
