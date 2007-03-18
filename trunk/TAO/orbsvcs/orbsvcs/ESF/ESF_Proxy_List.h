// -*- C++ -*-

/**
 *  @file   ESF_Proxy_List.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_PROXY_LIST_H
#define TAO_ESF_PROXY_LIST_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Unbounded_Set.h"
#include "ace/Containers.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// A concrete proxy collection.
/**
 * Based on the ACE_Unbounded_Set<> collection, used a double
 * linked list internally.
 */
template<class PROXY>
class TAO_ESF_Proxy_List
{
public:
  /// A typedef for the underlying implementaiton class
  typedef ACE_Unbounded_Set<PROXY*> Implementation;

  /// A typedef for the underlying iterator
  typedef ACE_Unbounded_Set_Iterator<PROXY*> Iterator;

  /// Constructor
  TAO_ESF_Proxy_List (void);

  /// Return the first element in the collection, or end() if there
  /// are none
  ACE_Unbounded_Set_Iterator<PROXY*> begin (void);

  /// Return one past the last element in the collection
  ACE_Unbounded_Set_Iterator<PROXY*> end (void);

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
  ACE_Unbounded_Set<PROXY*> impl_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Proxy_List.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/ESF/ESF_Proxy_List.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Proxy_List.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_PROXY_LIST_H */
