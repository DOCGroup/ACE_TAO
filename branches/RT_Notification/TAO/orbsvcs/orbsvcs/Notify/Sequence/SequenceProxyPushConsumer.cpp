// $Id$

#include "SequenceProxyPushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "SequenceProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_SequenceProxyPushConsumer, "$id$")

#include "tao/debug.h"
#include "SequencePushSupplier.h"
#include "../AdminProperties.h"
#include "../Method_Request_Lookup.h"
#include "../Worker_Task.h"
#include "../Structured/StructuredEvent.h"

TAO_NS_SequenceProxyPushConsumer::TAO_NS_SequenceProxyPushConsumer (void)
:pacing_interval_ (CosNotification::PacingInterval)
{
}

TAO_NS_SequenceProxyPushConsumer::~TAO_NS_SequenceProxyPushConsumer ()
{
}

void
TAO_NS_SequenceProxyPushConsumer::release (void)
{
  if (this->supplier_)
    this->supplier_->release ();

  delete this;
  //@@ inform factory
}

void
TAO_NS_SequenceProxyPushConsumer::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "In TAO_NS_SequenceProxyPushConsumer::destroy \n"));

  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return;

  ACE_CHECK;
}

CosNotifyChannelAdmin::ProxyType
TAO_NS_SequenceProxyPushConsumer::MyType (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::PUSH_SEQUENCE;
}

void
TAO_NS_SequenceProxyPushConsumer::connect_sequence_push_supplier (CosNotifyComm::SequencePushSupplier_ptr push_supplier ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   ))
{
  // Convert Supplier to Base Type
  TAO_NS_SequencePushSupplier *supplier;
  ACE_NEW_THROW_EX (supplier,
                    TAO_NS_SequencePushSupplier (this),
                    CORBA::NO_MEMORY ());

  supplier->init (push_supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->connect (supplier ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_SequenceProxyPushConsumer::push_structured_events (const CosNotification::EventBatch& event_batch ACE_ENV_ARG_DECL)
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

      TAO_NS_StructuredEvent_No_Copy event (notification);

      TAO_NS_Method_Request_Lookup_No_Copy request (&event, this);

      this->worker_task ()->execute (request ACE_ENV_ARG_PARAMETER);
    }
}

void
TAO_NS_SequenceProxyPushConsumer::disconnect_sequence_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
}
