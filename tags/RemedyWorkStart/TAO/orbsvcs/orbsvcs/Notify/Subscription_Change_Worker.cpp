// $Id$

#include "orbsvcs/Notify/Subscription_Change_Worker.h"

#include "orbsvcs/Notify/Proxy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Subscription_Change_Worker::TAO_Notify_Subscription_Change_Worker (
  const CosNotification::EventTypeSeq & added,
  const CosNotification::EventTypeSeq & removed)
  : added_ (added), removed_ (removed)
{
}

TAO_Notify_Subscription_Change_Worker::~TAO_Notify_Subscription_Change_Worker ()
{
}

void
TAO_Notify_Subscription_Change_Worker::work (TAO_Notify_Proxy* proxy)
{
  proxy->admin_types_changed (this->added_, this->removed_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
