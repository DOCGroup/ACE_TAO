/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Caching_Utility_T.h
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef CACHING_UTILITY_H
#define CACHING_UTILITY_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Cleanup_Strategies_T.h"

// For linkers that cant grok long names.
#define ACE_Pair_Caching_Utility APUTIL

template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES>
class ACE_Pair_Caching_Utility
{
  // = TITLE
  //    Defines a helper class for the Caching Strategies.
  //
  // = DESCRIPTION
  //   This class defines the methods commonly used by the different
  //   caching strategies. For instance: <clear_cache> method which
  //   decides and purges the entry from the container.  Note: This
  //   class helps in the caching_strategies using a container
  //   containing entries of <KEY, ACE_Pair<VALUE, attributes>>
  //   kind. The attributes helps in deciding the entries to be
  //   purged. The Cleanup_Strategy is the callback class to which the
  //   entries to be cleaned up will be delegated.
public:

  typedef ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> CLEANUP_STRATEGY;

  ACE_Pair_Caching_Utility (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_strategy = 0,
                            int delete_cleanup_strategy = 0);

  // Constructor.

  ~ACE_Pair_Caching_Utility (void);
  // Destructor.

  int clear_cache (CONTAINER &container,
                   double purge_percent);

  // Purge entries from the <container>. The Cleanup_Strategy will do
  // the actual job of cleanup once the entries to be cleaned up are
  // decided.

protected:

  void minimum (CONTAINER &container,
                KEY *&key_to_remove,
                VALUE *&value_to_remove);
  // Find the entry with minimum caching attributes.

  CLEANUP_STRATEGY *cleanup_strategy_;
  // The cleanup strategy which can be used to destroy the entries of
  // the container.

  int delete_cleanup_strategy_;
  // Whether the cleanup_strategy should be destroyed or not.
};

////////////////////////////////////////////////////////////////////////////////
#define ACE_Recyclable_Handler_Caching_Utility ARHUTIL

template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES>
class ACE_Recyclable_Handler_Caching_Utility
{
  // = TITLE
  //    Defines a helper class for the Caching Strategies.
  //
  // = DESCRIPTION
  //    This class defines the methods commonly used by the different
  //    caching strategies. For instance: <clear_cache> method which
  //    decides and purges the entry from the container.  Note: This
  //    class helps in the caching_strategies using a container
  //    containing entries of <KEY, Svc_Handler> kind. The attributes
  //    helps in deciding the entries to be purged. The
  //    Cleanup_Strategy is the callback class to which the entries to
  //    be cleaned up will be delegated.

public:

  typedef ACE_Recyclable_Handler_Cleanup_Strategy<KEY, VALUE, CONTAINER> CLEANUP_STRATEGY;
  typedef ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> CLEANUP_STRATEGY_BASE;

  ACE_Recyclable_Handler_Caching_Utility (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_strategy = 0,
                                          int delete_cleanup_strategy = 0);

  // Constructor.

  ~ACE_Recyclable_Handler_Caching_Utility (void);
  // Destructor.

  int clear_cache (CONTAINER &container,
                   double purge_percent);
  // Purge entries from the <container>. The Cleanup_Strategy will do
  // the actual job of cleanup once the entries to be cleaned up are
  // decided.

protected:

  void minimum (CONTAINER &container,
                KEY *&key_to_remove,
                VALUE *&value_to_remove);
  // Find the entry with minimum caching attributes.

  CLEANUP_STRATEGY_BASE *cleanup_strategy_;
  // This is the default Cleanup Strategy for this utility.

  int delete_cleanup_strategy_;
  // Whether the cleanup_strategy should be destroyed or not.

};

////////////////////////////////////////////////////////////////////////////////////////
#define ACE_Refcounted_Recyclable_Handler_Caching_Utility ARRHUTIL

template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES>
class ACE_Refcounted_Recyclable_Handler_Caching_Utility
{
  // = TITLE
  //    Defines a helper class for the Caching Strategies.
  //
  // = DESCRIPTION
  //    This class defines the methods commonly used by the different
  //    caching strategies. For instance: clear_cache () method which
  //    decides and purges the entry from the container.  Note: This
  //    class helps in the caching_strategies using a container
  //    containing entries of <Refcounted_KEY,
  //    Recyclable_Connection_Handler> kind. The attributes helps in
  //    deciding the entries to be purged. The Cleanup_Strategy is the
  //    callback class to which the entries to be cleaned up will be
  //    delegated.

public:

  typedef ACE_Refcounted_Recyclable_Handler_Cleanup_Strategy<KEY, VALUE, CONTAINER> CLEANUP_STRATEGY;
  typedef ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> CLEANUP_STRATEGY_BASE;

  ACE_Refcounted_Recyclable_Handler_Caching_Utility (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_strategy = 0,
                                                     int delete_cleanup_strategy = 0);

