/* -*- C++ -*- */
/**
 *  @file Subscription_Change_Worker.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_SUBSCRIPTION_CHANGE_WORKER_H
#define TAO_SUBSCRIPTION_CHANGE_WORKER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/ESF/ESF_Worker.h"

class TAO_NS_Proxy;

/**
 * @class TAO_Subscription_Change_Worker
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_Subscription_Change_Worker : public TAO_ESF_Worker<TAO_NS_Proxy>
{
public:
  /// Constuctor
  TAO_NS_Subscription_Change_Worker (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed);

  /// Destructor
  ~TAO_NS_Subscription_Change_Worker ();

  ///= TAO_ESF_Worker method
  void work (TAO_NS_Proxy* proxy ACE_ENV_ARG_DECL);

protected:
  const CosNotification::EventTypeSeq & added_;
  const CosNotification::EventTypeSeq & removed_;
};

#if defined (__ACE_INLINE__)
#include "Subscription_Change_Worker.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_SUBSCRIPTION_CHANGE_WORKER_H */
