// $Id$

#include "ProxyConsumer.h"
#include "Supplier.h"
#include "Admin.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "ProxyConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_ProxyConsumer, "$Id$")

TAO_NS_ProxyConsumer::TAO_NS_ProxyConsumer (void)
  :supplier_ (0)
{
}

TAO_NS_ProxyConsumer::~TAO_NS_ProxyConsumer ()
{
}

TAO_NS_Peer*
TAO_NS_ProxyConsumer::peer (void)
{
  return this->supplier ();
}

void
TAO_NS_ProxyConsumer::connect (TAO_NS_Supplier *supplier ACE_ENV_ARG_DECL)
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
      supplier_ = supplier;

      supplier->updates_dispatch_observer (this->event_manager_->updates_dispatch_observer ());

      this->parent_->subscribed_types (this->subscribed_types_); // get the parents subscribed types.

      event_manager_->publish (this, this->subscribed_types_ ACE_ENV_ARG_PARAMETER);
    }
}

void
TAO_NS_ProxyConsumer::disconnect (void)
{
  event_manager_->un_publish (this, this->subscribed_types_ ACE_ENV_ARG_PARAMETER);

  this->supplier_->_decr_refcnt ();
}

void
TAO_NS_ProxyConsumer::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);

  this->inherited::shutdown (ACE_ENV_ARG_PARAMETER);

  //@@ inform the supplier that its disconnected?
}

void
TAO_NS_ProxyConsumer::push (TAO_NS_Event_var &event)
{
  TAO_NS_Method_Request_Lookup request (event, this, this->event_manager_->consumer_map ());

  this->worker_task ()->exec (request);
}
