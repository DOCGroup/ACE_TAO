/* -*- C++ -*- */
/**
 *  @file Dispatch_Observer_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_DISPATCH_OBSERVER_T_H
#define TAO_NS_DISPATCH_OBSERVER_T_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//#include "types.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

/**
 * @class TAO_NS_Dispatch_Observer_T
 *
 * @brief An observer class that is notified of the success or failure of event/update diapatches.
 *        If a dispatch fails, the PEER is added to a pending list. This list is obtained by the Pending Worker to retry delivery of
 *        Events/Updates. When the max_retries are reached, the observer destroys the proxy.
 *
 */
template <class PEER>
class TAO_Notify_Export TAO_NS_Dispatch_Observer_T
{
public:
  typedef TAO_ESF_Proxy_Collection<PEER> PEER_COLLECTION;

  /// Constuctor
  TAO_NS_Dispatch_Observer_T (void);

  /// Destructor
  ~TAO_NS_Dispatch_Observer_T ();

  /// Init
  void init (ACE_ENV_SINGLE_ARG_DECL);

  void max_retry_attempts (int max_retry_attempts);

  /// Enqueue this peer.
  void enqueue (PEER* peer  ACE_ENV_ARG_DECL);

  /// Dispatch failure notice.
  void dispatch_failure (PEER* peer, int retry_count ACE_ENV_ARG_DECL);

  /// Dispatch success notice.
  void dispatch_success (PEER* peer ACE_ENV_ARG_DECL);

  /// Get peer collection.
  PEER_COLLECTION* peer_collection (void);

protected:
  /// Max retry attempts.
  int max_retry_attempts_;

  /// Pending list.
  PEER_COLLECTION* peer_collection_;
};

#if defined (__ACE_INLINE__)
#include "Dispatch_Observer_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Dispatch_Observer_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Dispatch_Observer_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_NS_DISPATCH_OBSERVER_T_H */
