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

#include "ace/Caching_Strategy_Utility_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Cleanup_Strategies_T.h"

template <class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY>
class ACE_LRU_Caching_Strategy
{
  // = TITLE
  //     Defines a Least Recently Used strategy which will decide on
  //     the item to be removed from the cache.
  //
  // = DESCRIPTION
  //     This is a strategy which makes use of a virtual timer which
  //     is updated whenever an item is inserted or looked up in the
  //     container. When the need of purging entries arises, the items
  //     with the lowest timer values are removed.
  //     
  //     Explanation of the template parameter list:
  //     CONTAINER is any map with entries of type <KEY, VALUE>.
  //     The ATTRIBUTES are the deciding factor for purging of entries
  //     and should logically be included with the VALUE. Some ways of
  //     doing this are: As being a member of the VALUE or VALUE being
  //     ACE_Pair<x, ATTRIBUTES>. The CACHING_STRATEGY_UTILITY is the
  //     class which can be plugged in and which decides the entries
  //     to purge. 

public:

  // Traits.
  typedef ATTRIBUTES CACHING_ATTRIBUTES;
  typedef CONTAINER CACHE;
 
  // = Initialisation and termination.

  ACE_LRU_Caching_Strategy (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s = 0,
                            int delete_cleanup_strategy = 1,
                            CACHING_STRATEGY_UTILITY *utility_s = 0,
                            int delete_caching_strategy_utility = 1);
  // The <container> is the map in which the entries reside.
  // The Cleanup_Strategy is the callback class to which the entries
  // to be cleaned up will be delegated. The <delete_cleanup_strategy>
  // flag is needed to decide the ownership of the cleanup strategy.
  // Also, the timer attribute is initialed to zero in this constructor.
  // And the <purge_percent> field denotes the percentage of the entries
  // in the cache which can be purged automagically and by default is
  // set to 10%. The ultility which helps the caching strategy in the
  // purging of entries needs to be specified. By default a new one
  // will be created of type CACHING_STRATEGY_UTILITY and 
  // <delete_caching_strategy_utility> decides whether to destroy the
  // utility object or not.

  ~ACE_LRU_Caching_Strategy (void);

  // = Operations of the strategy.

  int open (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s = 0,
            int delete_cleanup_strategy = 1,
            CACHING_STRATEGY_UTILITY *utility_s = 0,
            int delete_caching_strategy_utility = 1);
  // This method which does the actual initialisation.

  ATTRIBUTES attributes (void);
  // Accessor method for the timer attributes.

  // = Accessor methods for the percentage of entries to purge.
  unsigned int purge_percent (void);
  
  void purge_percent (unsigned int percentage);

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

  int clear_cache (CONTAINER &container, 
                   unsigned int &total_container_entries);
  // This is the method which looks at each ITEM's attributes  and
  // then decides on the one to remove. The <total_container_entries>
  // can be specified explicitly and facilitates use of the caching
  // strategy even when the <entries_> is not maintained by the
  // strategy itself.

  void dump (void) const;
  // Dumps the state of the object.

private:

  typedef ACE_Default_Cleanup_Strategy<KEY, VALUE, CONTAINER> CLEANUP_STRATEGY;

  ATTRIBUTES timer_;
  // This element is the one which is the deciding factor for purging
  // of an ITEM.

  unsigned int purge_percent_;
  // The level about which the purging will happen automagically.

  unsigned int entries_;
  // The no of entries bound in the cache.
  
  ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_strategy_;
  // The cleanup strategy which can be used to destroy the entries of
  // the container.

  int delete_cleanup_strategy_;
  // The flag which denotes the ownership of the cleanup strategy.
  // If 1 then this class itself will destroy the strategy.

  CACHING_STRATEGY_UTILITY *caching_strategy_utility_;
  // This is the helper class which will decide and expunge entries
  // from the cache.
  
  int delete_caching_strategy_utility_;
  // The flag which denotes the ownership of the
  // caching_strategy_utility. If 1 then this class itself will
  // destroy the strategy utility object.

};