  // Constructor.

  ~ACE_Refcounted_Recyclable_Handler_Caching_Utility (void);
  // Destructor.

  int clear_cache (CONTAINER &container,
                   double purge_percent);
  // Purge entries from the <container>. The Cleanup_Strategy will do
  // the actual job of cleanup once the entries to be cleaned up are
  // decided.

protected:

  void minimum (CONTAINER &container,
                KEY *&key_to_remove,
                VALUE *&value_to_remove);
  // Find the entry with minimum caching attributes.

  CLEANUP_STRATEGY_BASE *cleanup_strategy_;
  // This is the default Cleanup Strategy for this utility.

  int delete_cleanup_strategy_;
  // Whether the cleanup_strategy should be destroyed or not.

  size_t marked_as_closed_entries_;
  // This figure denotes the number of entries are there in the
  // container which have been marked as closed already but might 
  // not have been unbound from the container.

};

////////////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES>
class ACE_Handler_Caching_Utility
{
  // = TITLE
  //    Defines a helper class for the Caching Strategies.
  //
  // = DESCRIPTION
  //    This class defines the methods commonly used by the different
  //    caching strategies. For instance: <clear_cache> method which
  //    decides and purges the entry from the container.  Note: This
  //    class helps in the caching_strategies using a container
  //    containing entries of <KEY, HANDLER> kind where the HANDLER
  //    contains the caching attributes which help in deciding the
  //    entries to be purged. The Cleanup_Strategy is the callback
  //    class to which the entries to be cleaned up will be delegated.
public:

  typedef ACE_Handler_Cleanup_Strategy<KEY, VALUE, CONTAINER> CLEANUP_STRATEGY;
  typedef ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> CLEANUP_STRATEGY_BASE;

  ACE_Handler_Caching_Utility (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_strategy = 0,
                               int delete_cleanup_strategy = 0);
  // Constructor.

  ~ACE_Handler_Caching_Utility (void);
  // Destructor.

  int clear_cache (CONTAINER &container,
                   double purge_percent);
  // Purge entries from the <container>. The Cleanup_Strategy will do
  // the actual job of cleanup once the entries to be cleaned up are
  // decided.

protected:

  void minimum (CONTAINER &container,
                KEY *&key_to_remove,
                VALUE *&value_to_remove);
  // Find the entry with minimum caching attributes.  This is handler
  // specific since this utility is to be used very specifically for
  // handler who have caching_attributes for server side acched
  // connection management.

  CLEANUP_STRATEGY_BASE *cleanup_strategy_;
  // The cleanup strategy which can be used to destroy the entries of
  // the container.

  int delete_cleanup_strategy_;
  // Whether the cleanup_strategy should be destroyed or not.

};

///////////////////////////////////////////////////////////////////////////
#define ACE_Null_Caching_Utility ANUTIL
template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES>
class ACE_Null_Caching_Utility
{
  // = TITLE
  //    Defines a dummy helper class for the Caching Strategies.
  //
  // = DESCRIPTION
  //    This class defines the methods commonly used by the different
  //    caching strategies. For instance: <clear_cache> method which
  //    decides and purges the entry from the container.  Note: This
  //    class is be used with the Null_Caching_Strategy. The
  //    Cleanup_Strategy is the callback class to which the entries to
  //    be cleaned up will be delegated.
public:

  typedef ACE_Null_Cleanup_Strategy<KEY, VALUE, CONTAINER> CLEANUP_STRATEGY;
  typedef ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> CLEANUP_STRATEGY_BASE;

  ACE_Null_Caching_Utility (ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_strategy = 0,
                            int delete_cleanup_strategy = 0);
  // Constructor.

  ~ACE_Null_Caching_Utility (void);
  // Destructor.

  int clear_cache (CONTAINER &container,
                   double purge_percent);
  // Purge entries from the <container>. The Cleanup_Strategy will do
  // the actual job of cleanup once the entries to be cleaned up are
  // decided. NOte: Here it is a no-op.

protected:

  void minimum (CONTAINER &container,
                KEY *&key_to_remove,
                VALUE *&value_to_remove);
  // Find the entry with minimum caching attributes.  This is handler
  // specific since this utility is to be used very specifically for
  // handler who have caching_attributes for server side acched
  // connection management.Note: Here it is a no-op.

  CLEANUP_STRATEGY_BASE *cleanup_strategy_;
  // The cleanup strategy which can be used to destroy the entries of
  // the container.

  int delete_cleanup_strategy_;
  // Whether the cleanup_strategy should be destroyed or not.
};

///////////////////////////////////////////////////////////////////////////

#if defined (__ACE_INLINE__)
#include "ace/Caching_Utility_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Caching_Utility_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Caching_Utility_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CACHING_UTILITY_H */
