/* -*- C++ -*- */
/**
 *  @file Peer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PEER_H
#define TAO_NS_PEER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"
#include "EventTypeSeq.h"
#include "Types.h"

/**
 * @class TAO_NS_Peer
 *
 * @brief Base class for Supplier and Consumer classes.
 *        This object delegates its reference count to its creator proxy object.
 *
 */
class TAO_Notify_Export TAO_NS_Peer
{
public:
  /// Constuctor
  TAO_NS_Peer (void);

  /// Destructor
  ~TAO_NS_Peer ();

  void updates_dispatch_observer (TAO_NS_Updates_Dispatch_Observer* updates_dispatch_observer);

  /// Access Proxy.
  virtual TAO_NS_Proxy* proxy (void) = 0;

  /// This method sigantures deliberately match the RefCounting methods required for ESF Proxy
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  ///=Subscription change

  /// Subscription type added
  void type_added (const TAO_NS_EventType& added);

  /// Subscription type removed
  void type_removed (const TAO_NS_EventType& removed);

  /// Dispatch Pending.
  void dispatch_pending (ACE_ENV_SINGLE_ARG_DECL);

protected:
  // Dispatch updates implementation.
  virtual void dispatch_updates_i (const TAO_NS_EventTypeSeq & added,
                                   const TAO_NS_EventTypeSeq & removed
                                   ACE_ENV_ARG_DECL) = 0;

  ///= Data Members

  /// The mutex to serialize access to state variables.
  TAO_SYNCH_MUTEX lock_;

  /// Types added.
  TAO_NS_EventTypeSeq added_;

  /// Types removed.
  TAO_NS_EventTypeSeq removed_;

  // Updates Dispatch Observer
  TAO_NS_Updates_Dispatch_Observer* updates_dispatch_observer_;

  /// Retry count. How many times have we failed to contact the remote peer?
  int retry_count_;
};

#if defined (__ACE_INLINE__)
#include "Peer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PEER_H */
