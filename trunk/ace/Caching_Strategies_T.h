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
#include "ace/pre.h"

#include "ace/OS.h"

#include "ace/Caching_Utility_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(disable:4503)
#endif /* _MSC_VER */

// For linkers that cant grok long names.
#define ACE_Caching_Strategy ACS

template <class ATTRIBUTES, class CACHING_UTILITY>
class ACE_Caching_Strategy
{
  // = TITLE
  //     This class is an abstract base class for a caching strategy.
  //
  // = DESCRIPTION
  //     This class consists of all the interfaces a caching strategy should have and
  //   is used in association with the ACE_Caching_Strategy_Adaptor.

public:

  virtual ~ACE_Caching_Strategy (void);
  // Destructor.

  virtual ATTRIBUTES attributes (void) = 0;
  // Accessor method for the timer attributes.

  // = Accessor methods for the percentage of entries to purge.
  virtual double purge_percent (void) = 0;
  virtual void purge_percent (double percentage) = 0;

  // = Strategy related Operations

  virtual int notify_bind (int result,
                           const ATTRIBUTES &attr) = 0;
  // This method acts as a notification about the CONTAINERs bind
  // method call.

  virtual int notify_find (int result,
                           ATTRIBUTES &attr) = 0;
  // This method acts as a notification about the CONTAINERs find
  // method call

  virtual int notify_unbind (int result,
                             const ATTRIBUTES &attr) = 0;
  // This method acts as a notification about the CONTAINERs unbind
  // method call

  virtual int notify_trybind (int result,
                              ATTRIBUTES &attr) = 0;
  // This method acts as a notification about the CONTAINERs trybind
  // method call

  virtual int notify_rebind (int result,
                             const ATTRIBUTES &attr) = 0;
  // This method acts as a notification about the CONTAINERs rebind
  // method call

  virtual CACHING_UTILITY &caching_utility (void) = 0;
  // Purge the cache.

  virtual void dump (void) const = 0;
  // Dumps the state of the object.
};

//////////////////////////////////////////////////////////////////////////

#define ACE_Caching_Strategy_Adapter ACSA

template <class ATTRIBUTES, class CACHING_UTILITY, class IMPLEMENTATION>
class ACE_Caching_Strategy_Adapter : public ACE_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>
{
  // = TITLE
  //     This class follows the Adaptor pattern and is used to provide
  //     External Polymorphism by deriving from ACE_Caching_Strategy.
  //
  // = DESCRIPTION
  //     This class simply delegates all requests to the
  //     IMPLEMNETATION object within. This class should be passed in
  //     place of the the abstract base ACE_Caching_Strategy class as
  //     part of the External Polymorphism pattern.

public:

  ACE_Caching_Strategy_Adapter (IMPLEMENTATION *implementation = 0,
                                int delete_implementation = 0);
  // Constructor.

  ~ACE_Caching_Strategy_Adapter (void);
  // Destructor.

  ATTRIBUTES attributes (void);
  // Accessor method for the timer attributes.

  // = Accessor methods for the percentage of entries to purge.
  double purge_percent (void);
  void purge_percent (double percentage);

  // = Strategy related Operations

