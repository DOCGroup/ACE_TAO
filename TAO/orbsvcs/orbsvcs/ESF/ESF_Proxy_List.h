// -*- C++ -*-

/**
 *  @file   ESF_Proxy_List.h
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

#include /**/ "tao/Versioned_Namespace.h"

#include <cstddef>
#include <list>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// A concrete proxy collection.
/**
 * Uses a doubly linked list to keep iterators stable as proxies are added
 * or removed.
 */
template<class PROXY>
class TAO_ESF_Proxy_List
{
public:
  /// The underlying implementation type.
  using Implementation = std::list<PROXY *>;

  /// The underlying iterator type.
  using Iterator = typename Implementation::iterator;

  /// Constructor
  TAO_ESF_Proxy_List () = default;

  /// Return the first element in the collection, or end() if there
  /// are none
  Iterator begin ();

  /// Return one past the last element in the collection
  Iterator end ();

  /// Return the number of elements in the collection
  std::size_t size () const;

  /// Insert a new element to the collection
  void connected (PROXY *);

  /// Insert a new element that could be there already.
  void reconnected (PROXY *);

  /// Remove an element from the collection
  void disconnected (PROXY *);

  /// Shutdown the collection, i.e. remove all elements and release
  /// resources
  void shutdown ();

private:
  /// The underlying implementation object
  Implementation impl_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Proxy_List.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/ESF/ESF_Proxy_List.cpp"

#endif /* TAO_ESF_PROXY_LIST_H */
