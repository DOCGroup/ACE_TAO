// $Id$

#include "ProxySupplier.h"
#include "Event_Manager.h"
#include "Admin.h"

#if ! defined (__ACE_INLINE__)
#include "ProxySupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_ProxySupplier, "$Id$")

#include "Method_Request_Dispatch_No_Filtering.h"

TAO_NS_ProxySupplier::TAO_NS_ProxySupplier (void)
  :consumer_ (0)
{
}

TAO_NS_ProxySupplier::~TAO_NS_ProxySupplier ()
{
}

void
TAO_NS_ProxySupplier::init_ps (ACE_ENV_SINGLE_ARG_DECL)
{
}

TAO_NS_Peer*
TAO_NS_ProxySupplier:: peer (void)
{
  return this->consumer ();
}

void
TAO_NS_ProxySupplier::connect (TAO_NS_Consumer *consumer ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosEventChannelAdmin::AlreadyConnected
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  if (this->is_connected ())
    ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
  else
    {
      consumer_ = consumer;

      consumer->event_dispatch_observer (this->event_manager_->event_dispatch_observer ());
      consumer->updates_dispatch_observer (this->event_manager_->updates_dispatch_observer ());

      this->parent_->subscribed_types (this->subscribed_types_); // get the parents subscribed types.

      event_manager_->subscribe (this, this->subscribed_types_ ACE_ENV_ARG_PARAMETER);
    }
}

void
TAO_NS_ProxySupplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  event_manager_->un_subscribe (this, this->subscribed_types_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_->_decr_refcnt ();
}

void
TAO_NS_ProxySupplier::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);

  this->inherited::shutdown (ACE_ENV_ARG_PARAMETER);

  //@@ inform the consumer that its disconnected?
}

void
TAO_NS_ProxySupplier::push (TAO_NS_Event_var &event)
{
  TAO_NS_Method_Request_Dispatch request (event, this);

  this->worker_task ()->exec (request);
}

void
TAO_NS_ProxySupplier::push_no_filtering (TAO_NS_Event_var &event)
{
  TAO_NS_Method_Request_Dispatch_No_Filtering request (event, this);

  this->worker_task ()->exec (request);
}
