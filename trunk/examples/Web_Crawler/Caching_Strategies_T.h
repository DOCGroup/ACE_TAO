/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Caching_Strategies_T.h
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef CACHING_STRATEGIES_H
#define CACHING_STRATEGIES_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class CONTAINER>
class ACE_LRU_Caching_Strategy
{
  // = TITLE
  //    Defines a Least Recently Used strategy which will decide on
  //    the item to be removed from the cache.
  //
  // = DESCRIPTION
  //     This is a strategy which makes use of a virtual timer which
  //     is updated whenever an item is inserted or looked up in the
  //     container. When the need of purging entries arises, the items
  //     with the lowest timer values are removed.

public:

  // Traits.
  typedef int ATTRIBUTES;

  // @@ Should these be here?
  typedef ACE_TYPENAME CONTAINER::KEY KEY;
  typedef ACE_TYPENAME CONTAINER::VALUE VALUE;
  typedef ACE_TYPENAME CONTAINER::ITERATOR ITERATOR;
  typedef ACE_TYPENAME CONTAINER::ENTRY ITEM;

  // = Initialisation and termination.

  ACE_LRU_Caching_Strategy (CONTAINER &container,
                            ATTRIBUTES timer = 0,
                            int purge_percent = 10);
  // The <container> is the map in which the entries reside.
  // The timer attribute could be initialsed as per need.
  // The <purge_percent> field denotes the percentage of the entries
  // in the cache which can be purged automagically.

  ~ACE_LRU_Caching_Strategy (void);

  // = Operations of the startegy.

  ATTRIBUTES attributes (void);
  // Accessor method for the timer attributes.

  // = Accessor methods for the percentage of entries to purge.
  int purge_percent (void);
  void purge_percent (int percentage);

  // =  Strategy related Operations

