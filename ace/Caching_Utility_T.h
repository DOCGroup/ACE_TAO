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

// Forward declaration
template <class KEY, class VALUE, class CONTAINER>
class ACE_Cleanup_Strategy;

template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES>
class ACE_Pair_Caching_Utility
{
  // = TITLE
  //    Defines a helper class for the Caching Strategies.
  //
  // = DESCRIPTION
  //    This class defines the methods commonly used by the
  //    different caching strategies. For instance: clear_cache ()
  //    method which decides and purges the entry from the container.
  //    Note: This class helps in the caching_strategies using a
  //    container containing entries of <KEY, ACE_Pair<VALUE,
  //    attributes>> kind. The attributes helps in deciding the
  //    entries to be purged.
public:

  int clear_cache (CONTAINER &container,
                   ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                   double purge_percent);
  // Purge entries from the <container>. The Cleanup_Strategy will do
  // the actual job of cleanup once the entries to be cleaned up are
  // decided.

protected:

  void minimum (CONTAINER &container,
                KEY *&key_to_remove,
                VALUE *&value_to_remove);
  // Find the entry with minimum caching attributes.
};


////////////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER, class ITERATOR, class ATTRIBUTES>
class ACE_Handler_Caching_Utility
{
  // = TITLE
  //    Defines a helper class for the Caching Strategies.
  //
  // = DESCRIPTION
  //    This class defines the methods commonly used by the
  //    different caching strategies. For instance: clear_cache ()
  //    method which decides and purges the entry from the container.
  //    Note: This class helps in the caching_strategies using a
  //    container containing entries of <KEY, HANDLER> kind where the
  //    HANDLER contains the caching attributes which help in deciding
  //    the entries to be purged.
public:

  int clear_cache (CONTAINER &container,
                   ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
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
};

///////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER,  class ITERATOR, class ATTRIBUTES>
class ACE_Null_Caching_Utility
{
  // = TITLE
  //    Defines a dummy helper class for the Caching Strategies.
  //
  // = DESCRIPTION
  //    This class defines the methods commonly used by the
  //    different caching strategies. For instance: clear_cache ()
  //    method which decides and purges the entry from the container.
  //    Note: This class is be used with the Null_Caching_Strategy.
public:

  int clear_cache (CONTAINER &container,
                   ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
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
