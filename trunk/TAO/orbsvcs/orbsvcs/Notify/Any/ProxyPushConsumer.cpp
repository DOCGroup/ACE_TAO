// $Id$

#include "ProxyPushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "ProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_ProxyPushConsumer, "$id$")

#include "tao/debug.h"
#include "../AdminProperties.h"
#include "../Method_Request_Lookup.h"
#include "../Worker_Task.h"
#include "AnyEvent.h"
#include "PushSupplier.h"

TAO_NS_ProxyPushConsumer::TAO_NS_ProxyPushConsumer (void)
{
}

TAO_NS_ProxyPushConsumer::~TAO_NS_ProxyPushConsumer ()
{
}

void
TAO_NS_ProxyPushConsumer::release (void)
{
  if (this->supplier_)
    this->supplier_->release ();

  delete this;
  //@@ inform factory
}

void
TAO_NS_ProxyPushConsumer::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "In TAO_NS_ProxyPushConsumer::destroy \n"));

  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return;

  ACE_CHECK;
}

CosNotifyChannelAdmin::ProxyType
TAO_NS_ProxyPushConsumer::MyType (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::PUSH_ANY;
}

void
TAO_NS_ProxyPushConsumer::push (TAO_NS_Event_var &/*event*/)
{
  // This should never be called.
  ACE_ASSERT (1);
}

void
TAO_NS_ProxyPushConsumer::push (const CORBA::Any& any ACE_ENV_ARG_DECL)
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

  TAO_NS_AnyEvent_No_Copy event (any);

  TAO_NS_Method_Request_Lookup_No_Copy request (&event, this);

  this->worker_task ()->execute (request ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_ProxyPushConsumer::connect_any_push_supplier (CosEventComm::PushSupplier_ptr push_supplier ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   ))
{
  // Convert Supplier to Base Type
  TAO_NS_PushSupplier *supplier;
  ACE_NEW_THROW_EX (supplier,
                    TAO_NS_PushSupplier (this),
                    CORBA::NO_MEMORY ());

  supplier->init (push_supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->connect (supplier ACE_ENV_ARG_PARAMETER);
}

void TAO_NS_ProxyPushConsumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
}
