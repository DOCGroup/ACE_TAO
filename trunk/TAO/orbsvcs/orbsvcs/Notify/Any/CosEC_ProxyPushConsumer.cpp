// $Id$

#include "CosEC_ProxyPushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "CosEC_ProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Notify, TAO_Notify_CosEC_ProxyPushConsumer, "$Id$")

#include "tao/debug.h"
#include "../AdminProperties.h"
#include "../Method_Request_Lookup.h"
#include "../Worker_Task.h"
#include "AnyEvent.h"
#include "PushSupplier.h"

TAO_Notify_CosEC_ProxyPushConsumer::TAO_Notify_CosEC_ProxyPushConsumer (void)
{
}

TAO_Notify_CosEC_ProxyPushConsumer::~TAO_Notify_CosEC_ProxyPushConsumer ()
{
}

void
TAO_Notify_CosEC_ProxyPushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_CosEC_ProxyPushConsumer::push (const CORBA::Any& any ACE_ENV_ARG_DECL)
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

  TAO_Notify_AnyEvent_No_Copy event (any);

  TAO_Notify_Method_Request_Lookup_No_Copy request (&event, this);

  this->worker_task ()->execute (request ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_CosEC_ProxyPushConsumer::connect_push_supplier (CosEventComm::PushSupplier_ptr push_supplier ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   ))
{
  // Convert Supplier to Base Type
  TAO_Notify_PushSupplier *supplier;
  ACE_NEW_THROW_EX (supplier,
                    TAO_Notify_PushSupplier (this),
                    CORBA::NO_MEMORY ());

  supplier->init (push_supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->connect (supplier ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_CosEC_ProxyPushConsumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
}

const char *
TAO_Notify_CosEC_ProxyPushConsumer::get_proxy_type_name (void) const
{
  return "ec_proxy_push_consumer";
}
