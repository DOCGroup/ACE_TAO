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

template <class KEY, class VALUE, class CONTAINER>
class ACE_Cleanup_Strategy
{
  // = TITLE
  //    Defines a abstract base class which takes care of winding up
  //    and destroying the entries in the container.
  //
  // = DESCRIPTION
  //    This class is one of the ways to ensure that the cleanup
  //    can be decoupled from other strategies which need to do it.
  //    The cleanup method provided needs to be implemented as needed.
  
 public:  

  // = Termination.

  virtual ~ACE_Cleanup_Strategy (void);
  
  // = The cleanup operation.

  virtual int cleanup (CONTAINER &container, KEY *key, VALUE *value) = 0;
  // This pure virtual method is to be used to destroy the <KEY,
  // VALUE> entry.
  
};

//////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class CONTAINER>
class ACE_Default_Cleanup_Strategy : public ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER>
{
  // = TITLE
  //     Defines a default strategy to be followed for cleaning up
  //     entries from a map which is the container.
  //
  // = DESCRIPTION
  //     By defualt the entry to be cleaned up is removed from the
  //     container.

public:
 
  virtual int cleanup (CONTAINER &container, KEY *key, VALUE *value);
  // The method which will do the cleanup of the entry in the container.

};

//////////////////////////////////////////////////////////////////////
template <class KEY, class VALUE, class CONTAINER>
class ACE_Svc_Cleanup_Strategy : public ACE_Cleanup_Strategy<KEY, VALUE, CONTAINER>
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
#pragma implementation ("ace/Cleanup_Strategies_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CLEANUP_STRATEGIES_H */
