/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Caching_Strategy_Utility_T.h
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef CACHING_STRATEGY_UTILITY_H
#define CACHING_STRATEGY_UTILITY_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration
template <class KEY, class VALUE, class CONTAINER>
class ACE_Cleanup_Strategy;
template <class KEY, class VALUE, class CONTAINER, class ATTRIBUTES>
class ACE_Caching_Strategy_Utility
{
  // = TITLE
  //    Defines a abstract helper class for the Caching Strategies.
  // 
  // = DESCRIPTION
  //    This class defines the methods commonly used by the 
  //    different caching strategies. For instance: clear_cache ()
  //    method which decides and purges the entry from the container.

 public:
  
  virtual ~ACE_Caching_Strategy_Utility (void);
  // Destructor.

  virtual int clear_cache (CONTAINER &container, 
                           ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                           const unsigned int purge_percent,
                           unsigned int &entries)  = 0;
  // This method will remove the entries from the cache.
  
  virtual void minimum (CONTAINER &container, 
                        KEY *&key,
                        VALUE *&value) = 0;
  // Find the entry with minimum caching attributes.

};


////////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER, class ATTRIBUTES>
class ACE_Svc_Caching_Strategy_Utility : public ACE_Caching_Strategy_Utility<KEY, VALUE, CONTAINER, ATTRIBUTES>
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
  
  virtual int clear_cache (CONTAINER &container, 
                           ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                           const unsigned int purge_percent,
                           unsigned int &entries);
  // Purge entries from the <container>. The Cleanup_Strategy will do
  // the actual job of cleanup once the entries to be cleaned up are
  // decided.
  
  virtual void minimum (CONTAINER &container, 
                        KEY *&key,
                        VALUE *&value);
  // Find the entry with minimum caching attributes.

};


////////////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER, class ATTRIBUTES>
class ACE_Handler_Caching_Strategy_Utility : public ACE_Caching_Strategy_Utility <KEY, VALUE, CONTAINER, ATTRIBUTES>
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

  virtual int clear_cache (CONTAINER &container, 
                           ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                           const unsigned int purge_percent,
                           unsigned int &entries);
  // Purge entries from the <container>. The Cleanup_Strategy will do
  // the actual job of cleanup once the entries to be cleaned up are
  // decided.
  
  virtual void minimum (CONTAINER &container, 
                        KEY *&key,
                        VALUE *&value);
  // Find the entry with minimum caching attributes.
  // This is handler specific since this utility is to be used very
  // specifically for handler who have caching_attributes for server
  // side acched connection management.

};

///////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER, class ATTRIBUTES>
class ACE_Null_Caching_Strategy_Utility : public ACE_Caching_Strategy_Utility <KEY, VALUE, CONTAINER, ATTRIBUTES>
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

  virtual int clear_cache (CONTAINER &container, 
                           ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER> *cleanup_s,
                           const unsigned int purge_percent,
                           unsigned int &entries);
  // Purge entries from the <container>. The Cleanup_Strategy will do
  // the actual job of cleanup once the entries to be cleaned up are
  // decided. NOte: Here it is a no-op.
  
  virtual void minimum (CONTAINER &container, 
                        KEY *&key,
                        VALUE *&value);
  // Find the entry with minimum caching attributes.
  // This is handler specific since this utility is to be used very
  // specifically for handler who have caching_attributes for server
  // side acched connection management.Note: Here it is a no-op.

};

///////////////////////////////////////////////////////////////////////////

#if defined (__ACE_INLINE__)
#include "ace/Caching_Strategy_Utility_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Caching_Strategy_Utility_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ace/Caching_Strategy_Utility_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CACHING_STRATEGY_UTILITY_H */
