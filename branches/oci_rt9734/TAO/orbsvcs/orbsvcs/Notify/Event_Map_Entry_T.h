/* -*- C++ -*- */
/**
 *  @file Event_Map_Entry_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_EVENT_MAP_ENTRY_T_H
#define TAO_Notify_EVENT_MAP_ENTRY_T_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Copy_Disabled.h"
#include "ace/Atomic_Op.h"
#include "ace/CORBA_macros.h"
#include "tao/ORB_Constants.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class PROXY, class ACE_LOCK> class TAO_Notify_Event_Map_T;
template <class PROXY> class TAO_ESF_Proxy_Collection;

/**
 * @class TAO_Notify_Event_Map_Entry_T
 *
 * @brief The entry stored in the event map.
 *
 */
template <class PROXY>
class TAO_Notify_Event_Map_Entry_T : private ACE_Copy_Disabled
{
public:
  typedef TAO_ESF_Proxy_Collection<PROXY> COLLECTION;

  /// Constructor
  TAO_Notify_Event_Map_Entry_T (void);

  /// Destructor
  ~TAO_Notify_Event_Map_Entry_T (void);

  /// Init - Allocates collection
  void init (void);

  /// Connect
  void connected (PROXY* proxy);

  /// Disconnect
  void disconnected (PROXY* proxy);

  /// Collection accessor
  COLLECTION* collection (void);

  /// Count accessor
  int count (void);

  ///= Reference counting methods.
  // Incr the ref count.
  CORBA::ULong _incr_refcnt (void);

  // Decr the ref count. This object is destroyed when the count is 0.
  CORBA::ULong _decr_refcnt (void);

protected:
  /// The Collection
  COLLECTION* collection_;

  /// Count of PROXY's connected in the collection;
  int count_;

  /// Count of users accessing this entry.
  ACE_Atomic_Op<TAO_SYNCH_MUTEX,int> usage_count_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Event_Map_Entry_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Notify/Event_Map_Entry_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Event_Map_Entry_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_EVENT_MAP_ENTRY_T_H */
