/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Cleanup_Strategies_T.h
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef CLEANUP_STRATEGIES_H
#define CLEANUP_STRATEGIES_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

// For linkers that cant grok long names.
#define ACE_Cleanup_Strategy ACLE

template <class KEY, class VALUE, class CONTAINER>
class ACE_Cleanup_Strategy
{
  // = TITLE
  //     Defines a default strategy to be followed for cleaning up
  //     entries from a map which is the container.
  //
  // = DESCRIPTION
  //     By default the entry to be cleaned up is removed from the
  //     container.

public:

  virtual int cleanup (CONTAINER &container, KEY *key, VALUE *value);
  // The method which will do the cleanup of the entry in the container.
};

//////////////////////////////////////////////////////////////////////
#define ACE_Recyclable_Handler_Cleanup_Strategy ARHCLE

template <class KEY, class VALUE, class CONTAINER>
class ACE_Recyclable_Handler_Cleanup_Strategy : public ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER>
{
  // = TITLE
  //     Defines a strategy to be followed for cleaning up
  //     entries which are svc_handlers from a container.
  //
  // = DESCRIPTION
  //     The entry to be cleaned up is removed from the container.
  //     Here, since we are dealing with svc_handlers specifically, we
  //     perform a couple of extra operations. Note: To be used when
  //     the handler is recyclable.

public:

  virtual int cleanup (CONTAINER &container, KEY *key, VALUE *value);
  // The method which will do the cleanup of the entry in the container.
};

//////////////////////////////////////////////////////////////////////
#define ACE_Refcounted_Recyclable_Handler_Cleanup_Strategy ARRHCLE

template <class KEY, class VALUE, class CONTAINER>
class ACE_Refcounted_Recyclable_Handler_Cleanup_Strategy : public ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER>
{
  // = TITLE
  //     Defines a strategy to be followed for cleaning up
  //     entries which are svc_handlers from a container.
  //
  // = DESCRIPTION
  //     The entry to be cleaned up is removed from the container.
  //     Here, since we are dealing with recyclable svc_handlers with
  //     addresses which are refcountable specifically, we perform a
  //     couple of extra operations and do so without any locking. 

public:

  virtual int cleanup (CONTAINER &container, KEY *key, VALUE *value);
  // The method which will do the cleanup of the entry in the container.
};

//////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER>
class ACE_Handler_Cleanup_Strategy : public ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER>
{
  // = TITLE
  //     Defines a strategy to be followed for cleaning up
  //     entries which are svc_handlers from a container.
  //
  // = DESCRIPTION
  //     The entry to be cleaned up is removed from the container.
  //     Here, since we are dealing with svc_handlers specifically, we
  //     perform a couple of extra operations. Note: This cleanup strategy
  //     should be used in the case when the handler has the caching
  //     attributes.

public:

  virtual int cleanup (CONTAINER &container, KEY *key, VALUE *value);
  // The method which will do the cleanup of the entry in the container.
};

//////////////////////////////////////////////////////////////////////
#define ACE_Null_Cleanup_Strategy ANCLE

template <class KEY, class VALUE, class CONTAINER>
class ACE_Null_Cleanup_Strategy : public ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER>
{
  // = TITLE
  //     Defines a do-nothing implementation of the cleanup strategy.
  //
  // = DESCRIPTION
  //     This class simply does nothing at all! Can be used to nullify
  //     the effect of the Cleanup Strategy.

public:

  virtual int cleanup (CONTAINER &container, KEY *key, VALUE *value);
  // The dummy cleanup method.
};

#if defined (__ACE_INLINE__)
#include "ace/Cleanup_Strategies_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Cleanup_Strategies_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Cleanup_Strategies_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CLEANUP_STRATEGIES_H */
