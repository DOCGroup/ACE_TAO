// -*- C++ -*-

/**
 *  @file Subscription_Change_Worker.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_SUBSCRIPTION_CHANGE_WORKER_H
#define TAO_SUBSCRIPTION_CHANGE_WORKER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/ESF/ESF_Worker.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_Proxy;

/**
 * @class TAO_Subscription_Change_Worker
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Subscription_Change_Worker : public TAO_ESF_Worker<TAO_Notify_Proxy>
{
public:
  /// Constructor
  TAO_Notify_Subscription_Change_Worker (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed);

  /// Destructor
  virtual ~TAO_Notify_Subscription_Change_Worker ();

  ///= TAO_ESF_Worker method
  void work (TAO_Notify_Proxy* proxy);

protected:
  const CosNotification::EventTypeSeq & added_;
  const CosNotification::EventTypeSeq & removed_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_SUBSCRIPTION_CHANGE_WORKER_H */