//////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY>
class ACE_LFU_Caching_Strategy
{
  // = TITLE
  //     Defines a Least Frequently Used strategy for which will decide on
  //     the item to be removed from the cache.
  //
  // = DESCRIPTION
  //     A attribute is tagged to each item which increments whenever
  //     the item is bound or looked up in the cache. Thus it denotes
  //     the frequency of use. According to the value of the attribute
  //     the item is removed from the CONTAINER i.e cache.
  //
  //     Explanation of the template parameter list:
  //     CONTAINER is any map with entries of type <KEY, VALUE>.
  //     The ATTRIBUTES are the deciding factor for purging of entries
  //     and should logically be included with the VALUE. Some ways of
  //     doing this are: As being a member of the VALUE or VALUE being
  //     ACE_Pair<x, ATTRIBUTES>. The CACHING_STRATEGY_UTILITY is the
  //     class which can be plugged in and which decides the entries
  //     to purge. 

public:

  // Traits.
  typedef ATTRIBUTES CACHING_ATTRIBUTES;
  typedef CONTAINER CACHE;

  // = Initialisation and termination methods.

  ACE_LFU_Caching_Strategy (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s = 0,
                            int delete_cleanup_strategy = 1,
                            CACHING_STRATEGY_UTILITY *utility_s = 0,
                            int delete_caching_strategy_utility = 1);
  // The <container> is the map in which the entries reside.
  // The Cleanup_Strategy is the callback class to which the entries
  // to be cleaned up will be delegated. The <delete_cleanup_strategy>
  // flag is needed to decide the ownership of the cleanup strategy.
  // Also, the timer attribute is initialed to zero in this constructor.
  // And the <purge_percent> field denotes the percentage of the entries
  // in the cache which can be purged automagically and by default is
  // set to 10%.The ultility which helps the caching strategy in the
  // purging of entries will be default be the
  // ACE_Caching_Strategy_Utility and the
  // <delete_caching_strategy_utility> decides whether to destroy the
  // utility or not.

  ~ACE_LFU_Caching_Strategy (void);

  int open (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s = 0,
            int delete_cleanup_strategy = 1,
            CACHING_STRATEGY_UTILITY *utility_s = 0,
            int delete_caching_strategy_utility = 1);
  // This method which does the actual initialisation.

  // = Strategy methods.

  ATTRIBUTES attributes (void);
  // Access the attributes.

  // = Accessor methods for the percentage of entries to purge.
  unsigned int purge_percent (void);
  
  void purge_percent (unsigned int percentage);

  // =  Strategy related Operations
  
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

  int clear_cache (CONTAINER &container, 
                   unsigned int &total_container_entries);
  // This is the method which looks at each ITEM's attributes  and
  // then decides on the one to remove. The <total_container_entries>
  // can be specified explicitly and facilitates use of the caching
  // strategy even when the <entries_> is not maintained by the
  // strategy itself.

  void dump (void) const;
  // Dumps the state of the object.

private:

  typedef ACE_Default_Cleanup_Strategy<KEY, VALUE, CONTAINER> CLEANUP_STRATEGY;
 
  unsigned int purge_percent_;
  // The level about which the purging will happen automagically.

  unsigned int entries_;
  // The no of entries bound in the cache.

  ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_strategy_;
  // The cleanup strategy which can be used to destroy the entries of
  // the container.

  int delete_cleanup_strategy_;
  // The flag which denotes the ownership of the cleanup strategy.
  // If 1 then this class itself will destroy the strategy.

  CACHING_STRATEGY_UTILITY *caching_strategy_utility_;
  // This is the helper class which will decide and expunge entries
  // from the cache.

  int delete_caching_strategy_utility_;
  // The flag which denotes the ownership of the
  // caching_strategy_utility. If 1 then this class itself will
  // destroy the strategy utility object.
};

/////////////////////////////////////////////////////////////

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY>
class ACE_FIFO_Caching_Strategy
{
  // = TITLE
  //     The First In First Out strategy is implemented wherein each
  //     item is ordered.
  //
  // = DESCRIPTION
  //     The order tag of each item is used to decide the item to be
  //     removed from the cache. The items with least order are removed.
  //
  //     Explanation of the template parameter list:
  //     CONTAINER is any map with entries of type <KEY, VALUE>.
  //     The ATTRIBUTES are the deciding factor for purging of entries
  //     and should logically be included with the VALUE. Some ways of
  //     doing this are: As being a member of the VALUE or VALUE being
  //     ACE_Pair<x, ATTRIBUTES>. The CACHING_STRATEGY_UTILITY is the
  //     class which can be plugged in and which decides the entries
  //     to purge. 

public:

  typedef ATTRIBUTES CACHING_ATTRIBUTES;
  typedef CONTAINER CACHE;

  // = Initialisation and termination.

