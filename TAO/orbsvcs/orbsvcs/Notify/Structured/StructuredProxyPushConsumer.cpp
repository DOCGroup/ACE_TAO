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
#include "../Method_Request_Lookup.h"
#include "../Worker_Task.h"

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

void
TAO_Notify_StructuredProxyPushConsumer::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "In TAO_Notify_StructuredProxyPushConsumer::destroy \n"));

  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return;

  ACE_CHECK;
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

  TAO_Notify_Method_Request_Lookup_No_Copy request (&event, this);

  this->worker_task ()->execute (request ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredProxyPushConsumer::disconnect_structured_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
}
