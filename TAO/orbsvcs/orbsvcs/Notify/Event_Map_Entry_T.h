/* -*- C++ -*- */
/**
 *  @file Event_Map_Entry_T.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_EVENT_MAP_ENTRY_T_H
#define TAO_Notify_EVENT_MAP_ENTRY_T_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
 */
template <class PROXY>
class TAO_Notify_Event_Map_Entry_T
{
public:
  typedef TAO_ESF_Proxy_Collection<PROXY> COLLECTION;

  /// Constructor
  TAO_Notify_Event_Map_Entry_T ();

  TAO_Notify_Event_Map_Entry_T (const TAO_Notify_Event_Map_Entry_T &) = delete;
  TAO_Notify_Event_Map_Entry_T (TAO_Notify_Event_Map_Entry_T &&) = delete;
  TAO_Notify_Event_Map_Entry_T &operator= (const TAO_Notify_Event_Map_Entry_T &) = delete;
  TAO_Notify_Event_Map_Entry_T &operator= (TAO_Notify_Event_Map_Entry_T &&) = delete;

  /// Destructor
  ~TAO_Notify_Event_Map_Entry_T ();

  /// Init - Allocates collection
  void init ();

  /// Connect
  void connected (PROXY* proxy);

  /// Disconnect
  void disconnected (PROXY* proxy);

  /// Collection accessor
  COLLECTION* collection ();

  /// Count accessor
  int count ();

  ///= Reference counting methods.
  // Incr the ref count.
  CORBA::ULong _incr_refcnt ();

  // Decr the ref count. This object is destroyed when the count is 0.
  CORBA::ULong _decr_refcnt ();

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

#include "orbsvcs/Notify/Event_Map_Entry_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_Notify_EVENT_MAP_ENTRY_T_H */
