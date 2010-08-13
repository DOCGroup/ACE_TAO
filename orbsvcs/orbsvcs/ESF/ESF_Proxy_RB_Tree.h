// -*- C++ -*-

/**
 *  @file   ESF_Proxy_RB_Tree.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_PROXY_RB_TREE_H
#define TAO_ESF_PROXY_RB_TREE_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

#include "ace/RB_Tree.h"
#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Iterator class for a ACE_ESF_RB_Tree
template<class PROXY>
class TAO_ESF_Proxy_RB_Tree_Iterator
{
public:
  typedef ACE_RB_Tree_Iterator<PROXY *,
                               int,
                               ACE_Less_Than<PROXY*>,
                               ACE_Null_Mutex>
    Implementation;

  TAO_ESF_Proxy_RB_Tree_Iterator (const Implementation &i);

  bool operator == (const TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> &rhs) const;
  bool operator != (const TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> &rhs) const;
  TAO_ESF_Proxy_RB_Tree_Iterator<PROXY>& operator++ (void);
  TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> operator++ (int);
  PROXY *operator *(void);

private:
  Implementation impl_;
};

// ****************************************************************

/// Concrete Proxy collection based on ACE_RB_Tree
/**
 * The Event Service Framework provides several alternatives for the
 * underlying proxy collections.
 * This version is based on Red-Black trees that offer good insertion,
 * removal and lookup performance, but the iteration is slightly
 * degraded.
 */
template<class PROXY>
class TAO_ESF_Proxy_RB_Tree
{
public:
  /// A typedef for the underlying implementaiton class
  typedef ACE_RB_Tree<PROXY*,int,ACE_Less_Than<PROXY*>,ACE_Null_Mutex> Implementation;

  /// A typedef for the underlying iterator
  typedef TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> Iterator;

  /// Constructor
  TAO_ESF_Proxy_RB_Tree (void);

  /// Return the first element in the collection, or end() if there
  /// are none
  TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> begin (void);

  /// Return one past the last element in the collection
  TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> end (void);

  /// Return the number of elements in the collection
  size_t size (void) const;

  /// Insert a new element to the collection
  void connected (PROXY *);

  /// Insert a new element that could be there already.
  void reconnected (PROXY *);
  /// Remove an element from the collection
  void disconnected (PROXY *);

  /// Shutdown the collection, i.e. remove all elements and release
  /// resources
  void shutdown (void);

private:
  /// The underlying implementation object
  Implementation impl_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Proxy_RB_Tree.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/ESF/ESF_Proxy_RB_Tree.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Proxy_RB_Tree.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_PROXY_RB_TREE_H */
