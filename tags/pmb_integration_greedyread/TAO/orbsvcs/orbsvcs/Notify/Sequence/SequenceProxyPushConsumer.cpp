// $Id$

#include "SequenceProxyPushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "SequenceProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Notify, TAO_Notify_SequenceProxyPushConsumer, "$Id$")

#include "tao/debug.h"
#include "SequencePushSupplier.h"
#include "../AdminProperties.h"
#include "../Method_Request_Lookup.h"
#include "../Worker_Task.h"
#include "../Structured/StructuredEvent.h"

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
  if (this->supplier_)
    this->supplier_->release ();

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
}

void
TAO_Notify_SequenceProxyPushConsumer::push_structured_events (const CosNotification::EventBatch& event_batch ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventComm::Disconnected
                   ))
{
  // Check if we should proceed at all.
  if (this->admin_properties_->reject_new_events () == 1 && this->admin_properties_->queue_full ())
    ACE_THROW (CORBA::IMP_LIMIT ());

  if (this->is_connected () == 0)
    {
      ACE_THROW (CosEventComm::Disconnected ());
    }

  for (CORBA::ULong i = 0; i < event_batch.length (); ++i)
    {
      const CosNotification::StructuredEvent& notification = event_batch[i];

      TAO_Notify_StructuredEvent_No_Copy event (notification);

      TAO_Notify_Method_Request_Lookup_No_Copy request (&event, this);

      this->worker_task ()->execute (request ACE_ENV_ARG_PARAMETER);
    }
}

void
TAO_Notify_SequenceProxyPushConsumer::disconnect_sequence_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
}