  ACE_FIFO_Caching_Strategy (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s = 0,
                             int delete_cleanup_strategy = 1,
                             CACHING_STRATEGY_UTILITY *utility_s = 0,
                             int delete_caching_strategy_utility = 1);
  // The <container> is the map in which the entries reside.
  // The Cleanup_Strategy is the callback class to which the entries
  // to be cleaned up will be delegated. The <delete_cleanup_strategy>
  // flag is needed to decide the ownership of the cleanup strategy.
  // Also, the timer attribute is initialed to zero in this constructor.
  // And the <purge_percent> field denotes the percentage of the entries
  // in the cache which can be purged automagically and by default is
  // set to 10%.The ultility which helps the caching strategy in the
  // purging of entries will be default be the
  // ACE_Caching_Strategy_Utility and the
  // <delete_caching_strategy_utility> decides whether to destroy the
  // utility or not. 
  
  ~ACE_FIFO_Caching_Strategy (void);

  int open (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s = 0,
            int delete_cleanup_strategy = 1,
            CACHING_STRATEGY_UTILITY *utility_s = 0,
            int delete_caching_strategy_utility = 1);
  // This method which does the actual initialisation.

  // = Strategy methods.

  ATTRIBUTES attributes (void);
  // Accessor method.

  // = Accessor methods for the percentage of entries to purge.
  unsigned int purge_percent (void);
  
  void purge_percent (unsigned int percentage);

  // =  Strategy related Operations

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

  int clear_cache (CONTAINER &container, 
                   unsigned int &total_container_entries);
  // This is the method which looks at each ITEM's attributes  and
  // then decides on the one to remove. The <total_container_entries>
  // can be specified explicitly and facilitates use of the caching
  // strategy even when the <entries_> is not maintained by the
  // strategy itself.

  void dump (void) const;
  // Dumps the state of the object.

private:

  typedef ACE_Default_Cleanup_Strategy<KEY, VALUE, CONTAINER> CLEANUP_STRATEGY;
 
  ATTRIBUTES order_;
  // The order is the deciding factor for the item to be removed from
  // the cache.

  unsigned int purge_percent_;
  // The level about which the purging will happen automagically.

  unsigned int entries_;
  // The no of entries bound in the cache.
 
  ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_strategy_;
  // The cleanup strategy which can be used to destroy the entries of
  // the container.

  int delete_cleanup_strategy_;
  // The flag which denotes the ownership of the cleanup strategy.
  // If 1 then this class itself will destroy the strategy.

  CACHING_STRATEGY_UTILITY *caching_strategy_utility_;
  // This is the helper class which will decide and expunge entries
  // from the cache.

  int delete_caching_strategy_utility_;
  // The flag which denotes the ownership of the
  // caching_strategy_utility. If 1 then this class itself will
  // destroy the strategy utility object.

};

template<class KEY, class VALUE, class CONTAINER, class ATTRIBUTES, class CACHING_STRATEGY_UTILITY>
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

  // = Traits.
  typedef ATTRIBUTES CACHING_ATTRIBUTES;
  typedef CONTAINER CACHE;

  // = Initialisation and termination.

  ACE_Null_Caching_Strategy (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s = 0,
                             int delete_cleanup_strategy = 1,
                             CACHING_STRATEGY_UTILITY *utility_s = 0,
                             int delete_caching_strategy_utility = 1);

  ~ACE_Null_Caching_Strategy (void);


  int open (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s = 0,
            int delete_cleanup_strategy = 1,
            CACHING_STRATEGY_UTILITY *utility_s = 0,
            int delete_caching_strategy_utility = 1);
  // This method which does the actual initialisation.

  // = Strategy methods. All are NO_OP methods!!!

  ATTRIBUTES attributes (void);
  // Accessor method.

 // = Accessor methods for the percentage of entries to purge.
  unsigned int purge_percent (void);
  
  void purge_percent (unsigned int percentage);

  // =  Strategy related Operations

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

  int clear_cache (CONTAINER &container, 
                   unsigned int &total_container_entries);
  // This is the method which looks at each ITEM's attributes  and
  // then decides on the one to remove. The <total_container_entries>
  // can be specified explicitly and facilitates use of the caching
  // strategy even when the <entries_> is not maintained by the
  // strategy itself.

  void dump (void) const;
  // Dumps the state of the object.
  
};

#if defined (__ACE_INLINE__)
#include "ace/Caching_Strategies_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Caching_Strategies_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation "ace/Caching_Strategies_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CACHING_STRATEGIES_H */
