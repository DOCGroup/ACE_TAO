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
#include <math.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration
template <class CONTAINER>
class ACE_Cleanup_Strategy;

template <class CONTAINER, class ATTRIBUTES>
class ACE_Caching_Strategy_Utility
{
  // = TITLE
  //    Defines a helper class for the Caching Strategies.
  // 
  // = DESCRIPTION
  //    This class defines the methods commonly used by the 
  //    different caching strategies. For instance: clear_cache ()
  //    method which decides and purges the entry from the container.

 public:
  
  // = Traits.
  typedef ACE_TYPENAME CONTAINER::KEY KEY;
  typedef ACE_TYPENAME CONTAINER::VALUE VALUE;

  int clear_cache (CONTAINER &container, 
                   ACE_Cleanup_Strategy<CONTAINER> *cleanup_s,
                   const unsigned int purge_percent,
                   unsigned int &entries);
  // Purge entries from the <container>. The <entries> had to be
  // passed by refernce as it wuold reduce on cleanup.
  
  void minimum (CONTAINER &container, 
                KEY *&key,
                VALUE *&value);
  // Find the entry with minimum caching attributes.

};

#if defined (__ACE_INLINE__)
#include "ace/Caching_Strategy_Utility_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Caching_Strategy_Utility_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation "ace/Caching_Strategy_Utility_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CACHING_STRATEGY_UTILITY_H */
