// $Id$

#include "PushConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "PushConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_PushConsumer, "$id$")

#include "ace/Refcounted_Auto_Ptr.h"
#include "orbsvcs/CosEventCommC.h"
#include "../Event.h"

TAO_Notify_PushConsumer::TAO_Notify_PushConsumer (TAO_Notify_ProxySupplier* proxy)
  :TAO_Notify_Consumer (proxy)
{
}

TAO_Notify_PushConsumer::~TAO_Notify_PushConsumer ()
{
}

void
TAO_Notify_PushConsumer::init (CosEventComm::PushConsumer_ptr push_consumer ACE_ENV_ARG_DECL)
{
  this->push_consumer_ = CosEventComm::PushConsumer::_duplicate (push_consumer);

  ACE_TRY
    {
      this->publish_ = CosNotifyComm::NotifyPublish::_narrow (push_consumer ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // _narrow failed which probably means the interface is CosEventComm type.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_PushConsumer::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_PushConsumer::push_i (const TAO_Notify_Event* event ACE_ENV_ARG_DECL)
{
  event->push (this ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_PushConsumer::push_i (const TAO_Notify_Event_var& event ACE_ENV_ARG_DECL)
{
  event->push (this ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_PushConsumer::push (const CORBA::Any& payload ACE_ENV_ARG_DECL)
{
  this->push_consumer_->push (payload ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_PushConsumer::push (const CosNotification::StructuredEvent& event ACE_ENV_ARG_DECL)
{
  CORBA::Any any;

  TAO_Notify_Event::translate (event, any);

  this->push_consumer_->push (any ACE_ENV_ARG_PARAMETER);
}
