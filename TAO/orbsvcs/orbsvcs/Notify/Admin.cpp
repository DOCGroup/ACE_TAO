// $Id$

#include "Admin.h"
#include "EventChannel.h"
#include "Proxy.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"

#if ! defined (__ACE_INLINE__)
#include "Admin.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Admin, "$Id$")

TAO_NS_Admin::TAO_NS_Admin (void)
  :filter_operator_ (CosNotifyChannelAdmin::OR_OP)
{
  // Initialize all Admin objects to initially be subscribed for all events.
  // This is a reasonable default and is ewquired to allow Cos Event consumers/suppliers to send/receive events,
  this->subscribed_types_.insert (TAO_NS_EventType::special ());
}

TAO_NS_Admin::~TAO_NS_Admin ()
{
}

void
TAO_NS_Admin::subscribed_types (TAO_NS_EventTypeSeq& subscribed_types)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());
  ACE_CHECK;

  // copy
  subscribed_types = this->subscribed_types_;
}
