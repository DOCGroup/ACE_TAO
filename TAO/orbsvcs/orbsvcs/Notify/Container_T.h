/* -*- C++ -*- */
/**
 *  @file Container_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_CONTAINER_T_H
#define TAO_Notify_CONTAINER_T_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

/**
 * @class TAO_Notify_Container_T
 *
 * @brief A template class that manages a collection.
 * TYPE = type of collection
 *
 */
template <class TYPE>
class TAO_Notify_Export TAO_Notify_Container_T
{
  typedef TAO_ESF_Proxy_Collection<TYPE> COLLECTION;
 public:

  /// Constuctor
  TAO_Notify_Container_T (void);

  /// Destructor
  virtual ~TAO_Notify_Container_T ();

  /// Init this object.
  void init (ACE_ENV_SINGLE_ARG_DECL);

  /// Insert object to this container.
  virtual void insert (TYPE* type ACE_ENV_ARG_DECL);

  /// Remove type from container_
  virtual void remove (TYPE* type ACE_ENV_ARG_DECL);

  /// Shutdown
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// Collection
  COLLECTION* collection (void);

protected:
  /// The collection data structure that we add objects to.
  COLLECTION* collection_;
};

#if defined (__ACE_INLINE__)
#include "Container_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Container_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Container_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_Notify_CONTAINER_T_H */
