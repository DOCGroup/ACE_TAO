// $Id$

#include "Proxy.h"

#if ! defined (__ACE_INLINE__)
#include "Proxy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Proxy, "$Id$")

#include "Peer.h"
#include "Proxy.h"
#include "Admin.h"
#include "EventChannel.h"
#include "EventChannelFactory.h"
#include "Notify_Service.h"

TAO_NS_Proxy::TAO_NS_Proxy (void)
  :updates_off_ (0)
{
  // Set initial proxy mode to broadcast.
  this->subscribed_types_.insert (TAO_NS_EventType::special ());
}

TAO_NS_Proxy::~TAO_NS_Proxy ()
{
}

void
TAO_NS_Proxy::type_added (const TAO_NS_EventType& added)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->added_.insert (added);
  this->removed_.remove (added);
}

void
TAO_NS_Proxy::type_removed (const TAO_NS_EventType& removed)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->removed_.insert (removed);
  this->added_.remove (removed);
}

CORBA::Boolean
TAO_NS_Proxy::check_filters (TAO_NS_Event_var &event ACE_ENV_ARG_DECL)
{
  // check if it passes the parent filter.
  CORBA::Boolean parent_val =
    this->parent_->filter_admin ().match (event ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Boolean val = 0;

  if (this->parent_->filter_operator () == CosNotifyChannelAdmin::AND_OP)
    {
      val = parent_val && this->filter_admin_.match (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  else
    {
      val = parent_val || this->filter_admin_.match (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return val;
}

CosNotification::EventTypeSeq*
TAO_NS_Proxy::obtain_types (CosNotifyChannelAdmin::ObtainInfoMode mode ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotification::EventTypeSeq_var event_type_seq;

  ACE_NEW_THROW_EX (event_type_seq,
                    CosNotification::EventTypeSeq (),
                    CORBA::NO_MEMORY ());

  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_, CORBA::INTERNAL ());
  ACE_CHECK_RETURN (event_type_seq._retn ());

  if (mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_OFF ||
      mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON)
    {
      this->added_.populate (event_type_seq);
    }

  if (mode == CosNotifyChannelAdmin::NONE_NOW_UPDATES_ON ||
      mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON)
    {
      this->updates_off_ = 0;
    }
  else
    {
      this->updates_off_ = 1;
    }

  return event_type_seq._retn ();
}

void
TAO_NS_Proxy::qos_changed (void)
{
  //Inform Peers of qos changes.
  TAO_NS_Peer* peer = this->peer ();

  if (peer != 0)
    peer->qos_changed (this->qos_properties_);
}
