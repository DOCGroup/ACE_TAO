// $Id$

#include "Subscription_Change_Worker.h"

#if ! defined (__ACE_INLINE__)
#include "Subscription_Change_Worker.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Subscription_Change_Worker, "$Id$")

TAO_Notify_Subscription_Change_Worker::TAO_Notify_Subscription_Change_Worker (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed)
:added_ (added), removed_ (removed)
{
}

TAO_Notify_Subscription_Change_Worker::~TAO_Notify_Subscription_Change_Worker ()
{
}