  int notify_bind (int result,
                   const ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs bind
  // method call.

  int notify_find (int result,
                   ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs find
  // method call

  int notify_unbind (int result,
                     const ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs unbind
  // method call

  int notify_trybind (int result,
                      ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs trybind
  // method call

  int notify_rebind (int result,
                     const ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs rebind
  // method call

  IMPLEMENTATION &implementation (void);
  // Accessor to the implementation.

  CACHING_UTILITY &caching_utility (void);
  // Purge the cache.

  void dump (void) const;
  // Dumps the state of the object.

private:

  IMPLEMENTATION *implementation_;
  // Implementation class.

  int delete_implementation_;
  // Do we need to delete the implementation?
};

//////////////////////////////////////////////////////////////////////////
#define ACE_LRU_Caching_Strategy ALRU

template <class ATTRIBUTES, class CACHING_UTILITY>
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
  //     ACE_Pair<x, ATTRIBUTES>. The CACHING_UTILITY is the
  //     class which can be plugged in and which decides the entries
  //     to purge.

public:

  // Traits.
  typedef ATTRIBUTES CACHING_ATTRIBUTES;

  // = Initialisation and termination.

  ACE_LRU_Caching_Strategy (void);
  // The <container> is the map in which the entries reside.  The
  // timer attribute is initialed to zero in this constructor.  And
  // the <purge_percent> field denotes the percentage of the entries
  // in the cache which can be purged automagically and by default is
  // set to 10%.

  // = Operations of the strategy.

  ATTRIBUTES attributes (void);
  // Accessor method for the timer attributes.

  // = Accessor methods for the percentage of entries to purge.
  double purge_percent (void);

  void purge_percent (double percentage);

  // =  Strategy related Operations

  int notify_bind (int result,
                   const ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs bind
  // method call.

  int notify_find (int result,
                   ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs find
  // method call

  int notify_unbind (int result,
                     const ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs unbind
  // method call


  int notify_trybind (int result,
                      ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs trybind
  // method call

  int notify_rebind (int result,
                     const ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs rebind
  // method call

  CACHING_UTILITY &caching_utility (void);
  // Purge the cache.

  void dump (void) const;
  // Dumps the state of the object.

private:

  ATTRIBUTES timer_;
  // This element is the one which is the deciding factor for purging
  // of an ITEM.

  double purge_percent_;
  // The level about which the purging will happen automagically.

  CACHING_UTILITY caching_utility_;
  // This is the helper class which will decide and expunge entries
  // from the cache.
};

//////////////////////////////////////////////////////////////////////////
#define ACE_LFU_Caching_Strategy ALFU

template <class ATTRIBUTES, class CACHING_UTILITY>
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
  //     ACE_Pair<x, ATTRIBUTES>. The CACHING_UTILITY is the
  //     class which can be plugged in and which decides the entries
  //     to purge.

public:

  // Traits.
  typedef ATTRIBUTES CACHING_ATTRIBUTES;

  // = Initialisation and termination methods.

  ACE_LFU_Caching_Strategy (void);
  // The <container> is the map in which the entries reside.  The
  // timer attribute is initialed to zero in this constructor.  And
  // the <purge_percent> field denotes the percentage of the entries
  // in the cache which can be purged automagically and by default is
  // set to 10%.

  // = Strategy methods.

  ATTRIBUTES attributes (void);
  // Access the attributes.

  // = Accessor methods for the percentage of entries to purge.
  double purge_percent (void);

  void purge_percent (double percentage);

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
  // This method acts as a notification about the CONTAINERs unbind
  // method call

  int notify_trybind (int result,
                      ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs trybind
  // method call

  int notify_rebind (int result,
                     const ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs rebind
  // method call

  CACHING_UTILITY &caching_utility (void);
  // Purge the cache.

  void dump (void) const;
  // Dumps the state of the object.

private:

  double purge_percent_;
  // The level about which the purging will happen automagically.

  CACHING_UTILITY caching_utility_;
  // This is the helper class which will decide and expunge entries
  // from the cache.
};

/////////////////////////////////////////////////////////////
#define ACE_FIFO_Caching_Strategy AFIFO

template<class ATTRIBUTES, class CACHING_UTILITY>
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
  //     ACE_Pair<x, ATTRIBUTES>. The CACHING_UTILITY is the
  //     class which can be plugged in and which decides the entries
  //     to purge.

public:

  typedef ATTRIBUTES CACHING_ATTRIBUTES;

  // = Initialisation and termination.

  ACE_FIFO_Caching_Strategy (void);
  // The <container> is the map in which the entries reside.  The
  // timer attribute is initialed to zero in this constructor.  And
  // the <purge_percent> field denotes the percentage of the entries
  // in the cache which can be purged automagically and by default is
  // set to 10%.

  // = Strategy methods.

  ATTRIBUTES attributes (void);
  // Accessor method.

  // = Accessor methods for the percentage of entries to purge.
  double purge_percent (void);

  void purge_percent (double percentage);

  // =  Strategy related Operations

  int notify_bind (int result,
                   const ATTRIBUTES &attr);
  // Notification for an item getting bound into the cache.

  int notify_find (int result,
                   ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs find
  // method call

  int notify_unbind (int result,
                     const ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs unbind
  // method call

  int notify_trybind (int result,
                      ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs trybind
  // method call

  int notify_rebind (int result,
                     const ATTRIBUTES &attr);
  // Notification for an item getting bound again into the cache.

  CACHING_UTILITY &caching_utility (void);
  // Purge the cache.

  void dump (void) const;
  // Dumps the state of the object.

private:

  ATTRIBUTES order_;
  // The order is the deciding factor for the item to be removed from
  // the cache.

  double purge_percent_;
  // The level about which the purging will happen automagically.

  CACHING_UTILITY caching_utility_;
  // This is the helper class which will decide and expunge entries
  // from the cache.
};

//////////////////////////////////////////////////////////////////////
#define ACE_Null_Caching_Strategy ANULL

template<class ATTRIBUTES, class CACHING_UTILITY>
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

  // = Strategy methods. All are NO_OP methods!!!

  ATTRIBUTES attributes (void);
  // Accessor method.

 // = Accessor methods for the percentage of entries to purge.
  double purge_percent (void);

  void purge_percent (double percentage);

  // =  Strategy related Operations

  int notify_bind (int result,
                   const ATTRIBUTES &attr);
  // Notification for an item getting bound into the cache.

  int notify_find (int result,
                   ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs find
  // method call

  int notify_unbind (int result,
                     const ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs unbind
  // method call

  int notify_trybind (int result,
                      ATTRIBUTES &attr);
  // This method acts as a notification about the CONTAINERs trybind
  // method call

  int notify_rebind (int result,
                     const ATTRIBUTES &attr);
  // Notification for an item getting bound again into the cache.

  CACHING_UTILITY &caching_utility (void);
  // Purge the cache.

  void dump (void) const;
  // Dumps the state of the object.

private:

  CACHING_UTILITY caching_utility_;
  // This is the helper class which will decide and expunge entries
  // from the cache.
};

#if defined (__ACE_INLINE__)
#include "ace/Caching_Strategies_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Caching_Strategies_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Caching_Strategies_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* CACHING_STRATEGIES_H */
