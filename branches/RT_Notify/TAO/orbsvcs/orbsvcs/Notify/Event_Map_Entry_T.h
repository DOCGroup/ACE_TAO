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

#ifndef TAO_NS_EVENT_MAP_ENTRY_T_H
#define TAO_NS_EVENT_MAP_ENTRY_T_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Copy_Disabled.h"
#include "ace/Atomic_Op.h"
#include "tao/corba.h"

template <class PROXY, class ACE_LOCK> class TAO_NS_Event_Map_T;
template <class PROXY> class TAO_ESF_Proxy_Collection;

/**
 * @class TAO_NS_Event_Map_Entry_T
 *
 * @brief The entry stored in the event map.
 *
 */
template <class PROXY>
class TAO_NS_Event_Map_Entry_T : private ACE_Copy_Disabled
{
public:
  typedef TAO_ESF_Proxy_Collection<PROXY> COLLECTION;

  /// Constructor
  TAO_NS_Event_Map_Entry_T (void);

  /// Destructor
  ~TAO_NS_Event_Map_Entry_T (void);

  /// Init - Allocates collection
  void init (ACE_ENV_SINGLE_ARG_DECL);

  /// Connect
  void connected (PROXY* proxy ACE_ENV_ARG_DECL);

  /// Disconnect
  void disconnected (PROXY* proxy ACE_ENV_ARG_DECL);

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

#if defined (__ACE_INLINE__)
#include "Event_Map_Entry_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Event_Map_Entry_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Event_Map_Entry_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_NS_EVENT_MAP_ENTRY_T_H */