  int notify_bind (int result,
                   const ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs bind
  // method call.

  int notify_find (int result,
                   ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs find
  // method call

  int notify_unbind (int result,
                     const ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs unbind
  // method call

  int notify_trybind (int result,
                      ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs trybind
  // method call

  int notify_rebind (int result,
                     const ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs rebind
  // method call

  int clear_cache (CONTAINER &container);
  // This is the method which looks at each ITEM's attributes  and
  // then decides on the one to remove.

  void dump (void) const;
  // Dumps the state of the object.

 private:

  ATTRIBUTES timer_;
  // This element is the one which is the deciding factor for purging
  // of an ITEM.

  CONTAINER &container_;
  // The container over which the purging will occur according to the
  // strategy.

  int purge_percent_;
  // The level about which the purging will happen automagically.

  int entries_;
  // The no of entries bound in the cache.
};

//////////////////////////////////////////////////////////////////////////

template <class CONTAINER>
class ACE_LFU_Caching_Strategy
{
  // = TITLE
  //     Defines a Least Frequently Used strategy for which will decide on
  //   the item to be removed from the cache.
  //
  // = DESCRIPTION
  //     A attribute is tagged to each item which increments whenever
  //   the item is bound or looked up in the cache. Thus it denotes
  //   the frequency of use. According to the value of the attribute
  //   the item is removed from the CONTAINER i.e cache.
public:

  // Traits.
  typedef int ATTRIBUTES;

  typedef ACE_TYPENAME CONTAINER::KEY KEY;

  typedef ACE_TYPENAME CONTAINER::VALUE VALUE;

  typedef ACE_TYPENAME CONTAINER::ITERATOR ITERATOR;

  typedef ACE_TYPENAME CONTAINER::ENTRY ITEM;

  // = Initialisation and termination methods.

  ACE_LFU_Caching_Strategy (void);

  ~ACE_LFU_Caching_Strategy (void);

  // = Startegy methods. Most of the methods are used from the base
  //   class itself.

  ATTRIBUTES attributes (void);
  // Access the attributes.

  int notify_bind (int result,
                   const ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs bind
  // method call.

  int notify_find (int result,
                   ATTRIBUTES &attr);
  // Lookup notification.

  int notify_unbind (int result,
                     const ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs unbind
  // method call

  int notify_trybind (int result,
                      ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs trybind
  // method call

  int notify_rebind (int result,
                     const ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs rebind
  // method call

  int clear_cache (CONTAINER &container);
  // This is the method which looks at each ITEM's attributes  and
  // then decides on the one to remove.

  void dump (void) const;
  // Dumps the state of the object.
};

/////////////////////////////////////////////////////////////

template<class CONTAINER>
class ACE_FIFO_Caching_Strategy
{
  // = TITLE
  //    The First In First Out startegy is implemented wherein each
  //    item is ordered.
  //
  // = DESCRIPTION
  //    The order tag of each item is used to decide the item to be
  //    removed from the cache. The items with least order are removed.
 public:

  typedef int ATTRIBUTES;

  // Traits.
  typedef ACE_TYPENAME CONTAINER::KEY  KEY;

  typedef ACE_TYPENAME CONTAINER::VALUE  VALUE;

  typedef ACE_TYPENAME CONTAINER::ITERATOR ITERATOR;

  typedef ACE_TYPENAME CONTAINER::ENTRY ITEM;

  // = Initialisation and termination.

  ACE_FIFO_Caching_Strategy (ATTRIBUTES order = 0);

  ~ACE_FIFO_Caching_Strategy (void);

  // = Startegy methods.

  ATTRIBUTES attributes (void);
  // Accessor method.

  int notify_bind (int result,
                   const ATTRIBUTES &attr);
  // Notification for an item getting bound into the cache.

  int notify_find (int result,
                   ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs find
  // method call

  int notify_unbind (int result,
                     const ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs unbind
  // method call

  int notify_trybind (int result,
                      ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs trybind
  // method call

  int notify_rebind (int result,
                     const ATTRIBUTES &attr);
  // Notification for an item getting bound again into the cache.

  int clear_cache (CONTAINER &container);
  // This is the method which looks at each ITEM's attributes  and
  // then decides on the one to remove.

  void dump (void) const;
  // Dumps the state of the object.
private:

  ATTRIBUTES order_;
  // The order is the deciding factor for the item to be removed from
  // the cache.
};

template<class CONTAINER>
class ACE_Null_Caching_Strategy
{
  // = TITLE
  //    The is a special caching strategy which doesnt have the purging
  //    feature.
  //
  // = DESCRIPTION
  //    No purging provided. To be used when purging might be too expensive
  //    an operation.

 public:

  typedef int ATTRIBUTES;

  // = Initialisation and termination.

  ACE_Null_Caching_Strategy (void);

  ~ACE_Null_Caching_Strategy (void);

  // = Startegy methods. All are NO_OP methods!!!

  ATTRIBUTES attributes (void);
  // Accessor method.

  int notify_bind (int result,
                   const ATTRIBUTES &attr);
  // Notification for an item getting bound into the cache.

  int notify_find (int result,
                   ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs find
  // method call

  int notify_unbind (int result,
                     const ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs unbind
  // method call

  int notify_trybind (int result,
                      ATTRIBUTES &attr);
  //This method acts as a notification about the CONTAINERs trybind
  // method call

  int notify_rebind (int result,
                     const ATTRIBUTES &attr);
  // Notification for an item getting bound again into the cache.

  int clear_cache (CONTAINER &container);
  // This is the method which looks at each ITEM's attributes  and
  // then decides on the one to remove.

  void dump (void) const;
  // Dumps the state of the object.

private:

  // @@ Remove this
  ATTRIBUTES dummy_;
  // Just a dummy member to be able to keep up with the common interface.
};

#if defined (__ACE_INLINE__)
#include "Caching_Strategies_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Caching_Strategies_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation "Caching_Strategies_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CACHING_STRATEGIES_H */
